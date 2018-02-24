/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's LED tester (see \ref PROJECTS_FFLEDTESTER)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER

    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
#include <avr/pgmspace.h>  // avr-libc: program Space Utilities
#include <avr/eeprom.h>    // avr-libc: EEPROM handling

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

#include "ffledtester.h"
#include "dl2416t.h"


/* ***** menu stuff ********************************************************** */

// type of menu entry
typedef enum MENU_TYPE_e
{
    MENU_TYPE_STR,  // entry is list of strings to choose from
    MENU_TYPE_VAL,  // entry is value
    MENU_TYPE_HEX,  // entry is value, display hex
    MENU_TYPE_MENU, // entry is a sub-menu
    MENU_TYPE_JUMP, // entry is jump (link) elsewhere
    MENU_TYPE_FUNC, // entry calls function

} MENU_TYPE_t;

// menu entries
typedef struct MENU_s
{
    uint8_t  mid;  // unique menu ID
    uint8_t  pid;  // parent menu ID (or 0 for main menu entries)

    MENU_TYPE_t type;         // the entry type
    const char  name[16];     // the name
    bool        wrap;         // wrap at beginning/end of list/value (only for MENU_TYPE_STR/MENU_TYPE_NUM)
    union
    {
        struct
        {                         // type = MENU_TYPE_STR:
            const __flash char * const __flash *strs;    // - strings to choose from
            uint8_t nStrs;        // - the number of strings
        };
        struct
        {                         // type = MENU_TYPE_NUM
            char    ind;          // - indicator
            int16_t min;          // - minimum value
            int16_t max;          // - maximum value
            int16_t def;          // - default value
        };
        struct
        {                         // type = MENU_TYPE_JUMP
            uint8_t jump;         // - mid to jump to
        };
        struct                    // type = MENU_TYPE_FUNC
        {
            void (*func)(void);   // function to call

        };
    };
} MENU_t;

// pointer to MENU_t in flash
typedef const __flash MENU_t MENU_P_t;

#define MENU_MID(pkMenu)         ((uint8_t)pgm_read_byte(&((pkMenu)->mid)))
#define MENU_PID(pkMenu)         ((uint8_t)pgm_read_byte(&((pkMenu)->pid)))
#define MENU_TYPE(pkMenu)        ((MENU_TYPE_t)pgm_read_byte(&((pkMenu)->type)))
#define MENU_NAME(pkMenu)        ((pkMenu)->name)
#define MENU_IX(pkMenu, pkCurr)  ((uint8_t)((pkCurr) - (pkMenu)))
#define MENU_WRAP(pkMenu)        ((bool)pgm_read_byte(&((pkMenu)->wrap)))
#define MENU_NSTRS(pkMenu)       ((uint8_t)pgm_read_byte(&((pkMenu)->nStrs)))
#define MENU_STRS(pkMenu)        ((pkMenu)->strs))
#define MENU_STR_IX(pkMenu, ix)  ((pkMenu)->strs[ix])
#define MENU_MIN(pkMenu)         ((int16_t)pgm_read_word(&((pkMenu)->min)))
#define MENU_MAX(pkMenu)         ((int16_t)pgm_read_word(&((pkMenu)->max)))
#define MENU_IND(pkMenu)         ((char)pgm_read_byte(&((pkMenu)->ind)))
#define MENU_DEF(pkMenu)         ((int16_t)pgm_read_byte(&((pkMenu)->def)))
#define MENU_JUMP(pkMenu)        ((uint8_t)pgm_read_byte(&((pkMenu)->jump)))
#define MENU_FUNC(pkMenu)        ((void (*)(void))pgm_read_word(&((pkMenu)->func)))

// helper macros to generate consistent MENU_t array and value struct
#define M_STR(_mid, _pid, _name, _var, _wrap, _def, _strs) \
    { .type = MENU_TYPE_STR, .mid = (_mid), .pid = (_pid), .name = (_name), .wrap = (_wrap), .def = (_def), .strs = (_strs), .nStrs = NUMOF(_strs) },
#define M_VAL(_mid, _pid, _name, _var, _wrap, _def, _ind, _min, _max) \
    { .type = MENU_TYPE_VAL, .mid = (_mid), .pid = (_pid), .name = (_name), .wrap = (_wrap), .def = (_def), .ind = (_ind), .min = (_min), .max = (_max) },
