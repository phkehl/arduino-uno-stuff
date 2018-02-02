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

#if 1

// -------------------------------------------------------------------------------------------------

// type of menu entry
typedef enum MENU_TYPE_e
{
    MENU_TYPE_STR,  // entry is list of strings to choose from
    MENU_TYPE_VAL,  // entry is value
    MENU_TYPE_MENU, // entry is a sub-menu

} MENU_TYPE_t;

// menu entries
typedef struct MENU_s
{
    uint8_t  uid;
    uint8_t  pid;

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
            int16_t min;          // - minimum value
            int16_t max;          // - maximum value
        };
    };
} MENU_t;

static const char skModeMenuStrs[][5] PROGMEM =
{
    { "MRGB\0" }, { "MHSV\0" }
};
static const char skOrderMenuStrs[][5] PROGMEM =
{
    { "ORGB\0" }, { "ORBG\0" }, { "OBRG\0" }, { "OBGR\0" }, { "OBRG\0" }, { "OBGR\0" }
};

static const MENU_t skMenu[] PROGMEM =
{
    { .uid =  1, .pid =  0, .type = MENU_TYPE_STR,  .name = "1 MO (Mode)\0",   .wrap = false, .strs = (const char **)skModeMenuStrs , .nStrs = NUMOF(skModeMenuStrs) },
    { .uid =  2, .pid =  0, .type = MENU_TYPE_STR,  .name = "2 OR (Order)\0",  .wrap = false, .strs = (const char **)skOrderMenuStrs , .nStrs = NUMOF(skOrderMenuStrs) },
    { .uid =  3, .pid =  0, .type = MENU_TYPE_VAL,  .name = "3 RD (red)\0",    .wrap = false, .min = 0, .max = 255 },
    { .uid =  4, .pid =  0, .type = MENU_TYPE_VAL,  .name = "4 GN (green)\0",  .wrap = false, .min = 0, .max = 255 },
    { .uid =  5, .pid =  0, .type = MENU_TYPE_VAL,  .name = "5 BL (blue)\0",   .wrap = false, .min = 0, .max = 255 },
    { .uid =  6, .pid =  0, .type = MENU_TYPE_VAL,  .name = "6 HU (hue)\0",    .wrap = true,  .min = 0, .max = 255 },
    { .uid =  7, .pid =  0, .type = MENU_TYPE_VAL,  .name = "7 SA (sat)\0",    .wrap = false, .min = 0, .max = 255 },
    { .uid =  8, .pid =  0, .type = MENU_TYPE_VAL,  .name = "8 VA (val)\0",    .wrap = false, .min = 0, .max = 255 },
    { .uid =  9, .pid =  0, .type = MENU_TYPE_MENU, .name = "9 MA (matrix)\0", .wrap = false },
    { .uid = 10, .pid =  0, .type = MENU_TYPE_MENU, .name = "A XX (test)\0",   .wrap = false },

    { .uid = 31, .pid =  9, .type = MENU_TYPE_VAL,  .name = "1 NX (n_x)\0",    .wrap = false, .min = 1, .max = 10 },
    { .uid = 32, .pid =  9, .type = MENU_TYPE_VAL,  .name = "2 NY (n_y)\0",    .wrap = false, .min = 1, .max = 10 },

    { .uid = 41, .pid = 10, .type = MENU_TYPE_VAL,  .name = "1 AA\0",          .wrap = false, .min = 1, .max = 10 },
    { .uid = 42, .pid = 10, .type = MENU_TYPE_VAL,  .name = "2 BB\0",          .wrap = false, .min = 1, .max = 10 },
    { .uid = 43, .pid = 10, .type = MENU_TYPE_MENU, .name = "3 CC\0",          .wrap = false },

    { .uid = 44, .pid = 43, .type = MENU_TYPE_VAL,  .name = "1 FOO\0",         .wrap = false, .min = 1, .max = 10 },
    { .uid = 45, .pid = 43, .type = MENU_TYPE_VAL,  .name = "2 BAR\0",         .wrap = false, .min = 1, .max = 10 },
};

