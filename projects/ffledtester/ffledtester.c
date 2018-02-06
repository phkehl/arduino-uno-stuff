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
    };
} MENU_t;

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

typedef struct MENU_STATE_s
{
    const __flash MENU_t *pkMenu; // list of menu entries
    uint8_t               nMenu;  // number of menu entries
    const __flash MENU_t *pkCurr; // current menu
    bool                  active; // true if entry is active
    int16_t              *vals;   // storage for the values associated with each entry (must be big enough for nMenu values)
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
            case MENU_TYPE_MENU:
            case MENU_TYPE_JUMP:
                break;
        }
    }
    sMenuUpdate(pState, NULL);
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
    DEBUG("menu: mid=%02"PRIu8" pid=%02"PRIu8" ix=%02"PRIu8" -> %S (%S) %"PRIi16,
        MENU_MID(pState->pkCurr), MENU_PID(pState->pkCurr), MENU_IX(pState->pkMenu, pState->pkCurr),
        MENU_NAME(pState->pkCurr), pState->active ? PSTR("active") : PSTR("menu"),
        pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ]);
    if (pState->active)
    {
        switch (MENU_TYPE(pState->pkCurr))
        {
            case MENU_TYPE_VAL:
            {
                const int16_t min = MENU_MIN(pState->pkCurr);
                int16_t val = pState->vals[ MENU_IX(pState->pkMenu, pState->pkCurr) ];
                dl2416tWrite(0, MENU_IND(pState->pkCurr));
                if (min < 0)
                {
                    dl2416tWrite(1, val < 0 ? '-' : '+');
                    dl2416tUnsigned(ABS(val), 2, 2);
                }
                else
                {
                    dl2416tUnsigned(val, 1, 3);
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
                }
                break;
            // leave sub-menu
            case ROTENC_BTN_LONG:
            {
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
                        pState->active = false;
                        break;
                    // reset to default
                    case ROTENC_BTN_LONG:
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
const __flash char * const __flash skModeMenuStrs[] =
{
    skModeMenuStr0, skModeMenuStr1
};
typedef enum MODE_e { MODE_RGB = 0, MODE_HSV = 1 } MODE_t;

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


#define TEST_MENU 1

//menu structure
static const MENU_t skMenu1[] PROGMEM =
{
    // main menu
    { .mid =  1, .pid =  0, .type = MENU_TYPE_STR,  .name = "1 MO (Mode)\0",   .wrap = false, .def = MODE_RGB, .strs = skModeMenuStrs, .nStrs = NUMOF(skModeMenuStrs) },
    { .mid =  2, .pid =  0, .type = MENU_TYPE_STR,  .name = "2 OR (Order)\0",  .wrap = false, .def = ORDER_RGB, .strs = skOrderMenuStrs, .nStrs = NUMOF(skOrderMenuStrs) },
    { .mid =  3, .pid =  0, .type = MENU_TYPE_VAL,  .name = "3 RD (red)\0",    .wrap = false, .ind = 'R', .min = 0, .max = 255, .def =   1 },
    { .mid =  4, .pid =  0, .type = MENU_TYPE_VAL,  .name = "4 GN (green)\0",  .wrap = false, .ind = 'G', .min = 0, .max = 255, .def =   1 },
    { .mid =  5, .pid =  0, .type = MENU_TYPE_VAL,  .name = "5 BL (blue)\0",   .wrap = false, .ind = 'B', .min = 0, .max = 255, .def =   1 },
    { .mid =  6, .pid =  0, .type = MENU_TYPE_VAL,  .name = "6 HU (hue)\0",    .wrap = true,  .ind = 'H', .min = 0, .max = 255, .def =   0 },
    { .mid =  7, .pid =  0, .type = MENU_TYPE_VAL,  .name = "7 SA (sat)\0",    .wrap = false, .ind = 'S', .min = 0, .max = 255, .def = 255 },
    { .mid =  8, .pid =  0, .type = MENU_TYPE_VAL,  .name = "8 VA (val)\0",    .wrap = false, .ind = 'V', .min = 0, .max = 255, .def =   1 },
    { .mid =  9, .pid =  0, .type = MENU_TYPE_MENU, .name = "9 MA (matrix)\0" },
    { .mid = 10, .pid =  0, .type = MENU_TYPE_MENU, .name = "A HD (hex-dec)\0" },
    { .mid = 11, .pid =  0, .type = MENU_TYPE_STR,  .name = "B CH (chars)\0",  .wrap = true, .def = 0, .strs = skCharsMenuStrs, .nStrs = NUMOF(skCharsMenuStrs) },
#if (TEST_MENU > 0)
    { .mid = 12, .pid =  0, .type = MENU_TYPE_MENU, .name = "X XX (test)\0" },
#endif

    // matrix menu
    { .mid = 13, .pid =  9, .type = MENU_TYPE_VAL,  .name = "1 NX (n_x)\0",    .wrap = false, .ind = 'X', .min = 1, .max = 10, .def = 8 },
    { .mid = 14, .pid =  9, .type = MENU_TYPE_VAL,  .name = "2 NY (n_y)\0",    .wrap = false, .ind = 'Y', .min = 1, .max = 10, .def = 8 },
    { .mid = 15, .pid =  9, .type = MENU_TYPE_VAL,  .name = "2 XY (total)\0",  .wrap = false, .ind = '#', .min = 0, .max = FF_LEDFX_NUM_LED, .def = 5 },
    { .mid = 16, .pid =  9, .type = MENU_TYPE_JUMP, .name = "X (-\0",          .wrap = false, .jump = 9 },

    // hex-dec menu
#define HEXDEC_MENU_IX 16
    { .mid = 17, .pid = 10, .type = MENU_TYPE_HEX,  .name = "1HEX (hex)\0",    .wrap = true,  .ind = 'X', .min = 0, .max = 255, .def = 0 },
    { .mid = 18, .pid = 10, .type = MENU_TYPE_VAL,  .name = "2DEC (dec)\0",    .wrap = true,  .ind = 'D', .min = 0, .max = 255, .def = 0 },
    { .mid = 19, .pid = 10, .type = MENU_TYPE_JUMP, .name = "X (-\0",          .wrap = false, .jump = 10 },

    // test menu
#if (TEST_MENU > 0)
    { .mid = 91, .pid = 12, .type = MENU_TYPE_VAL,  .name = "1 AA\0",          .wrap = true,  .ind = 'A', .min = 3, .max = 45, .def = 0 },
    { .mid = 92, .pid = 12, .type = MENU_TYPE_VAL,  .name = "2 BB\0",          .wrap = false, .ind = 'B', .min = -8, .max = +8, .def = 0 },
    { .mid = 93, .pid = 12, .type = MENU_TYPE_MENU, .name = "3 CC\0" },
    { .mid = 94, .pid = 12, .type = MENU_TYPE_JUMP, .name = "X (-\0",          .wrap = false, .jump = 10 },
    // test sub-menu
    { .mid = 95, .pid = 93, .type = MENU_TYPE_VAL,  .name = "1 FOO\0",         .wrap = false, .ind = 'F', .min = 1, .max = 10, .def = 0 },
    { .mid = 96, .pid = 93, .type = MENU_TYPE_VAL,  .name = "2 BAR\0",         .wrap = false, .ind = 'B', .min = 1, .max = 10, .def = 0 },
#endif // (TEST_MENU > 0)

};

// storage for menu values
typedef union MENU1_VALUES_u
{
    // this many values
    int16_t values[NUMOF(skMenu1)];
    // and direct, named access (offset must be in sync with the above list)
    struct
    {
        int16_t mode;   //  1
        int16_t order;  //  2
        int16_t red;    //  3
        int16_t green;  //  4
        int16_t blue;   //  5
        int16_t hue;    //  6
        int16_t sat;    //  7
        int16_t val;    //  8
        int16_t _pad0;  //  9
        int16_t _pad1;  // 10
        int16_t _pad2;  // 11
#if (TEST_MENU > 0)
        int16_t _pad3;  // 12
#endif // (TEST_MENU > 0)
        int16_t nx;     // 13
        int16_t ny;     // 14
        int16_t nxy;    // 15
        int16_t _pad4;  // 16
        int16_t hex;    // 17
        int16_t dec;    // 18
    };

} MENU1_VALUES_t;
#define STRIP_PIN _D7

// -------------------------------------------------------------------------------------------------

static void sUpdateLeds(const MENU1_VALUES_t *pkVal)
{
    uint8_t rd, gr, bl;
    switch ((MODE_t)pkVal->mode)
    {
        case MODE_RGB:
            rd = pkVal->red;
            gr = pkVal->green;
            bl = pkVal->blue;
            break;
        case MODE_HSV:
            hsv2rgb(pkVal->hue, pkVal->sat, pkVal->val, &rd, &gr, &bl);
            break;
    }
    uint8_t ch1, ch2, ch3;
    switch ((ORDER_t)pkVal->order)
    {
        case ORDER_RBG: ch1 = rd; ch2 = bl; ch3 = gr; break;
        case ORDER_GRB: ch1 = gr; ch2 = rd; ch3 = bl; break;
        case ORDER_GBR: ch1 = gr; ch2 = bl; ch3 = rd; break;
        case ORDER_BRG: ch1 = bl; ch2 = rd; ch3 = gr; break;
        case ORDER_BGR: ch1 = bl; ch2 = gr; ch3 = rd; break;
        default:
        case ORDER_RGB: ch1 = rd; ch2 = gr; ch3 = bl; break;
    }
    ledfxClear(0, 0);
    for (int16_t ix = 0; ix < pkVal->nxy; ix++)
    {
        ledfxSetIxRGB(ix, ch1, ch2, ch3);
    }
    ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
    ws2812Send(WS2812_PIN, ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
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

    // clear event queue
    rotencClearEvents();

    // say hello
    dl2416tStrScroll_P(PSTR("    FLIPFLIP'S LED TESTER (C) 2018 FFI    "), 200);
    dl2416tClear();
    osTaskDelay(500);
    dl2416tStr_P(PSTR(":-)"), 1, 3);
    dl2416tBlink(5, 150, 250);


    // clear event queue
    rotencClearEvents();

    // initialise menu
    static MENU_STATE_t sMenu1State;
    static MENU1_VALUES_t sMenu1Values;
    sMenuInit(&sMenu1State, skMenu1, NUMOF(skMenu1), sMenu1Values.values);

    while (ENDLESS)
    {
        const ROTENC_EVENT_t ev = rotencGetEvent(1000);
        //if (ev != ROTENC_NONE) { DEBUG("%S", rotencEventStr(ev));  }
        switch (ev)
        {
            case ROTENC_NONE:
                break;
            case ROTENC_INC:
            case ROTENC_INC_DN:
            case ROTENC_DEC:
            case ROTENC_DEC_DN:
            case ROTENC_BTN:
            case ROTENC_BTN_LONG:
                sMenuHandle(&sMenu1State, ev);
                break;
                break;
        }

        // special hex-dec conversion menu
        if ( (sMenu1State.pkCurr == &skMenu1[HEXDEC_MENU_IX]) || (sMenu1State.pkCurr == &skMenu1[HEXDEC_MENU_IX + 1]) )
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
        // any other menu -> update LEDs
        else
        {
            hwTic(0);
            sUpdateLeds(&sMenu1Values);
            const uint16_t dt = hwToc(0);
            DEBUG("%03"PRIu16"us m:%"PRIi16" (%S) o:%"PRIi16" (%S) n:%02"PRIi16"x%02"PRIi16"/%03"PRIi16" r:%03"PRIi16" g:%03"PRIi16" b:%03"PRIi16" h:%03"PRIi16" s:%03"PRIi16" v:%03"PRIi16,
                dt, sMenu1Values.mode, skModeMenuStrs[sMenu1Values.mode],
                sMenu1Values.order, skOrderMenuStrs[sMenu1Values.order],
                sMenu1Values.nx, sMenu1Values.ny, sMenu1Values.nxy,
                sMenu1Values.red, sMenu1Values.green, sMenu1Values.blue, sMenu1Values.hue, sMenu1Values.sat, sMenu1Values.val);
        }

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

    ws2801Init();

    PIN_OUTPUT(WS2812_PIN); PIN_LOW(WS2812_PIN);

    dl2416tInit();

    ledfxClear(0, 0);
    ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
    ws2812Send(WS2812_PIN, ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static OS_TASK_t task;
    static uint8_t stack[250];
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

//------------------------------------------------------------------------------

//@}
// eof
