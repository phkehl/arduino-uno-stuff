/*!
    \file
    \brief flipflip's Arduino Uno stuff: tenth example application (see \ref EXAMPLES_EX10)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX10
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

#include "ex10.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

static const uint8_t skHeart[8] PROGMEM  = // 5x8
{
    0x00, // .....
    0x0a, // .#.#.
    0x1f, // #####
    0x1f, // #####
    0x0e, // .###.
    0x04, // ..#..
    0x00, // .....
};

// initialise the user application
void appInit(void)
{
    DEBUG("ex10: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    hd44780Init();

    hd44780Printf_P(PSTR("hoihoi :-)))"));

    hd44780PutCursor(1, 5);
    hd44780Printf_P(PSTR("gugus"));

    hd44780CreateChar(0x01, skHeart);
    hd44780PutCursor(0, 12);
    hd44780Write(0x01);
    hd44780Write(0x01);
    hd44780PutCursor(0, 15);
    hd44780Write(43);

}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    osTaskDelay(5000);
    hd44780CursorMode(true, true);
    while (ENDLESS)
    {
        //PRINT("app... %"PRIu32, osTaskGetTicks());
        hd44780PutCursor(1, 0);
        hd44780Printf_P(PSTR("msss=%"PRIu32), osTaskGetTicks());

        osTaskDelay(1234);
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("status..."));
}


//@}
// eof