#define MENU_UID(pkMenu)    ((uint8_t)pgm_read_byte(&((pkMenu)->uid)))
#define MENU_PID(pkMenu)    ((uint8_t)pgm_read_byte(&((pkMenu)->pid)))
#define MENU_TYPE(pkMenu)   ((MENU_TYPE_t)pgm_read_byte(&((pkMenu)->type)))
#define MENU_NAME(pkMenu)   ((pkMenu)->name)

typedef struct MENU_STATE_s
{
    const MENU_t *pkMenu ; // list of menu entries
    uint8_t       nMenu;  // number of menu entries
    const MENU_t *pkCurr; // current menu
    bool          active; // true if entry is active
    int16_t      *vals;   // storage for the values associated with each entry (must be big enough for nMenu values)
} MENU_STATE_t;

// jump to given menu entry
static void sMenuSet(MENU_STATE_t *pState, const MENU_t *pkMenu)
{
    if (pkMenu == NULL)
    {
        pState->pkCurr = pState->pkMenu;
    }
    else
    {
        pState->pkCurr = pkMenu;
    }
    DEBUG("menu: uid=%02"PRIu8" pid=%02"PRIu8" -> %S (%S)",
        MENU_UID(pState->pkCurr), MENU_PID(pState->pkCurr), MENU_NAME(pState->pkCurr),
        pState->active ? PSTR("active") : PSTR("menu"));
}

static void sMenuUpdate(MENU_STATE_t *pState, const ROTENC_EVENT_t event)
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
                        sMenuSet(pState, pkCand);
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
                        sMenuSet(pState, pkCand);
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
                        const uint8_t uid = MENU_UID(pState->pkCurr);
                        const MENU_t *pkCand = pState->pkMenu;
                        while (pkCand < &pState->pkMenu[pState->nMenu])
                        {
                            if (MENU_PID(pkCand) == uid)
                            {
                                sMenuSet(pState, pkCand);
                                break;
                            }
                            pkCand++;
                        }
                        // TODO: remember previous uid in vals
                        break;
                    }
                    // activate entry
                    case MENU_TYPE_VAL:
                    case MENU_TYPE_STR:
                        pState->active = true;
                        sMenuSet(pState, pState->pkCurr);
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
                    if (MENU_UID(pkCand) == pid)
                    {
                        sMenuSet(pState, pkCand);
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
        DEBUG("active");

    }
}

static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // clear event queue
    rotencClearEvents();


    // initialise menu
    static int16_t sMenuVals[NUMOF(skMenu)];
    static MENU_STATE_t sMenuState = { .pkMenu = skMenu, .nMenu = NUMOF(skMenu), .vals = sMenuVals };
    sMenuSet(&sMenuState, NULL);


    while (ENDLESS)
    {
        const ROTENC_EVENT_t ev = rotencGetEvent(1000);
        if (ev != ROTENC_NONE)
        {
            DEBUG("%S", rotencEventStr(ev));
        }
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
                sMenuUpdate(&sMenuState, ev);
                break;
                break;
        }
    }

}

/* ***** application functions *********************************************** */

#else
typedef enum MENU_e
{
    CHOOSE,
    SET_MODE,
    SET_ORDER,
    SET_RED,
    SET_GREEN,
    SET_BLUE,
    SET_HUE,
    SET_SAT,
    SET_VAL,

    DEMO,

} MENU_t;

static const char skMenuStrs[][10] PROGMEM =
{
    { "CHOOSE\0" }, { "SET_MODE\0" }, { "SET_ORDER\0" },
    { "SET_RED\0" }, { "SET_GREEN\0" }, { "SET_BLUE\0" },
    { "SET_HUE\0" }, { "SET_SAT\0" }, { "SET_VAL\0" },
    { "DEMO\0" },
};

static const MENU_t skMenuChoices[] PROGMEM =
{
    SET_MODE, SET_ORDER, SET_RED, SET_GREEN, SET_BLUE, SET_HUE, SET_SAT, SET_VAL, DEMO
};

typedef enum MODE_e
{
    MODE_RGB,
    MODE_HSV,
} MODE_t;

