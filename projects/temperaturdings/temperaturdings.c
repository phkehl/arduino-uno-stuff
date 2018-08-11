/*!
    \file
    \brief flipflip's Arduino Uno stuff: Temperatur Dings (see \ref PROJECTS_TEMPERATURDINGS)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_TEMPERATURDINGS
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "si7021.h"        // ff: Si7021 driver
#include "ssd1306.h"       // ff: SSD1306 driver
#include "gfx.h"           // ff: graphics primitives
#include "version_gen.h"   // ff: generated version information

#include "temperaturdings.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);
static void sDisplayHello(void);

static OS_MUTEX_t sMeasMx;
static uint16_t sHumidity    = SI7021_NODATA;
static uint16_t sTemperature = SI7021_NODATA;
static uint32_t sMsssLastMeas;
static uint32_t sMeasCnt;

// initialise the user application
void appInit(void)
{
    DEBUG("ex14: init");

    // temperature and humidity sensor
    si7021Init();
    osMutexCreate(&sMeasMx);

    // display
    ssd1306Init();

    // graphics
    const GFX_DRV_t gfxDrv =
        GFX_DRV(ssd1306Clear, ssd1306Update, ssd1306Pixel, ssd1306Width(), ssd1306Height());
    gfxInit(&gfxDrv);
    sDisplayHello();

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

static void sDisplayHello(void)
{
    gfxClear();
    gfxRect(0, 0, gfxWidth() - 1, 33 /*gfxHeight() - 1*/, GFX_BLACK);
    gfxPrint(GFX_FONT_5X7,  3,  3, 1, GFX_BLACK, GFX_TRANS, "Temperatur");
    gfxPrint(GFX_FONT_5X7, 17, 11, 1, GFX_BLACK, GFX_TRANS, "Dings");
    //gfxLineH(0, 20, gfxWidth(), GFX_BLACK);
    gfxPrint(GFX_FONT_5X7, 11, 23, 1, GFX_BLACK, GFX_TRANS, "(c) ffi");
    gfxPrint(GFX_FONT_5X7,  5, 41, 1, GFX_BLACK, GFX_TRANS, FF_BUILDVER);
    gfxUpdate();
}

static void sDisplayData(void)
{
    gfxClear();
    static char hStr[10];
    static char tStr[10];
    uint8_t fh, ft;
    bool haveData = false;
    if (osMutexClaim(&sMeasMx, 100))
    {
        haveData = true;
        snprintf_P(hStr, sizeof(hStr), PSTR("%.2f"), (float)sHumidity * 0.01f);
        snprintf_P(tStr, sizeof(tStr), PSTR("%.2f"), (float)sTemperature * 0.01f);
        fh = sHumidity % 10;
        ft = sTemperature % 10;
        osMutexRelease(&sMeasMx);
    }
    else
    {
        fh = 0;
        ft = 0;
        strcpy_P(hStr, PSTR("????"));
        strcpy_P(tStr, PSTR("????"));
    }
    DEBUG("%08"PRIu32": RH=%s, T=%s", sMsssLastMeas, hStr, tStr);

    // strip last digit (xx.xx --> xx.x)
    hStr[4] = '\0'; // FIXME: can be 100.00
    tStr[4] = '\0';

    gfxPrint(GFX_FONT_5X7,  0,  7, 1, GFX_BLACK, GFX_TRANS, "R");
    gfxPrint(GFX_FONT_5X7, 10,  0, 2, GFX_BLACK, GFX_TRANS, hStr);
    if (haveData)
    {
        char str[2] = { '0' + fh, '\0' };
        gfxPrint(GFX_FONT_5X7, 58, 7, 1, GFX_BLACK, GFX_TRANS, str);
    }
    gfxPrint(GFX_FONT_5X7,  0, 25, 1, GFX_BLACK, GFX_TRANS, "T");
    gfxPrint(GFX_FONT_5X7, 10, 18, 2, GFX_BLACK, GFX_TRANS, tStr);
    if (haveData)
    {
        char str[2] = { '0' + ft, '\0' };
        gfxPrint(GFX_FONT_5X7, 58, 25, 1, GFX_BLACK, GFX_TRANS, str);
    }

    static char cStr[10];
    snprintf_P(cStr, sizeof(cStr), PSTR("%"PRIu32), sMeasCnt);
    gfxPrint(GFX_FONT_5X7, 0, 41, 1, GFX_BLACK, GFX_TRANS, cStr);

    gfxUpdate();
}

/* ***** application task **************************************************** */

static uint32_t sAppCnt = 0;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    osTaskDelay(500);

    static uint32_t sMsss;
    sMsss = osTaskGetMsss();

    // keep running...
    while (ENDLESS)
    {
        osTaskDelayUntil(&sMsss, 1000);
        sAppCnt++;

        if (osMutexClaim(&sMeasMx, 100))
        {
            si7021MeasHumidityAndTemperature(&sHumidity, &sTemperature);
            sMsssLastMeas = sMsss;
            sMeasCnt++;
            osMutexRelease(&sMeasMx);
        }
        else
        {
            continue;
        }

        if ((sAppCnt % 10) < 2)
        {
            sDisplayHello();
        }
        else
        {
            sDisplayData();
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    if (osMutexClaim(&sMeasMx, 100))
    {
        snprintf_P(str, size, PSTR("sAppCnt=%"PRIu32", sMeasCnt=%"PRIu32", RH=%.2f, T=%.2f"),
            sAppCnt, sMeasCnt, (float)sHumidity * 0.01f, (float)sTemperature * 0.01f);
        osMutexRelease(&sMeasMx);
    }
}

//@}
// eof