#define M_HEX(_mid, _pid, _name, _var, _wrap, _def, _ind, _min, _max) \
    { .type = MENU_TYPE_HEX, .mid = (_mid), .pid = (_pid), .name = (_name), .wrap = (_wrap), .def = (_def), .ind = (_ind), .min = (_min), .max = (_max) },
#define M_MEN(_mid, _pid, _name, _var) \
    { .type = MENU_TYPE_MENU, .mid = (_mid), .pid = (_pid), .name = (_name) },
#define M_JMP(_mid, _pid, _name, _var, _jump) \
    { .type = MENU_TYPE_JUMP, .mid = (_mid), .pid = (_pid), .name = (_name), .jump = (_jump) },
#define M_FUN(_mid, _pid, _name, _var, _func) \
    { .type = MENU_TYPE_FUNC, .mid = (_mid), .pid = (_pid), .name = (_name), .func = (_func) },
#define M_VAR(_mod, _pid, _name, _var, ...) int16_t _var;

typedef struct MENU_STATE_s
{
    MENU_P_t *pkMenu; // list of menu entries
    uint8_t   nMenu;  // number of menu entries
    MENU_P_t *pkCurr; // current menu
    bool      active; // true if entry is active
    int16_t  *vals;   // storage for the values associated with each entry (must be big enough for nMenu values)
} MENU_STATE_t;

// forward declarations
static void sMenuUpdate(MENU_STATE_t *pState, const MENU_t *pkMenu);

// initialise menu state and values
static void sMenuInit(MENU_STATE_t *pState, const MENU_t *pkMenu, const uint8_t nMenu, int16_t *pVals)
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
static void sMenuDump(const MENU_t *pkMenu, const uint8_t nMenu)
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
static void sMenuHandle(MENU_STATE_t *pState, const ROTENC_EVENT_t event)
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


/* ***** LED stuff *********************************************************** */

const __flash char skModeMenuStr0[] = "MRGB";
const __flash char skModeMenuStr1[] = "MHSV";
const __flash char skModeMenuStr2[] = "MPAT";
const __flash char * const __flash skModeMenuStrs[] =
{
    skModeMenuStr0, skModeMenuStr1, skModeMenuStr2
};
typedef enum MODE_e { MODE_RGB = 0, MODE_HSV = 1, MODE_PATTERN = 2 } MODE_t;

const __flash char skOrderMenuStr0[] = "ORGB";
const __flash char skOrderMenuStr1[] = "ORBG";
const __flash char skOrderMenuStr2[] = "OGRB";
const __flash char skOrderMenuStr3[] = "OGBR";
const __flash char skOrderMenuStr4[] = "OBRG";
const __flash char skOrderMenuStr5[] = "OBGR";
const __flash char * const __flash skOrderMenuStrs[] =
{
    skOrderMenuStr0, skOrderMenuStr1, skOrderMenuStr2, skOrderMenuStr3, skOrderMenuStr4, skOrderMenuStr5
};
typedef enum ORDER_e { ORDER_RGB = 0, ORDER_RBG = 1, ORDER_GRB = 2, ORDER_GBR = 3, ORDER_BRG = 4, ORDER_BGR = 5 } ORDER_t;

const __flash char skDriverMenuStr0[] = "NONE";
const __flash char skDriverMenuStr1[] = "2801";
const __flash char skDriverMenuStr2[] = "2812";
const __flash char * const __flash skDriverMenuStrs[] =
{
    skDriverMenuStr0, skDriverMenuStr1, skDriverMenuStr2
};
typedef enum DRIVER_e { DRIVER_NONE = 0, DRIVER_WS2801 = 1, DRIVER_WS2812 = 2 } DRIVER_t;

