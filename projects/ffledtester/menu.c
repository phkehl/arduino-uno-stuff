/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's LED tester, menu functions (see \ref PROJECTS_FFLEDTESTER_MENU)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER_MENU

    @{
*/

#include <string.h>        // libc: string operations

#include <avr/pgmspace.h>  // avr-libc: program Space Utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ws2812.h"        // ff: WS2812 LED driver
#include "ledfx.h"         // ff: LED effects
#include "hsv2rgb.h"       // ff: HSV to RGV conversion
#include "rotenc.h"        // ff: rotary encoder input
#include "tone.h"          // ff: tone and melody generator

#include "menu.h"
#include "dl2416t.h"


/* ***** menu stuff ********************************************************** */


// forward declarations
static void sMenuUpdate(MENU_STATE_t *pState, const MENU_t *pkMenu);

// initialise menu state and values
void menuInit(MENU_STATE_t *pState, const MENU_t *pkMenu, const uint8_t nMenu, int16_t *pVals)
{
    memset(pState, 0, sizeof(*pState));
    memset(pVals, 0, nMenu * sizeof(*pVals));
    pState->pkMenu = pkMenu;
    pState->nMenu  = nMenu;
    pState->vals   = pVals;
    for (uint8_t ix = 0; ix < nMenu; ix++)
    {
        switch (MENU_TYPE(&pkMenu[ix]))
        {
            case MENU_TYPE_VAL:
            case MENU_TYPE_HEX:
                pVals[ix] = MENU_DEF(&pkMenu[ix]);
                break;
            case MENU_TYPE_STR:
                break;
            case MENU_TYPE_FUNC:
                break;
            case MENU_TYPE_MENU:
            case MENU_TYPE_JUMP:
                break;
        }
    }
    sMenuUpdate(pState, NULL);
}

// dump menu recursively
static void sMenuDumpHelper(const MENU_t *pkMenu, const uint8_t nMenu, const uint8_t pid, const uint8_t indent)
{
    char indentBuf[10];
    memset(indentBuf, ' ', sizeof(indentBuf));
    indentBuf[sizeof(indentBuf) - 1] = '\0';
    if (indent < (sizeof(indentBuf) - 1))
    {
        indentBuf[indent] = '\0';
    }
    for (uint8_t ix = 0; ix < nMenu; ix++)
    {
        MENU_P_t *pkEntry = &pkMenu[ix];
        if (MENU_PID(pkEntry) == pid)
        {
            // use LED frame buffer for temp string
            char *buf = (char *)ledfxGetFrameBuffer();
            const uint16_t size = ledfxGetFrameBufferSize();
            buf[0] = '\0';
            switch (MENU_TYPE(pkEntry))
            {
                case MENU_TYPE_VAL:
                    snprintf_P(buf, size, PSTR(" [%c=%"PRIi16"..%"PRIi16"]"),
                        MENU_IND(pkEntry), MENU_MIN(pkEntry), MENU_MAX(pkEntry));
                    break;
                case MENU_TYPE_HEX:
                    snprintf_P(buf, size, PSTR(" [%c=0x%"PRIx16"..0x%"PRIx16"]"),
                        (uint16_t)MENU_IND(pkEntry), (uint16_t)MENU_MIN(pkEntry), MENU_MAX(pkEntry));
                    break;
                case MENU_TYPE_STR:
                {
                    for (uint8_t sIx = 0; sIx < MENU_NSTRS(pkEntry); sIx++)
                    {
                        if (sIx == 0)
                        {
                            snprintf_P(buf, size, PSTR(" [%S"), MENU_STR_IX(pkEntry, sIx));
                        }
                        else
                        {
                            const uint16_t len = strlen(buf);
                            snprintf_P(&buf[len], size - len, PSTR(", %S"), MENU_STR_IX(pkEntry, sIx));
                        }
                    }
                    uint16_t len = strlen(buf);
                    if (len < (size - 1))
                    {
                        buf[len]     = ']';
                        buf[len + 1] = '\0';
                    }
                    else
                    {
                        buf[len - 1] = '\0';
                    }
                    break;
                }
                case MENU_TYPE_FUNC:
                case MENU_TYPE_MENU:
                case MENU_TYPE_JUMP:
                    break;
            }

            PRINT_W("%s- %S%s", indentBuf, MENU_NAME(pkEntry), buf);
            sMenuDumpHelper(pkMenu, nMenu, MENU_MID(pkEntry), indent + 2);
            ledfxClear(0, 0);
        }
    }
}

// dump whole menu structure
void menuDump(const MENU_t *pkMenu, const uint8_t nMenu)
{
    NOTICE_W("The menu:");
    sMenuDumpHelper(pkMenu, nMenu, 0, 2);
}

// activate / update menu entry
static void sMenuUpdate(MENU_STATE_t *pState, const MENU_t *pkMenu)
{
    if (pkMenu == NULL)
    {
        pState->pkCurr = pState->pkMenu;
    }
    else if (pState->pkCurr != pkMenu)
    {
        pState->pkCurr = pkMenu;
    }
    //DEBUG("menu: mid=%02"PRIu8" pid=%02"PRIu8" ix=%02"PRIu8" -> %S (%S) %"PRIi16,
    //    MENU_MID(pState->pkCurr), MENU_PID(pState->pkCurr), MENU_IX(pState->pkMenu, pState->pkCurr),
    //    MENU_NAME(pState->pkCurr), pState->active ? PSTR("active") : PSTR("menu"),
    //    pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ]);
    if (pState->active)
    {
        switch (MENU_TYPE(pState->pkCurr))
        {
            case MENU_TYPE_VAL:
            {
                const int16_t min = MENU_MIN(pState->pkCurr);
                const int16_t max = MENU_MAX(pState->pkCurr);
                int16_t val = pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ];
                dl2416tWrite(0, MENU_IND(pState->pkCurr));
                if (min < 0)
                {
                    dl2416tWrite(1, val < 0 ? '-' : '+');
                    dl2416tUnsigned(ABS(val), 2, 2);
                }
                else if (max > 99)
                {
                    dl2416tUnsigned(val, 1, 3);
                }
                else
                {
                    dl2416tUnsigned(val, 2, 2);
                }
                break;
            }
            case MENU_TYPE_HEX:
            {
                int16_t val = pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ];
                dl2416tWrite(0, MENU_IND(pState->pkCurr));
                dl2416tWrite(1, ' ');
                dl2416tHex(val, 2, 2);
                break;
            }
            case MENU_TYPE_STR:
            {
                const int16_t ix = pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ];
                dl2416tStr_P(MENU_STR_IX(pState->pkCurr, ix), 0, 4);
                break;
            }
            case MENU_TYPE_FUNC:
                break;
            case MENU_TYPE_MENU:
            case MENU_TYPE_JUMP:
                dl2416tStr_P(PSTR("WTF?"), 0, 4);
                break;
        }
    }
    else
    {
        dl2416tStr_P(MENU_NAME(pState->pkCurr), 0, 4);
    }
}