static const MODE_t skModeChoices[] PROGMEM =
{
    MODE_RGB, MODE_HSV
};

typedef enum ORDER_e
{
    ORDER_RGB,
    ORDER_RBG,
    ORDER_GRB,
    ORDER_GBR,
    ORDER_BRG,
    ORDER_BGR,
} ORDER_t;

static const ORDER_t skOrderChoices[] PROGMEM =
{
    ORDER_RGB, ORDER_RBG, ORDER_GRB, ORDER_GBR, ORDER_BRG, ORDER_BGR
};

typedef enum DEMO_e
{
    DEMO_DISP,
} DEMO_t;

static const DEMO_t skDemoChoices[] PROGMEM =
{
    DEMO_DISP
};

static const char skDemoStrs[][10] PROGMEM =
{
    { "DEMO_DISP\0" }
};


typedef struct STATE_s
{
    MENU_t  menu;

    uint8_t choice; // menu = CHOOSE

    uint8_t mode;   // menu = SET_MODE

    uint8_t order;  // menu = SET_ORDER

    uint8_t demo;   // menu = DEMO

    uint8_t red;    // menu = SET_RED
    uint8_t green;  // menu = SET_GREEN
    uint8_t blue;   // menu = SET_BLUE

    uint8_t hue;    // menu = SET_RED
    uint8_t sat;    // menu = SET_SAT
    uint8_t val;    // menu = SET_VAL

} STATE_t;

static void sInitState(STATE_t *pState)
{
    DEBUG("init state");
    memset(pState, 0, sizeof(*pState));
    pState->menu = CHOOSE;
    pState->mode = MODE_HSV;
    pState->order = ORDER_RGB;
    pState->sat = 255;
    dl2416tStr_P(PSTR("****"), 0, 0);
    dl2416tBlink(3, 100, 200);
    dl2416tClear();
}

static void sUpdateDisplay(const STATE_t *pkState)
{
    switch (pkState->menu)
    {
        case CHOOSE:
            switch ((MENU_t)pgm_read_byte(&skMenuChoices[ pkState->choice ]))
            {
                case SET_MODE:  dl2416tStr_P(PSTR("1 MO"), 0, 0); break;
                case SET_ORDER: dl2416tStr_P(PSTR("2 OR"), 0, 0); break;
                case SET_RED:   dl2416tStr_P(PSTR("3 RD"), 0, 0); break;
                case SET_GREEN: dl2416tStr_P(PSTR("4 GN"), 0, 0); break;
                case SET_BLUE:  dl2416tStr_P(PSTR("5 BL"), 0, 0); break;
                case SET_HUE:   dl2416tStr_P(PSTR("6 HU"), 0, 0); break;
                case SET_SAT:   dl2416tStr_P(PSTR("7 SA"), 0, 0); break;
                case SET_VAL:   dl2416tStr_P(PSTR("8 VA"), 0, 0); break;
                case DEMO:      dl2416tStr_P(PSTR("DEMO"), 0, 0); break;
                default:        dl2416tStr_P(PSTR("????"), 0, 0); break;
            }
            break;
        case SET_MODE:
            dl2416tWrite(0, 'M');
            switch ((MODE_t)pgm_read_byte(&skModeChoices[ pkState->mode ]))
            {
                case MODE_RGB:  dl2416tStr_P(PSTR("RGB"), 1, 0); break;
                case MODE_HSV:  dl2416tStr_P(PSTR("HSV"), 1, 0); break;
            }
            break;
        case SET_ORDER:
            dl2416tWrite(0, 'O');
            switch ((ORDER_t)pgm_read_byte(&skOrderChoices[ pkState->order ]))
            {
                case ORDER_RGB:  dl2416tStr_P(PSTR("RGB"), 1, 0); break;
                case ORDER_RBG:  dl2416tStr_P(PSTR("RBG"), 1, 0); break;
                case ORDER_GRB:  dl2416tStr_P(PSTR("GRB"), 1, 0); break;
                case ORDER_GBR:  dl2416tStr_P(PSTR("GBR"), 1, 0); break;
                case ORDER_BRG:  dl2416tStr_P(PSTR("BRG"), 1, 0); break;
                case ORDER_BGR:  dl2416tStr_P(PSTR("BGR"), 1, 0); break;
            }
            break;
        case DEMO:
            switch ((DEMO_t)pgm_read_byte(&skDemoChoices[ pkState->demo ]))
            {
                case DEMO_DISP:  dl2416tStr_P(PSTR("DISP"), 0, 0); break;
            }
            break;
        case SET_RED:   dl2416tWrite(0, 'R'); dl2416tUnsigned(pkState->red,   0, 3); break;
        case SET_GREEN: dl2416tWrite(0, 'G'); dl2416tUnsigned(pkState->green, 0, 3); break;
        case SET_BLUE:  dl2416tWrite(0, 'B'); dl2416tUnsigned(pkState->blue,  0, 3); break;
        case SET_HUE:   dl2416tWrite(0, 'H'); dl2416tUnsigned(pkState->hue,   0, 3); break;
        case SET_SAT:   dl2416tWrite(0, 'S'); dl2416tUnsigned(pkState->sat,   0, 3); break;
        case SET_VAL:   dl2416tWrite(0, 'V'); dl2416tUnsigned(pkState->val,   0, 3); break;
    }
}

