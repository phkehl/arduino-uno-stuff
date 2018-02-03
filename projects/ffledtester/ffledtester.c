/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's LED tester (see \ref PROJECTS_FFLEDTESTER)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER

    \ref ARDUINOPINS usage:
    - D0: \ref ROTENC pin 2
    - D1: serial tx for \ref DEBUG output
    - D2: \ref ROTENC pin 1 ("CLK")
    - D3: \ref ROTENC pin 3 ("SW")
    - D4: Siemens DL2416T display: data input (D0)
    - D5: Siemens DL2416T display: data input (D1)
    - D6: Siemens DL2416T display: data input (D2)
    - D7: Siemens DL2416T display: data input (D3)
    - D8: Siemens DL2416T display: display blank (~BL)
    - D9: \ref FF_HW_LOAD_PIN
    - D10 (SS): needed by \ref WS2801 (FIXME: really?)
    - D11 (MOSI): \ref WS2801 data ("DI")
    - D12: \ref WS2812 data pin
    - D13 (SCK): \ref WS2801 clock output ("CI")
    - A0: Siemens DL2416T display: data input (D4)
    - A1: Siemens DL2416T display: data input (D5)
    - A2: Siemens DL2416T display: data input (D6)
    - A3: Siemens DL2416T display: address (A0)
    - A4: Siemens DL2416T display: address (A1)
    - A5: Siemens DL2416T display: write (~WR)

    Yay, all used! See also config.h.

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
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects
#include "hsv2rgb.h"       // ff: HSV to RGV conversion
#include "rotenc.h"        // ff: rotary encoder input

#include "ffledtester.h"
#include "dl2416t.h"


/* ***** application functions *********************************************** */