// handle menu events
void menuHandle(MENU_STATE_t *pState, const ROTENC_EVENT_t event)
{
    // change menu entry
    if (!pState->active)
    {
        switch (event)
        {
            // go to next menu entry
            case ROTENC_INC:
            case ROTENC_INC_DN:
            {
                toneGenerate(TONE_NOTE_A6, 20);
                const MENU_t *pkCand = pState->pkCurr + 1;
                while (pkCand < &pState->pkMenu[pState->nMenu])
                {
                    if (MENU_PID(pkCand) == MENU_PID(pState->pkCurr))
                    {
                        sMenuUpdate(pState, pkCand);
                        break;
                    }
                    pkCand++;
                }
                break;
            }
            // go to previous menu entry
            case ROTENC_DEC:
            case ROTENC_DEC_DN:
            {
                toneGenerate(TONE_NOTE_A6, 20);
                const MENU_t *pkCand = pState->pkCurr - 1;
                while (pkCand >= pState->pkMenu)
                {
                    if (MENU_PID(pkCand) == MENU_PID(pState->pkCurr))
                    {
                        sMenuUpdate(pState, pkCand);
                        break;
                    }
                    pkCand--;
                }
                break;
            }
            // activate (to change value) or enter sub-menu
            case ROTENC_BTN:
                toneGenerate(TONE_NOTE_B5, 50);
                switch (MENU_TYPE(pState->pkCurr))
                {
                    // enter sub-menu
                    case MENU_TYPE_MENU:
                    {
                        // go back where we were..
                        const uint8_t pid = pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ];
                        // ..or to the first of this menu group
                        const uint8_t mid = MENU_MID(pState->pkCurr);
                        const MENU_t *pkCand = pState->pkMenu;
                        while (pkCand < &pState->pkMenu[pState->nMenu])
                        {
                            if (pid != 0)
                            {
                                if (MENU_MID(pkCand) == pid)
                                {
                                    sMenuUpdate(pState, pkCand);
                                    break;
                                }
                            }
                            else
                            {
                                if (MENU_PID(pkCand) == mid)
                                {
                                    sMenuUpdate(pState, pkCand);
                                    break;
                                }
                            }
                            pkCand++;
                        }
                        break;
                    }
                    // jump to menu entry
                    case MENU_TYPE_JUMP:
                    {
                        const uint8_t mid = MENU_JUMP(pState->pkCurr);
                        const MENU_t *pkCand = pState->pkMenu;
                        while (pkCand < &pState->pkMenu[pState->nMenu])
                        {
                            if (MENU_MID(pkCand) == mid)
                            {
                                // don't remember where we were last
                                pState->vals[ MENU_IX(pState->pkMenu, pkCand) ] = 0;
                                sMenuUpdate(pState, pkCand);
                                break;
                            }
                            pkCand++;
                        }
                        break;
                    }
                    // activate entry
                    case MENU_TYPE_VAL:
                    case MENU_TYPE_HEX:
                    case MENU_TYPE_STR:
                        pState->active = true;
                        sMenuUpdate(pState, pState->pkCurr);
                        break;
                    // call function
                    case MENU_TYPE_FUNC:
                        MENU_FUNC(pState->pkCurr)();
                        break;
                }
                break;
            // leave sub-menu
            case ROTENC_BTN_LONG:
            {
                toneGenerate(TONE_NOTE_D5, 25);
                osTaskDelay(30);
                toneGenerate(TONE_NOTE_B5, 25);
                const uint8_t pid = MENU_PID(pState->pkCurr);
                const MENU_t *pkCand = pState->pkMenu;
                while (pkCand < &pState->pkMenu[pState->nMenu])
                {
                    if (MENU_MID(pkCand) == pid)
                    {
                        // remember where we were last
                        pState->vals[ MENU_IX(pState->pkMenu, pkCand) ] = MENU_MID(pState->pkCurr);
                        sMenuUpdate(pState, pkCand);
                        break;
                    }
                    pkCand++;
                }
                break;
            }
            default:
                break;
        }
    }
    // change value in currently active menu entry
    else
    {
        bool update = false;
        bool blink = false;
        switch (MENU_TYPE(pState->pkCurr))
        {
            // not handled here (these should never be active)
            case MENU_TYPE_MENU:
            case MENU_TYPE_JUMP:
            case MENU_TYPE_FUNC:
                DEBUG("WTF?!");
                break;

            // adjust value
            case MENU_TYPE_VAL:
            case MENU_TYPE_HEX:
            {
                update = true;
                const bool wrap = MENU_WRAP(pState->pkCurr);
                const int16_t min = MENU_MIN(pState->pkCurr);
                const int16_t max = MENU_MAX(pState->pkCurr);
                int16_t val = pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ];
                if (val < min) { val = min; }
                if (val > max) { val = max; }
                switch (event)
                {
                    case ROTENC_INC:
                        toneGenerate(TONE_NOTE_A6, 20);
                        if (val < max)
                        {
                            val++;
                        }
                        else if (wrap)
                        {
                            val = min;
                        }
                        break;
                    case ROTENC_INC_DN:
                        toneGenerate(TONE_NOTE_E6, 20);
                        if (val < (max - 10))
                        {
                            val += 10;
                        }
                        else if (wrap)
                        {
                            val += 10;
                            val = min + (val - max - 1);
                        }
                        break;
                    case ROTENC_DEC:
                        toneGenerate(TONE_NOTE_A6, 20);
                        if (val > min)
                        {
                            val--;
                        }
                        else if (wrap)
                        {
                            val = max;
                        }
                        break;
                    case ROTENC_DEC_DN:
                        toneGenerate(TONE_NOTE_E6, 20);
                        if (val > (min + 10))
                        {
                            val -= 10;
                        }
                        else if (wrap)
                        {
                            val -= 10;
                            val = max - (min - val - 1);
                        }
                        break;
                    // back to menu
                    case ROTENC_BTN:
                        toneGenerate(TONE_NOTE_D5, 50);
                        pState->active = false;
                        break;
                    // reset to default
                    case ROTENC_BTN_LONG:
                        toneGenerate(TONE_NOTE_D5, 25);
                        osTaskDelay(30);
                        toneGenerate(TONE_NOTE_B5, 25);
                        val = MENU_DEF(pState->pkCurr);
                        blink = true;
                        break;
                    case ROTENC_NONE:
                        break;
                }
                pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ] = val;
                break;
            }

            case MENU_TYPE_STR:
            {
                update = true;
                const bool wrap = MENU_WRAP(pState->pkCurr);
                const uint8_t nStrs = MENU_NSTRS(pState->pkCurr);
                const uint8_t maxIx = nStrs - 1;
                //const char   **strs = MENU_STRS(pState->pkCurr);
                int16_t val = pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ];
                if (val < 0) { val = 0; }
                if (val > maxIx) { val = maxIx; }
                switch (event)
                {
                    case ROTENC_INC:
                    case ROTENC_INC_DN:
                        toneGenerate(TONE_NOTE_A6, 20);
                        if (val < maxIx)
                        {
                            val++;
                        }
                        else if (wrap)
                        {
                            val = 0;
                        }
                        break;
                    case ROTENC_DEC:
                    case ROTENC_DEC_DN:
                        toneGenerate(TONE_NOTE_A6, 20);
                        if (val > 0)
                        {
                            val--;
                        }
                        else if (wrap)
                        {
                            val = maxIx;
                        }
                        break;
                    // back to menu
                    case ROTENC_BTN:
                        toneGenerate(TONE_NOTE_D5, 50);
                        pState->active = false;
                        break;
                    case ROTENC_BTN_LONG:
                    case ROTENC_NONE:
                        break;
                }
                pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ] = val;
                break;
            }

        }
        if (update)
        {
            sMenuUpdate(pState, pState->pkCurr);
        }
        if (blink)
        {
            dl2416tBlink(2, 100, 100);
        }
    }
}



//------------------------------------------------------------------------------

//@}
// eof