const __flash char skCharsMenuStr0[] = " !\"#";
const __flash char skCharsMenuStr1[] = "$%&'";
const __flash char skCharsMenuStr2[] = "()*+";
const __flash char skCharsMenuStr3[] = ",-./";
const __flash char skCharsMenuStr4[] = "0123";
const __flash char skCharsMenuStr5[] = "4567";
const __flash char skCharsMenuStr6[] = "89:;";
const __flash char skCharsMenuStr7[] = "<=>?";
const __flash char skCharsMenuStr8[] = "@ABC";
const __flash char skCharsMenuStr9[] = "DEFG";
const __flash char skCharsMenuStrA[] = "HIJK";
const __flash char skCharsMenuStrB[] = "LMNO";
const __flash char skCharsMenuStrC[] = "PQRS";
const __flash char skCharsMenuStrD[] = "TUVW";
const __flash char skCharsMenuStrE[] = "XYZ[";
const __flash char skCharsMenuStrF[] = "\\]^_";
const __flash char * const __flash skCharsMenuStrs[] =
{
    skCharsMenuStr0, skCharsMenuStr1, skCharsMenuStr2, skCharsMenuStr3,
    skCharsMenuStr4, skCharsMenuStr5, skCharsMenuStr6, skCharsMenuStr7,
    skCharsMenuStr8, skCharsMenuStr9, skCharsMenuStrA, skCharsMenuStrB,
    skCharsMenuStrC, skCharsMenuStrD, skCharsMenuStrE, skCharsMenuStrF
};

static void sHardReset(void)
{
    PRINT_W("RESET");

    for (uint8_t n = 0; n < 5; n++)
    {
        dl2416tBlank(true);
        osTaskDelay(100);
        toneGenerate(TONE_NOTE_D5, 25);
        dl2416tBlank(false);
        osTaskDelay(150);
    }

    dl2416tStr_P(PSTR("BOOM"), 0, 4);

    for (uint8_t n = 0; n < 10; n++)
    {
        dl2416tBlank(true);
        osTaskDelay(50);
        toneGenerate(TONE_NOTE_G3, 25);
        dl2416tBlank(false);
        osTaskDelay(50);
    }

    dl2416tClear();
    hwReset(HW_RESET_HARD);
}

// our menu structure
#define MENU1_DEF(_STR, _VAL, _HEX, _MEN, _JMP, _FUN) \
    /*    mid pid  name            var    STR: wrap   default      strs */ \
    /*                                VAL/HEX: wrap   default      ind min max */ \
    /*                                    MEN: -- */ \
    /*                                    FUN:                     function */ \
    /*                                    JMP:                     jump */ \
    _STR( 11,  0, "1 DR (driver)\0",  driver,  false, DRIVER_NONE, skDriverMenuStrs ) \
    _STR( 12,  0, "2 MO (mode)\0",    mode,    false, MODE_RGB,    skModeMenuStrs) \
    _STR( 13,  0, "3 OR (order)\0",   order,   false, ORDER_RGB,   skOrderMenuStrs ) \
    _MEN( 14,  0, "4 MA (matrix)\0",  matrix ) \
    _MEN( 15,  0, "5 PA (params)\0",  params ) \
    _MEN( 16,  0, "5 PT (pattern)\0", patmen ) \
    _MEN( 17,  0, "A HD (hex-dec)\0", hexdec ) \
    _STR( 18,  0, "B CH (chars)\0",   chars,   true,  0,           skCharsMenuStrs ) \
    _FUN( 19,  0, "KILL (reset)\0",   kill,                        sHardReset ) \
    \
    /* matrix menu */ \
    _VAL( 41, 14, "1 N# (total)\0",   nxy,     false, 5,           '#', 0, FF_LEDFX_NUM_LED ) \
    _VAL( 42, 14, "2 NX (n_x)\0",     nx,      false, 8,           'X', 1, 10 ) \
    _VAL( 43, 14, "3 NY (n_y)\0",     ny,      false, 8,           'Y', 1, 10 ) \
    /* TODO: XY arrangement */ \
    _JMP( 44, 14, "X (- (return)\0",  ret2,                        14 ) \
    \
    /* params menu */ \
    _VAL( 51, 15, "1 RD (red)\0",     red,     false, 1,           'R', 0, 255 )   \
    _VAL( 52, 15, "2 GN (green)\0",   green,   false, 1,           'G', 0, 255 ) \
    _VAL( 53, 15, "3 BL (blue)\0",    blue,    false, 1,           'B', 0, 255 ) \
    _VAL( 54, 15, "4 HU (hue)\0",     hue,     true,  0,           'H', 0, 255 ) \
    _VAL( 55, 15, "5 SA (sat)\0",     sat,     false, 255,         'S', 0, 255 ) \
    _VAL( 56, 15, "6 VA (val)\0",     val,     false, 1,           'V', 0, 255 ) \
    _JMP( 57, 15, "X (- (return)\0",  ret0,                        15 ) \
    \
    /* pattern menu */ \
    _VAL( 61, 16, "1 PA (pattern)\0", pattern, false, 1,           'P', 1,   3 ) \
    _VAL( 62, 16, "2 HZ (speed)\0",   hz,      false, 5,           'S', 1,  50 ) \
    _VAL( 63, 16, "3 PE (period)\0",  period,  false, 5,           'R', 2,  2 * FF_LEDFX_NUM_LED ) \
    _JMP( 64, 16, "X (- (return)\0",  ret1,                        16 ) \
    \
    /* hex-dec menu */ \
    _HEX( 71, 17, "1HEX (hex)\0",     hex,     true,  0,           'X', 0, 255 ) \
    _VAL( 72, 17, "2DEC (dec)\0",     dec,     true,  0,           'D', 0, 255 ) \
    _JMP( 73, 17, "X (- (return)\0",  ret3,                        17 )

