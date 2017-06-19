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
#include "ffphone.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ffphone: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    hd44780Init();
    arf32Init();
}

// starts the user application tasks
void appCreateTask(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);

    arf32Start();

    hd44780PutCursor(0, 0);
    hd44780Printf_P(PSTR("flipflipPHONE"));
    hd44780PutCursor(1, 0);
    hd44780Printf_P(PSTR("(c) 2017 ffi"));
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
    static const uint8_t skPhone1[8] PROGMEM  = // 5x8
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
    static const uint8_t skPhone2[8] PROGMEM  = // 5x8
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
    hd44780CreateChar(0x01, skHeart1);
    hd44780CreateChar(0x02, skHeart2);
    hd44780CreateChar(0x03, skPhone1);
    hd44780CreateChar(0x04, skPhone2);
}


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    hd44780PutCursor(1, 15);
    hd44780Write(0x03);

    static uint8_t heartbeat;
    while (ENDLESS)
    {
        //DEBUG("app... %"PRIu32, osTaskGetTicks());
#if 0
        uint32_t n = 55555;
        while (n--)
        {
        }
        osTaskDelay(1234);
        heartbeat++;
        hd44780PutCursor(0, 15);
        hd44780Write( (heartbeat % 2) == 0 ? 0x01 : 0x02 );
#endif

        heartbeat++;
        hd44780PutCursor(0, 15);
        hd44780Write( (heartbeat % 4) == 0 ? 0x02 : 0x01 );
        osTaskDelay(250);

    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("status... "));
    //arf32Status(&str[n], size - n);
    arf32Status(NULL, 0);
}


//@}
// eof
