/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip phone (see \ref PROJECTS_FFPHONE)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFPHONE
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "hd44780.h"       // ff: HD44780 LCD driver

#include "arf32.h"
#include "ag1170.h"
#include "ffphone.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);
static void sStatusInit(void);

// initialise the user application
void appInit(void)
{
    DEBUG("ffphone: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    arf32Init();
    ag1170Init();

    arf32Start();
    ag1170Start();

    sStatusInit();
}

// starts the user application tasks
void appCreateTask(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);

}

/* ***** application state *************************************************** */




/* ***** status display ****************************************************** */

#define STATUS_CHAR_HEART_FULL    0x00
#define STATUS_CHAR_HEART_SMALL   0x01
#define STATUS_CHAR_MOBILE_READY  0x02
#define STATUS_CHAR_MOBILE_PAIRED 0x03
#define STATUS_CHAR_PHONE_ONHOOK  0x04
#define STATUS_CHAR_PHONE_OFFHOOK 0x05

static void sStatusInit(void)
{
    hd44780Init();

    static const uint8_t skHeart1[8] PROGMEM  = // 5x8
    {
        0b00000, // .....
        0b01010, // .#.#.
        0b11111, // #####
        0b11111, // #####
        0b01110, // .###.
        0b00100, // ..#..
        0b00000, // .....
    };
    static const uint8_t skHeart2[8] PROGMEM  = // 5x8
    {
        0b00000, // .....
        0b01010, // .#.#.
        0b01110, // .###.
        0b01110, // .###.
        0b00100, // ..#..
        0b00000, // .....
        0b00000, // .....
    };
    static const uint8_t skMobile1[8] PROGMEM  = // 5x8
    {
        0b00010, // ...#.
        0b00010, // ...#.
        0b01111, // .####
        0b01001, // .#..#
        0b01001, // .#..#
        0b01001, // .#..#
        0b01001, // .#..#
        0b01111, // .####
    };
    static const uint8_t skMobile2[8] PROGMEM  = // 5x8
    {
        0b00010, // ...#.
        0b00010, // ...#.
        0b01111, // .####
        0b01111, // .####
        0b01111, // .####
        0b01111, // .####
        0b01111, // .####
        0b01111, // .####
    };
    static const uint8_t skPhone1[8] PROGMEM  = // 5x8
    {
        0b00000, // .....
        0b00000, // .....
        0b01110, // .###.
        0b10101, // #.#.#
        0b00100, // ..#..
        0b11111, // #####
        0b10001, // #...#
        0b11111, // #####
    };
    static const uint8_t skPhone2[8] PROGMEM  = // 5x8
    {
        0b00000, // .....
        0b01110, // .###.
        0b10001, // #...#
        0b00000, // .....
        0b00100, // ..#..
        0b11111, // #####
        0b10001, // #...#
        0b11111, // #####
    };
    hd44780CreateChar(STATUS_CHAR_HEART_FULL,    skHeart1);
    hd44780CreateChar(STATUS_CHAR_HEART_SMALL,   skHeart2);
    hd44780CreateChar(STATUS_CHAR_MOBILE_READY,  skMobile1);
    hd44780CreateChar(STATUS_CHAR_MOBILE_PAIRED, skMobile2);
    hd44780CreateChar(STATUS_CHAR_PHONE_ONHOOK,  skPhone1);
    hd44780CreateChar(STATUS_CHAR_PHONE_OFFHOOK, skPhone2);

    // temporary
    hd44780PutCursor(0, 0);
    hd44780Printf_P(PSTR("flipflipPHONE"));
    //hd44780PutCursor(1, 0);
    //hd44780Printf_P(PSTR("(c) 2017 ffi"));
}

static void sStatusUpdate(void)
{
    static uint8_t heartbeat;
    heartbeat++;
    const bool mod2 = (heartbeat % 2) == 0 ? false : true;
    const bool mod4 = (heartbeat % 4) == 0 ? false : true;

    // heartbeat at top right
    hd44780PutCursor(0, 15);
    hd44780Write(mod4 ? STATUS_CHAR_HEART_FULL : STATUS_CHAR_HEART_SMALL );

    // module states
    const ARF32_STATE_t  arfState = arf32GetState();
    const AG1170_STATE_t agState  = ag1170GetState();
    const char *arfStr = arf32StateStr(arfState);
    const char *agStr  = ag1170StateStr(agState);;
    hd44780PutCursor(1, 0);
    hd44780Printf_P(PSTR("%S %S             "), arfStr, agStr);

    // status icons at bottom right
    hd44780PutCursor(1, 14);
    switch (arfState)
    {
        case ARF32_STATE_UNKNOWN:
            hd44780Write(mod2 ? '?' : ' ');
            break;
        case ARF32_STATE_READY:
            hd44780Write(STATUS_CHAR_MOBILE_READY);
            break;
        case ARF32_STATE_PAIRED:
            hd44780Write(STATUS_CHAR_MOBILE_PAIRED);
            break;
        case ARF32_STATE_INCALL:
            hd44780Write(mod2 ? STATUS_CHAR_MOBILE_PAIRED : STATUS_CHAR_MOBILE_READY);
            break;
        case ARF32_STATE_ERROR:
            hd44780Write(mod2 ? '!' : ' ');
            break;
    }
    switch (agState)
    {
        case AG1170_STATE_UNKNOWN:
            hd44780Write(mod2 ? '?' : ' ');
            break;
        case AG1170_STATE_ONHOOK:
            hd44780Write(STATUS_CHAR_PHONE_ONHOOK);
            break;
        case AG1170_STATE_OFFHOOK:
            hd44780Write(STATUS_CHAR_PHONE_OFFHOOK);
            break;
        case AG1170_STATE_ERROR:
            hd44780Write(mod2 ? '!' : ' ');
            break;
    }
}


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    while (ENDLESS)
    {
        sStatusUpdate();
        osTaskDelay(250);
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    arf32Status(str, size);
    PRINT_W("mon: arf32: %s", str);

    ag1170Status(str, size);
    PRINT_W("mon: ag1170: %s", str);

    /*const int n = */snprintf_P(str, size, PSTR("status... "));
    //arf32Status(&str[n], size - n);
}


//@}
// eof