// menu structure
static const MENU_t skMenu1[] PROGMEM = { MENU1_DEF(M_STR, M_VAL, M_HEX, M_MEN, M_JMP, M_FUN) };

// storage for menu values
typedef union MENU1_VALUES_u
{
    // this many values
    int16_t values[NUMOF(skMenu1)];
    // and direct, named access (offset must be in sync with the above list)
    struct { MENU1_DEF(M_VAR, M_VAR, M_VAR, M_VAR, M_VAR, M_VAR) };

} MENU1_VALUES_t;

#define MENU1_HEX_IX (offsetof(MENU1_VALUES_t, hex) / sizeof(int16_t))
#define MENU1_DEC_IX (offsetof(MENU1_VALUES_t, dec) / sizeof(int16_t))

// -------------------------------------------------------------------------------------------------


static void sUpdatePattern(const MENU1_VALUES_t *pkVal);

static void sSwapRGB(const ORDER_t order, uint8_t *pR, uint8_t *pG, uint8_t *pB)
{
    uint8_t ch1, ch2, ch3;
    switch (order)
    {
        case ORDER_RBG: ch1 = *pR; ch2 = *pB; ch3 = *pG; break;
        case ORDER_GRB: ch1 = *pG; ch2 = *pR; ch3 = *pB; break;
        case ORDER_GBR: ch1 = *pG; ch2 = *pB; ch3 = *pR; break;
        case ORDER_BRG: ch1 = *pB; ch2 = *pR; ch3 = *pG; break;
        case ORDER_BGR: ch1 = *pB; ch2 = *pG; ch3 = *pR; break;
        default:
        case ORDER_RGB: ch1 = *pR; ch2 = *pG; ch3 = *pB; break;
    }
    *pR = ch1;
    *pG = ch2;
    *pB = ch3;
}

