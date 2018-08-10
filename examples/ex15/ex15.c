/*!
    \file
    \brief flipflip's Arduino Uno stuff: fourteenth example application (see \ref EXAMPLES_EX15)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX15
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
#include "ssd1306.h"       // ff: SSD1306 driver

#include "ex15.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex15: init");

    ssd1306Init();

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

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    ssd1306Clear();

    for (uint16_t xy = 0; xy < 20; xy += 2)
    {
        ssd1306SetPixel(xy, xy, true);
    }
    ssd1306SetPixel(ssd1306Width() - 1, 0, true);
    ssd1306SetPixel(0, ssd1306Height() - 1, true);
    ssd1306SetPixel(ssd1306Width() - 1, ssd1306Height() - 1, true);
    ssd1306Print(20, 5, 1, "Abcde");
    ssd1306LineH(20, 5 + 7 + 1, 5 * 5 + 4 * 1, true);
    ssd1306LineV(ssd1306Width() - 2, 5, 30, true);
    ssd1306LineV(ssd1306Width() - 4, 5, 20, true);
    ssd1306Rect(0, 10, 5, 20, true);
    ssd1306Fill(2, 24, 15, 30, true);
    ssd1306Update();

    // keep running...
    while (ENDLESS)
    {
        sAppCnt++;
        DEBUG("app...");
        ssd1306Invert((sAppCnt % 2) == 0);

        if ((sAppCnt % 10 == 0))
        {
            static bool dim = true;
            ssd1306Dim(dim);
            dim = !dim;
        }
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