static int16_t sUpdateMenu(STATE_t *pState, const int16_t inVal)
{
    int16_t val = inVal;
    switch (pState->menu)
    {
        case CHOOSE:
            val = CLIP(val, 0, (int16_t)NUMOF(skMenuChoices) - 1);
            pState->choice = val;
            break;
        case SET_MODE:
            val = CLIP(val, 0, (int16_t)NUMOF(skModeChoices) - 1);
            pState->mode = val;
            break;
        case SET_ORDER:
            val = CLIP(val, 0, (int16_t)NUMOF(skOrderChoices) - 1);
            pState->order = val;
            break;
        case DEMO:
            val = CLIP(val, 0, (int16_t)NUMOF(skDemoChoices) - 1);
            pState->demo = val;
            break;
        case SET_RED:   val = CLIP(val, 0, 255); pState->red   = val; break;
        case SET_GREEN: val = CLIP(val, 0, 255); pState->green = val; break;
        case SET_BLUE:  val = CLIP(val, 0, 255); pState->blue  = val; break;
        case SET_SAT:   val = CLIP(val, 0, 255); pState->sat   = val; break;
        case SET_VAL:   val = CLIP(val, 0, 255); pState->val   = val; break;
        case SET_HUE:
            while (val < 0)
            {
                val += 256;
            }
            while (val > 255)
            {
                val -= 256;
            }
            pState->hue   = val;
            break;
    }

    DEBUG("upd %S %"PRIi16" -> %"PRIi16, skMenuStrs[pState->menu], inVal, val);
    return val;
}

// forward declaration
static void sRunDemo(const STATE_t *pkState, const DEMO_t which);

// short press
static int16_t sApplyMenu(STATE_t *pState)
{
    int16_t val = 0;
    switch (pState->menu)
    {
        case CHOOSE:
            switch ((MENU_t)pgm_read_byte(&skMenuChoices[pState->choice]))
            {
                case CHOOSE:
                case SET_MODE:  pState->menu = SET_MODE;  val = pState->mode;  break;
                case SET_ORDER: pState->menu = SET_ORDER; val = pState->order; break;
                case SET_RED:   pState->menu = SET_RED;   val = pState->red;   break;
                case SET_GREEN: pState->menu = SET_GREEN; val = pState->green; break;
                case SET_BLUE:  pState->menu = SET_BLUE;  val = pState->blue;  break;
                case SET_HUE:   pState->menu = SET_HUE;   val = pState->hue;   break;
                case SET_SAT:   pState->menu = SET_SAT;   val = pState->sat;   break;
                case SET_VAL:   pState->menu = SET_VAL;   val = pState->val;   break;
                case DEMO:      pState->menu = DEMO;      val = pState->demo;  break;
            }
            break;
        default:
            pState->menu = CHOOSE;
            val = pState->choice;
            break;
    }
    DEBUG("menu %S %"PRIi16, skMenuStrs[pState->menu], val);
    return val;
}