static void sUpdateLeds(const MENU1_VALUES_t *pkVal)
{
    uint8_t rd, gr, bl;
    const MODE_t mode = (MODE_t)pkVal->mode;
    switch (mode)
    {
        case MODE_RGB:
            rd = pkVal->red;
            gr = pkVal->green;
            bl = pkVal->blue;
            break;
        case MODE_HSV:
            hsv2rgb(pkVal->hue, pkVal->sat, pkVal->val, &rd, &gr, &bl);
            break;
        case MODE_PATTERN:
            break;
    }

    if (mode != MODE_PATTERN)
    {
        // adjust for colour ordering
        sSwapRGB((ORDER_t)pkVal->order, &rd, &gr, &bl);
        ledfxClear(0, 0);
        for (int16_t ix = 0; ix < pkVal->nxy; ix++)
        {
            ledfxSetIxRGB(ix, rd, gr, bl);
        }
    }
    else
    {
        sUpdatePattern(pkVal);
    }

    // detect driver switch
    static DRIVER_t oldDriver = DRIVER_NONE;
    const DRIVER_t newDriver = (DRIVER_t)pkVal->driver;
    if (oldDriver != newDriver)
    {
        DEBUG("driver switch");
        switch (oldDriver)
        {
            case DRIVER_WS2801:
                ws2801Send(NULL, ledfxGetFrameBufferSize());
                break;
            case DRIVER_WS2812:
                ws2812Send(WS2812_PIN, NULL, ledfxGetFrameBufferSize());
                break;
            default:
                break;
        }
        oldDriver = newDriver;
    }

    // send data
    switch (newDriver)
    {
        case DRIVER_WS2801:
            ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
            break;
        case DRIVER_WS2812:
            ws2812Send(WS2812_PIN, ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
            break;
        default:
            break;
    }
}

static void sUpdatePattern(const MENU1_VALUES_t *pkVal)
{
    static uint16_t iter;

    ledfxClear(0, 0);
    // walking LED (RGB)
    if (pkVal->pattern == 1)
    {
        uint8_t rd = pkVal->red, gr = pkVal->green, bl = pkVal->blue;
        sSwapRGB((ORDER_t)pkVal->order, &rd, &gr, &bl);
        for (uint16_t ix = iter % pkVal->nxy; ix < (uint16_t)pkVal->nxy; ix += pkVal->period)
        {
            ledfxSetIxRGB(ix, rd, gr, bl);
        }
        iter++;
    }
    // walking LED (HSV)
    else if (pkVal->pattern == 2)
    {
        uint8_t rd, gr, bl;
        hsv2rgb(pkVal->hue, pkVal->sat, pkVal->val, &rd, &gr, &bl);
        sSwapRGB((ORDER_t)pkVal->order, &rd, &gr, &bl);
        for (uint16_t ix = iter % pkVal->nxy; ix < (uint16_t)pkVal->nxy; ix += pkVal->period)
        {
            ledfxSetIxRGB(ix, rd, gr, bl);
        }
        iter++;
    }
    // rainbow
    else if (pkVal->pattern == 3)
    {
        const uint8_t dHue = 255 / pkVal->period;
        for (uint16_t ix = 0; ix < (uint16_t)pkVal->nxy; ix++)
        {
            const uint8_t hue = (uint8_t)iter + ((ix % pkVal->period) * dHue);
            uint8_t rd, gr, bl;
            hsv2rgb(hue, pkVal->sat, pkVal->val, &rd, &gr, &bl);
            sSwapRGB((ORDER_t)pkVal->order, &rd, &gr, &bl);
            ledfxSetIxRGB(ix, rd, gr, bl);
        }
        iter += dHue;
    }
}




// -------------------------------------------------------------------------------------------------

// static void sRunDemo(const STATE_t *pkState, const DEMO_t which)
// {
//     UNUSED(pkState);
//     PRINT("Demo %S", skDemoStrs[which]);
//     switch (which)
//     {
//         case DEMO_DISP:
//         {
//             const char *skDemoStr = PSTR("    !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_    ");
//             uint8_t n = 10;
//             while (n > 0)
//             {
//                 dl2416tClear();
//                 dl2416tUnsigned(n, 1, 2);
//                 dl2416tBlink(3, 100, 200);
//                 dl2416tStrScroll_P(skDemoStr, 300);
//                 n--;
//             }
//         }
//     }
// }


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    static const uint16_t melody1[] PROGMEM =
    {
        TONE_NOTE_E5, 80,  TONE_PAUSE, 40,  TONE_NOTE_E5, 80,  TONE_PAUSE, 40,  TONE_NOTE_B5, 50,
        TONE_END
    };
    toneMelody(melody1, true);

    // help and print menu
    NOTICE_W("Pins: 1 GND  2 VCC  3 NONE  4 CLK  5 MOSI  6 DATA");
    PRINT_W( "- WS2801: 1, 2, 4, 5");
    PRINT_W( "- WS2812: 1, 2, 6");
    sMenuDump(skMenu1, NUMOF(skMenu1));

    // clear event queue
    rotencClearEvents();

    // say hello
    dl2416tStrScroll_P(PSTR("    FLIPFLIP'S LED TESTER (C) 2018 FFI    "), 200);
    dl2416tClear();
    osTaskDelay(500);
    dl2416tStr_P(PSTR(":-)"), 1, 3);
    dl2416tBlink(5, 100, 150);

    static const uint16_t melody2[] PROGMEM =
    {
        TONE_NOTE_B5, 80,  TONE_PAUSE, 40,  TONE_NOTE_B5, 80,  TONE_PAUSE, 40,  TONE_NOTE_E5, 50,
        TONE_END
    };
    toneMelody(melody2, true);

    // initialise menu
    static MENU_STATE_t sMenu1State;
    static MENU1_VALUES_t sMenu1Values;
    sMenuInit(&sMenu1State, skMenu1, NUMOF(skMenu1), sMenu1Values.values);

    // clear event queue
    rotencClearEvents();

    NOTICE_W("Ready!");

    while (ENDLESS)
    {
        // calculate time for next LED update given the selected frame rate
        const uint32_t period = 1000 / sMenu1Values.hz;
        const uint32_t now = osTaskGetTicks();
        const uint32_t next = (now / period) * period + period;

        // listen for event while waiting for the next frame
        const ROTENC_EVENT_t event = now < next ? rotencGetEvent(next - now) : ROTENC_NONE;

        // handle event
        switch (event)
        {
            // no event, so it must be time for the next frame
            case ROTENC_NONE:
                break;

            // handle user activity
            case ROTENC_INC:
            case ROTENC_INC_DN:
            case ROTENC_DEC:
            case ROTENC_DEC_DN:
            case ROTENC_BTN:
            case ROTENC_BTN_LONG:
                sMenuHandle(&sMenu1State, event);
                // go wait more..
                continue;
                break;
        }

        if (event != ROTENC_NONE)
        {
            // special hex-dec conversion menu
            if ( (sMenu1State.pkCurr == &skMenu1[MENU1_HEX_IX]) || (sMenu1State.pkCurr == &skMenu1[MENU1_DEC_IX]) )
            {
                static int16_t oldHexDec;
                if (sMenu1Values.hex != oldHexDec)
                {
                    sMenu1Values.dec = sMenu1Values.hex;
                    oldHexDec = sMenu1Values.hex;
                }
                else if (sMenu1Values.dec != oldHexDec)
                {
                    sMenu1Values.hex = sMenu1Values.dec;
                    oldHexDec = sMenu1Values.dec;
                }
                DEBUG("%03"PRIi16" 0x%02"PRIx16, sMenu1Values.dec, sMenu1Values.hex);
            }
            // any other menu -> update parameters
            else
            {
                DEBUG("m:%"PRIi16" (%S) o:%"PRIi16" (%S) d:%"PRIi16" (%S) n:%02"PRIi16"x%02"PRIi16"/%03"PRIi16" r:%03"PRIi16" g:%03"PRIi16" b:%03"PRIi16" h:%03"PRIi16" s:%03"PRIi16" v:%03"PRIi16,
                    sMenu1Values.mode, skModeMenuStrs[sMenu1Values.mode],
                    sMenu1Values.order, skOrderMenuStrs[sMenu1Values.order],
                    sMenu1Values.driver, skDriverMenuStrs[sMenu1Values.driver],
                    sMenu1Values.nx, sMenu1Values.ny, sMenu1Values.nxy,
                    sMenu1Values.red, sMenu1Values.green, sMenu1Values.blue, sMenu1Values.hue, sMenu1Values.sat, sMenu1Values.val);
            }
        }

        // always update LEDs
        sUpdateLeds(&sMenu1Values);
    }

}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("ffledtester..."));
}

/* ***** application init **************************************************** */

// initialise the user application
void appInit(void)
{
    DEBUG("ffledtester: init");

    rotencInit();

    toneInit();

    ws2801Init();

    PIN_OUTPUT(WS2812_PIN); PIN_LOW(WS2812_PIN);

    dl2416tInit();

    ledfxClear(0, 0);
    ws2801Send(NULL, ledfxGetFrameBufferSize());
    ws2812Send(WS2812_PIN, NULL, ledfxGetFrameBufferSize());

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static OS_TASK_t task;
    static uint8_t stack[300];
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

//------------------------------------------------------------------------------

//@}
// eof
