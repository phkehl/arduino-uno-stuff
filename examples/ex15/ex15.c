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
#include "gfx.h"           // ff: graphics primitives

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

    const GFX_DRV_t gfxDrv =
        GFX_DRV(ssd1306Clear, ssd1306Update, ssd1306Pixel, ssd1306Width(), ssd1306Height());
    gfxInit(&gfxDrv);

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

    // keep running...
    while (ENDLESS)
    {
        sAppCnt++;
        DEBUG("app... %"PRIu32, sAppCnt);

        gfxClear();

        for (uint16_t xy = 0; xy < 20; xy += 2)
        {
            gfxPixel(xy, xy, GFX_BLACK);
        }

        gfxPixel(gfxWidth() - 1, 0, GFX_BLACK);
        gfxPixel(0, gfxHeight() - 1, GFX_BLACK);
        gfxPixel(gfxWidth() - 1, gfxHeight() - 1, GFX_BLACK);

        gfxPrint(GFX_FONT_5X7, 17, 2, 1, GFX_BLACK, GFX_TRANS, "Hallo!");
        gfxLineH(17, 2 + 7 + 1, 6 * 5 + 5 * 1, GFX_BLACK);

        gfxLineV(gfxWidth() - 2, 5, 30, GFX_BLACK);
        gfxLineV(gfxWidth() - 4, 5, 20, GFX_BLACK);

        gfxRect(0, 10, 5, 20, GFX_BLACK);

        gfxFill(25, 13, 51, 22, GFX_BLACK);
        char str[10];
        snprintf(str, sizeof(str), "%4"PRIu32, sAppCnt);
        gfxPrint(GFX_FONT_5X7, 27, 14, 1, GFX_WHITE, GFX_TRANS, str);

        gfxLine(10, 24, 55, 30, GFX_BLACK);
        gfxLine(10, 26, 55, 35, GFX_BLACK);
        gfxLine(10, 28, 59, 43, GFX_BLACK);

        gfxPixel(0, 32, GFX_BLACK);
        gfxPixel(0, 34, GFX_BLACK);
        gfxPixel(0, 36, GFX_BLACK);
        gfxPixel(0, 38, GFX_BLACK);
        gfxPixel(0, 40, GFX_BLACK);
        gfxPixel(0, 42, GFX_BLACK);
        gfxPixel(0, 44, GFX_BLACK);
        gfxPixel(0, 46, GFX_BLACK);
        gfxPrint(GFX_FONT_5X7, 2, 32, 2, GFX_BLACK, GFX_TRANS, "Xx");

        gfxFill(28 + 0, 40 + 0, 29 + 0, 41 + 0, GFX_BLACK);
        gfxFill(28 + 3, 40 + 2, 29 + 3, 41 + 2, GFX_BLACK);
        gfxFill(28 + 6, 40 + 4, 29 + 6, 41 + 4, GFX_BLACK);

        gfxUpdate();

        if ((sAppCnt % 4 == 0) && false)
        {
            static bool invert = true;
            ssd1306Invert(invert);
            invert = !invert;
        }
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
