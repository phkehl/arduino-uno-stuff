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

// initialise the user application
void appInit(void)
{
    DEBUG("ex10: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    hd44780Init();

    hd44780Printf_P(PSTR("hoihoi :-)))"));

    hd44780Cursor(5, 1);
    hd44780Printf_P(PSTR("gugus"));
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

    while (ENDLESS)
    {
        //PRINT("app... %"PRIu32, osTaskGetTicks());
        hd44780Cursor(0, 1);
        hd44780Printf_P(PSTR("msss=%"PRIu32), osTaskGetTicks());

        osTaskDelay(100);
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