// type of menu entry
typedef enum MENU_TYPE_e
{
    MENU_TYPE_STR,  // entry is list of strings to choose from
    MENU_TYPE_VAL,  // entry is value
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
    bool        wrap;         // wrap at beginning/end of list/value
    union
    {
        struct
        {                         // type = MENU_TYPE_STR:
            const char **strs;    // - strings to choose from
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
#define MENU_MIN(pkMenu)         ((int16_t)pgm_read_word(&((pkMenu)->min)))
#define MENU_MAX(pkMenu)         ((int16_t)pgm_read_word(&((pkMenu)->max)))
#define MENU_IND(pkMenu)         ((char)pgm_read_byte(&((pkMenu)->ind)))
#define MENU_DEF(pkMenu)         ((int16_t)pgm_read_byte(&((pkMenu)->def)))
#define MENU_JUMP(pkMenu)        ((uint8_t)pgm_read_byte(&((pkMenu)->jump)))

static const char skModeMenuStrs[][5] PROGMEM =
{
    { "MRGB\0" }, { "MHSV\0" }
};
static const char skOrderMenuStrs[][5] PROGMEM =
{
    { "ORGB\0" }, { "ORBG\0" }, { "OBRG\0" }, { "OBGR\0" }, { "OBRG\0" }, { "OBGR\0" }
};

// menu structure
static const MENU_t skMenu[] PROGMEM =
{
    // main menu
    { .mid =  1, .pid =  0, .type = MENU_TYPE_STR,  .name = "1 MO (Mode)\0",   .wrap = false, .strs = (const char **)skModeMenuStrs , .nStrs = NUMOF(skModeMenuStrs) },
    { .mid =  2, .pid =  0, .type = MENU_TYPE_STR,  .name = "2 OR (Order)\0",  .wrap = false, .strs = (const char **)skOrderMenuStrs , .nStrs = NUMOF(skOrderMenuStrs) },
    { .mid =  3, .pid =  0, .type = MENU_TYPE_VAL,  .name = "3 RD (red)\0",    .wrap = false, .ind = 'R', .min = 0, .max = 255, .def =   0 },
    { .mid =  4, .pid =  0, .type = MENU_TYPE_VAL,  .name = "4 GN (green)\0",  .wrap = false, .ind = 'G', .min = 0, .max = 255, .def =   0 },
    { .mid =  5, .pid =  0, .type = MENU_TYPE_VAL,  .name = "5 BL (blue)\0",   .wrap = false, .ind = 'B', .min = 0, .max = 255, .def =   0 },
    { .mid =  6, .pid =  0, .type = MENU_TYPE_VAL,  .name = "6 HU (hue)\0",    .wrap = true,  .ind = 'H', .min = 0, .max = 255, .def =   0 },
    { .mid =  7, .pid =  0, .type = MENU_TYPE_VAL,  .name = "7 SA (sat)\0",    .wrap = false, .ind = 'S', .min = 0, .max = 255, .def = 255 },
    { .mid =  8, .pid =  0, .type = MENU_TYPE_VAL,  .name = "8 VA (val)\0",    .wrap = false, .ind = 'V', .min = 0, .max = 255, .def =   0 },
    { .mid =  9, .pid =  0, .type = MENU_TYPE_MENU, .name = "9 MA (matrix)\0", .wrap = false },
    { .mid = 10, .pid =  0, .type = MENU_TYPE_MENU, .name = "A XX (test)\0",   .wrap = false },

    // matrix menu
    { .mid = 31, .pid =  9, .type = MENU_TYPE_VAL,  .name = "1 NX (n_x)\0",    .wrap = false, .ind = 'X', .min = 1, .max = 10, .def = 0 },
    { .mid = 32, .pid =  9, .type = MENU_TYPE_VAL,  .name = "2 NY (n_y)\0",    .wrap = false, .ind = 'Y', .min = 1, .max = 10, .def = 0 },
    { .mid = 33, .pid =  9, .type = MENU_TYPE_VAL,  .name = "2 XY (total)\0",  .wrap = false, .ind = '#', .min = 1, .max = 100, .def = 0 },

    // test menu
    { .mid = 41, .pid = 10, .type = MENU_TYPE_VAL,  .name = "1 AA\0",          .wrap = true,  .ind = 'A', .min = 3, .max = 45, .def = 0 },
    { .mid = 42, .pid = 10, .type = MENU_TYPE_VAL,  .name = "2 BB\0",          .wrap = false, .ind = 'B', .min = -8, .max = +8, .def = 0 },
    { .mid = 43, .pid = 10, .type = MENU_TYPE_MENU, .name = "3 CC\0",          .wrap = false },
    { .mid = 44, .pid = 10, .type = MENU_TYPE_JUMP, .name = "X <-\0",          .wrap = false, .jump = 10 },
    // test sub-menu
    { .mid = 51, .pid = 43, .type = MENU_TYPE_VAL,  .name = "1 FOO\0",         .wrap = false, .ind = 'F', .min = 1, .max = 10, .def = 0 },
    { .mid = 52, .pid = 43, .type = MENU_TYPE_VAL,  .name = "2 BAR\0",         .wrap = false, .ind = 'B', .min = 1, .max = 10, .def = 0 },
};

typedef struct MENU_STATE_s
{
    const MENU_t *pkMenu; // list of menu entries
    uint8_t       nMenu;  // number of menu entries
    const MENU_t *pkCurr; // current menu
    bool          active; // true if entry is active
    int16_t      *vals;   // storage for the values associated with each entry (must be big enough for nMenu values)
} MENU_STATE_t;


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
                //const int16_t max = MENU_MAX(pState->pkCurr);
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
            case MENU_TYPE_STR:
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

// initialise menu state and values
static void sMenuInit(MENU_STATE_t *pState, const MENU_t *pkMenu, const uint8_t nMenu, int16_t *pVals)
{
    memset(pState, 0, sizeof(*pState));
    memset(pVals, 0, nMenu * sizeof(*pState));
    pState->pkMenu = pkMenu;
    pState->nMenu  = nMenu;
    pState->vals   = pVals;
    for (uint8_t ix = 0; ix < nMenu; ix++)
    {
        switch (MENU_TYPE(&pkMenu[ix]))
        {
            case MENU_TYPE_VAL:
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
                break;

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

// -------------------------------------------------------------------------------------------------

// static void sUpdateLeds(const STATE_t *pkState)
// {
//     uint8_t rd, gr, bl;
//     switch ((MODE_t)pgm_read_byte(&skModeChoices[ pkState->mode ]))
//     {
//         case MODE_RGB:
//             rd = pkState->red;
//             gr = pkState->green;
//             bl = pkState->blue;
//             break;
//         case MODE_HSV:
//             hsv2rgb(pkState->hue, pkState->sat, pkState->val, &rd, &gr, &bl);
//             break;
//     }
//     uint8_t ch1, ch2, ch3;
//     switch ((ORDER_t)pgm_read_byte(&skOrderChoices[ pkState->order ]))
//     {
//         case ORDER_RBG: ch1 = rd; ch2 = bl; ch3 = gr; break;
//         case ORDER_GRB: ch1 = gr; ch2 = rd; ch3 = bl; break;
//         case ORDER_GBR: ch1 = gr; ch2 = bl; ch3 = rd; break;
//         case ORDER_BRG: ch1 = bl; ch2 = rd; ch3 = gr; break;
//         case ORDER_BGR: ch1 = bl; ch2 = gr; ch3 = rd; break;
//         default:
//         case ORDER_RGB: ch1 = rd; ch2 = gr; ch3 = bl; break;
//     }
//     ledfxFillRGB(0, 0, ch1, ch2, ch3);
//     ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
// }

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
    osTaskDelay(500);

    // clear event queue
    dl2416tClear();
    rotencClearEvents();

    // initialise menu
    static int16_t sMenuVals[NUMOF(skMenu)];
    static MENU_STATE_t sMenuState;
    sMenuInit(&sMenuState, skMenu, NUMOF(skMenu), sMenuVals);


    while (ENDLESS)
    {
        const ROTENC_EVENT_t ev = rotencGetEvent(1000);
        if (ev != ROTENC_NONE) { DEBUG("%S", rotencEventStr(ev));  }
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
                sMenuHandle(&sMenuState, ev);
                break;
                break;
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

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

//------------------------------------------------------------------------------

//@}
// eof
