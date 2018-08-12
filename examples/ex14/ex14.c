/*!
    \file
    \brief flipflip's Arduino Uno stuff: fourteenth example application (see \ref EXAMPLES_EX14)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX14
    @{

    \todo use checksums
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "si7021.h"        // ff: Si7021 driver

#include "ex14.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex14: init");

    si7021Init();

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}


// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 7, &task, stack, sizeof(stack), sAppTask, NULL);
}

/* ***** application functions *********************************************** */


/* ***** application task **************************************************** */

static uint32_t sAppCnt = 0;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // keep running...
    while (ENDLESS)
    {
        sAppCnt++;

        const uint16_t hum1  = si7021MeasHumidity();
        const uint16_t temp1 = si7021MeasTemperature();
        uint16_t hum2, temp2;
        si7021MeasHumidityAndTemperature(&hum2, &temp2);

        DEBUG("RH=%.2f (%.2f), T=%.2f (%.2f)",
            (float)hum1 * 0.01f, (float)hum2 * 0.01f,
            (float)temp1 * 0.01f, (float)temp2 * 0.01f);

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