// long press
static int16_t sApplyValue(STATE_t *pState, const uint8_t inVal)
{
    int16_t val = inVal;
    switch (pState->menu)
    {
        case CHOOSE:
            sInitState(pState);
            val = 0;
            break;
        case SET_MODE:
            // nothing
            break;
        case SET_ORDER:
            // nothing
            break;
        case DEMO:
            sRunDemo(pState, (DEMO_t)pState->demo);
            pState->menu = CHOOSE;
            val = pState->choice;
            rotencClearEvents();
            break;
        case SET_HUE:
            val += 255/6;
            break;
        case SET_RED:
        case SET_GREEN:
        case SET_BLUE:
        case SET_SAT:
        case SET_VAL:
            if      (val ==   0) { val = 128; }
            else if (val == 128) { val = 255; }
            else if (val == 255) { val = 127; }
            else if (val  < 128) { val =   0; }
            else     /* > 128 */ { val = 255; }
            break;
    }
    DEBUG("value %S %"PRIi16, skMenuStrs[pState->menu], val);
    return val;
}

static void sUpdateLeds(const STATE_t *pkState)
{
    uint8_t rd, gr, bl;
    switch ((MODE_t)pgm_read_byte(&skModeChoices[ pkState->mode ]))
    {
        case MODE_RGB:
            rd = pkState->red;
            gr = pkState->green;
            bl = pkState->blue;
            break;
        case MODE_HSV:
            hsv2rgb(pkState->hue, pkState->sat, pkState->val, &rd, &gr, &bl);
            break;
    }
    uint8_t ch1, ch2, ch3;
    switch ((ORDER_t)pgm_read_byte(&skOrderChoices[ pkState->order ]))
    {
        case ORDER_RBG: ch1 = rd; ch2 = bl; ch3 = gr; break;
        case ORDER_GRB: ch1 = gr; ch2 = rd; ch3 = bl; break;
        case ORDER_GBR: ch1 = gr; ch2 = bl; ch3 = rd; break;
        case ORDER_BRG: ch1 = bl; ch2 = rd; ch3 = gr; break;
        case ORDER_BGR: ch1 = bl; ch2 = gr; ch3 = rd; break;
        default:
        case ORDER_RGB: ch1 = rd; ch2 = gr; ch3 = bl; break;
    }
    ledfxFillRGB(0, 0, ch1, ch2, ch3);
    ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
}

static void sRunDemo(const STATE_t *pkState, const DEMO_t which)
{
    UNUSED(pkState);
    PRINT("Demo %S", skDemoStrs[which]);
    switch (which)
    {
        case DEMO_DISP:
        {
            const char *skDemoStr = PSTR("    !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_    ");
            uint8_t n = 10;
            while (n > 0)
            {
                dl2416tClear();
                dl2416tUnsigned(n, 1, 2);
                dl2416tBlink(3, 100, 200);
                dl2416tStrScroll_P(skDemoStr, 300);
                n--;
            }
        }
    }
}


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

    static STATE_t sState;
    static int16_t val;
    sInitState(&sState);
    val = sUpdateMenu(&sState, 0);
    sUpdateDisplay(&sState);
    sUpdateLeds(&sState);

    while (ENDLESS)
    {
        const ROTENC_EVENT_t ev = rotencGetEvent(1000);
        switch (ev)
        {
            case ROTENC_INC_DN: val += 10; break;
            case ROTENC_INC:    val++;     break;
            case ROTENC_DEC_DN: val -= 10; break;
            case ROTENC_DEC:    val--;     break;
            case ROTENC_BTN:
                val = sApplyMenu(&sState);
                break;
            case ROTENC_BTN_LONG:
                val = sApplyValue(&sState, val);
                break;
            case ROTENC_NONE:
                //DEBUG("NONE");
                break;
        }
        val = sUpdateMenu(&sState, val);
        sUpdateDisplay(&sState);
        sUpdateLeds(&sState);
    }
}

#endif

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
