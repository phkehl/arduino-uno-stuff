/*!
    \file
    \brief flipflip's Arduino Uno stuff: fourteenth example application (see \ref EXAMPLES_EX14)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX14
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "i2c.h"           // ff: i2c driver

#include "ex14.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex14: init");

    i2cInit();

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

/* ***** application functions *********************************************** */


/* ***** application task **************************************************** */

static uint32_t sAppCnt = 0;


#define SI7021_I2C_ADDR   0x40  // 7bit address of the Si7021 chip
#define SI7021_CMD_RESET  0xfe  // reset command

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    const bool resStart = i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 1000);
    const bool resWrite = i2cWrite(SI7021_CMD_RESET);
    i2cStop();
    DEBUG("start=%"PRIi8" write=%"PRIi8, resStart, resWrite);

    // keep running...
    while (ENDLESS)
    {
        sAppCnt++;

        DEBUG("bla..");

        osTaskDelay(987);
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("sAppCnt=%"PRIu32), sAppCnt);
}


//@}
// eof
