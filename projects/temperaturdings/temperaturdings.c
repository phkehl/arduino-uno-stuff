/*!
    \file
    \brief flipflip's Arduino Uno stuff: Temperatur Dings (see \ref PROJECTS_TEMPERATURDINGS)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_TEMPERATURDINGS
    @{

    \todo statistics (last minute, hour, day, week)
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
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

/* ***** application functions *********************************************** */

// measurement period [ms]
#define MEAS_PERIOD 1000

// number of measurements so far
static uint32_t sMeasCnt = 0;

// latest measurement
static uint16_t sLastMeasH = SI7021_NODATA;
static uint16_t sLastMeasT = SI7021_NODATA;
static uint32_t sLastMeasTime = 0;

// measurements of the last minute
#define MEAS_PER_MIN 5 // (60 * 1000 / MEAS_PERIOD)
static uint16_t sMinuteMeasH[MEAS_PER_MIN];
static uint16_t sMinuteMeasT[MEAS_PER_MIN];
static uint16_t sMinuteMeasIx;
static uint16_t sMinuteAvgH;
static uint16_t sMinuteMinH;
static uint16_t sMinuteMaxH;
static uint16_t sMinuteAvgT;
static uint16_t sMinuteMinT;
static uint16_t sMinuteMaxT;

// minute measurements of last last hour
#define MIN_PER_HOUR 4 // 60
static uint16_t sHourMeasH[MIN_PER_HOUR];
static uint16_t sHourMeasT[MIN_PER_HOUR];
static uint16_t sHourMeasIx;
static uint16_t sHourAvgH;
static uint16_t sHourMinH;
static uint16_t sHourMaxH;
static uint16_t sHourAvgT;
static uint16_t sHourMinT;
static uint16_t sHourMaxT;

// hour measurements of last last day
#define HOUR_PER_DAY 5 // 24
static uint16_t sDayMeasH[HOUR_PER_DAY];
static uint16_t sDayMeasT[HOUR_PER_DAY];
static uint16_t sDayMeasIx;
static uint16_t sDayAvgH;
static uint16_t sDayMinH;
static uint16_t sDayMaxH;
static uint16_t sDayAvgT;
static uint16_t sDayMinT;
static uint16_t sDayMaxT;

static void sInitStats(void)
{
    uint16_t ix;

    ix = NUMOF(sMinuteMeasH);
    while (ix > 0)
    {
        ix--;
        sMinuteMeasH[ix] = SI7021_NODATA;
        sMinuteMeasT[ix] = SI7021_NODATA;
    }
    sMinuteMeasIx = 0;
    sMinuteAvgH = SI7021_NODATA;
    sMinuteMinH = SI7021_NODATA;
    sMinuteMaxH = SI7021_NODATA;
    sMinuteAvgT = SI7021_NODATA;
    sMinuteMinT = SI7021_NODATA;
    sMinuteMaxT = SI7021_NODATA;

    ix = NUMOF(sHourMeasH);
    while (ix > 0)
    {
        ix--;
        sHourMeasH[ix] = SI7021_NODATA;
        sHourMeasT[ix] = SI7021_NODATA;
    }
    sHourMeasIx = 0;
    sHourAvgH = SI7021_NODATA;
    sHourMinH = SI7021_NODATA;
    sHourMaxH = SI7021_NODATA;
    sHourAvgT = SI7021_NODATA;
    sHourMinT = SI7021_NODATA;
    sHourMaxT = SI7021_NODATA;

    ix = NUMOF(sDayMeasH);
    while (ix > 0)
    {
        ix--;
        sDayMeasH[ix] = SI7021_NODATA;
        sDayMeasT[ix] = SI7021_NODATA;
    }
    sDayMeasIx = 0;
    sDayAvgH = SI7021_NODATA;
    sDayMinH = SI7021_NODATA;
    sDayMaxH = SI7021_NODATA;
    sDayAvgT = SI7021_NODATA;
    sDayMinT = SI7021_NODATA;
    sDayMaxT = SI7021_NODATA;
}

static void sMeasurement(void)
{
    sLastMeasTime = osTaskGetMsss();
    si7021MeasHumidityAndTemperature(&sLastMeasH, &sLastMeasT);
    sMeasCnt++;

    DEBUG("meas #%07"PRIu32" @ %010"PRIu32": H=%.2f, T=%.2f",
        sMeasCnt, sLastMeasTime, (float)sLastMeasH * 0.01f, (float)sLastMeasT * 0.01f);
}

static void sUpdateStats(void)
{
    // record last measurement
    sMinuteMeasH[sMinuteMeasIx] = sLastMeasH;
    sMinuteMeasT[sMinuteMeasIx] = sLastMeasT;
    sMinuteMeasIx++;
    sMinuteMeasIx %= NUMOF(sMinuteMeasH);

    bool minuteFull = false;
    if (sMinuteMeasIx == 0)
    {
        minuteFull = true;
    }

    // calculate last minute's average
    {
        uint32_t sumH = 0;
        uint16_t numH = 0;
        uint16_t minH = UINT16_MAX;
        uint16_t maxH = 0;

        uint32_t sumT = 0;
        uint16_t numT = 0;
        uint16_t minT = UINT16_MAX;
        uint16_t maxT = 0;

        uint16_t measIx = NUMOF(sMinuteMeasH);
        while (measIx > 0)
        {
            measIx--;
            const uint16_t measH = sMinuteMeasH[measIx];
            if (measH != SI7021_NODATA)
            {
                sumH += measH;
                numH++;
                if (measH > maxH) { maxH = measH; }
                if (measH < minH) { minH = measH; }
            }
            const uint16_t measT = sMinuteMeasT[measIx];
            if (measT != SI7021_NODATA)
            {
                sumT += measT;
                numT++;
                if (measT > maxT) { maxT = measT; }
                if (measT < minT) { minT = measT; }
            }
        }

        if (numH > 0)
        {
            sMinuteMinH = minH;
            sMinuteMaxH = maxH;
            sMinuteAvgH = sumH / numH;
        }
        else
        {
            sMinuteMinH = SI7021_NODATA;
            sMinuteMaxH = SI7021_NODATA;
            sMinuteAvgH = SI7021_NODATA;
        }
        if (numT > 0)
        {
            sMinuteMinT = minT;
            sMinuteMaxT = maxT;
            sMinuteAvgT = sumT / numT;
        }
        else
        {
            sMinuteMinT = SI7021_NODATA;
            sMinuteMaxT = SI7021_NODATA;
            sMinuteAvgT = SI7021_NODATA;
        }
    }

    if (minuteFull)
    {
        DEBUG("stats min: R=%.2f/%.2f/%.2f, T=%.2f/%.2f/%.2f",
            (float)sMinuteMinH * 0.01f, (float)sMinuteAvgH * 0.01f, (float)sMinuteMaxH * 0.01f,
            (float)sMinuteMinT * 0.01f, (float)sMinuteAvgT * 0.01f, (float)sMinuteMaxT * 0.01f);
    }

    // update minute average
    sHourMeasH[sHourMeasIx] = sMinuteAvgH;
    sHourMeasT[sHourMeasIx] = sMinuteAvgT;

    // record another minutes average every minute
    bool hourFull = false;
    if (minuteFull)
    {
        sHourMeasIx++;
        sHourMeasIx %= NUMOF(sHourMeasH);

        if (sHourMeasIx == 0)
        {
            hourFull = true;
        }
    }

    // calculate last hours's average
    {
        uint32_t sumH = 0;
        uint16_t numH = 0;
        uint16_t minH = UINT16_MAX;
        uint16_t maxH = 0;

        uint32_t sumT = 0;
        uint16_t numT = 0;
        uint16_t minT = UINT16_MAX;
        uint16_t maxT = 0;

        uint16_t measIx = NUMOF(sHourMeasH);
        while (measIx > 0)
        {
            measIx--;
            const uint16_t measH = sHourMeasH[measIx];
            if (measH != SI7021_NODATA)
            {
                sumH += measH;
                numH++;
                if (measH > maxH) { maxH = measH; }
                if (measH < minH) { minH = measH; }
            }
            const uint16_t measT = sHourMeasT[measIx];
            if (measT != SI7021_NODATA)
            {
                sumT += measT;
                numT++;
                if (measT > maxT) { maxT = measT; }
                if (measT < minT) { minT = measT; }
            }
        }

        if (numH > 0)
        {
            sHourMinH = minH;
            sHourMaxH = maxH;
            sHourAvgH = sumH / numH;
        }
        else
        {
            sHourMinH = SI7021_NODATA;
            sHourMaxH = SI7021_NODATA;
            sHourAvgH = SI7021_NODATA;
        }
        if (numT > 0)
        {
            sHourMinT = minT;
            sHourMaxT = maxT;
            sHourAvgT = sumT / numT;
        }
        else
        {
            sHourMinT = SI7021_NODATA;
            sHourMaxT = SI7021_NODATA;
            sHourAvgT = SI7021_NODATA;
        }
    }

    if (hourFull)
    {
        DEBUG("stats hour: R=%.2f/%.2f/%.2f, T=%.2f/%.2f/%.2f",
            (float)sHourMinH * 0.01f, (float)sHourAvgH * 0.01f, (float)sHourMaxH * 0.01f,
            (float)sHourMinT * 0.01f, (float)sHourAvgT * 0.01f, (float)sHourMaxT * 0.01f);
    }

    // update hour average
    sDayMeasH[sDayMeasIx] = sHourAvgH;
    sDayMeasT[sDayMeasIx] = sHourAvgT;

    // record another hours average every hour
    if (hourFull)
    {
        sDayMeasIx++;
        sDayMeasIx %= NUMOF(sDayMeasH);
    }

    // calculate last 24 hours (day's) average
    {
        uint32_t sumH = 0;
        uint16_t numH = 0;
        uint16_t minH = UINT16_MAX;
        uint16_t maxH = 0;

        uint32_t sumT = 0;
        uint16_t numT = 0;
        uint16_t minT = UINT16_MAX;
        uint16_t maxT = 0;

        uint16_t measIx = NUMOF(sDayMeasH);
        while (measIx > 0)
        {
            measIx--;
            const uint16_t measH = sDayMeasH[measIx];
            if (measH != SI7021_NODATA)
            {
                sumH += measH;
                numH++;
                if (measH > maxH) { maxH = measH; }
                if (measH < minH) { minH = measH; }
            }
            const uint16_t measT = sDayMeasT[measIx];
            if (measT != SI7021_NODATA)
            {
                sumT += measT;
                numT++;
                if (measT > maxT) { maxT = measT; }
                if (measT < minT) { minT = measT; }
            }
        }

        if (numH > 0)
        {
            sDayMinH = minH;
            sDayMaxH = maxH;
            sDayAvgH = sumH / numH;
        }
        else
        {
            sDayMinH = SI7021_NODATA;
            sDayMaxH = SI7021_NODATA;
            sDayAvgH = SI7021_NODATA;
        }
        if (numT > 0)
        {
            sDayMinT = minT;
            sDayMaxT = maxT;
            sDayAvgT = sumT / numT;
        }
        else
        {
            sDayMinT = SI7021_NODATA;
            sDayMaxT = SI7021_NODATA;
            sDayAvgT = SI7021_NODATA;
        }
    }
}

static void sDisplayHello(void)
{
    gfxClear();
    gfxRect(0, 0, gfxWidth() - 1, 33 /*gfxHeight() - 1*/, GFX_BLACK);
    gfxPrint(GFX_FONT_5X7,  3,  3, 1, GFX_BLACK, GFX_TRANS, "Temperatur");
    gfxPrint(GFX_FONT_5X7, 17, 11, 1, GFX_BLACK, GFX_TRANS, "Dings");
    //gfxLineH(0, 20, gfxWidth(), GFX_BLACK);
    gfxPrint(GFX_FONT_5X7, 11, 23, 1, GFX_BLACK, GFX_TRANS, "(c) ffi");
    gfxPrint(GFX_FONT_5X7, 9, 41, 1, GFX_BLACK, GFX_TRANS, FF_BUILDVER);
    gfxUpdate();
}

static void sDisplayData(void)
{
    gfxClear();
    static char strH[10];
    static char strT[10];
    uint8_t fH, fT;
    bool haveData = false;
    if (osMutexClaim(&sMeasMx, 50))
    {
        haveData = true;
        snprintf_P(strH, sizeof(strH), PSTR("%.2f"), (float)sLastMeasH * 0.01f);
        snprintf_P(strT, sizeof(strT), PSTR("%.2f"), (float)sLastMeasT * 0.01f);
        fH = sLastMeasH % 10;
        fT = sLastMeasT % 10;
        osMutexRelease(&sMeasMx);
    }
    else
    {
        WARNING("meas mx (disp)");
        fH = 0;
        fT = 0;
        strcpy_P(strH, PSTR("????"));
        strcpy_P(strT, PSTR("????"));
    }

    // strip last digit (xx.xx --> xx.x)
    strH[4] = '\0'; // FIXME: can be 100.00
    strT[4] = '\0';

    gfxPrint(GFX_FONT_5X7,  0,  7, 1, GFX_BLACK, GFX_TRANS, "R");
    gfxPrint(GFX_FONT_5X7, 10,  0, 2, GFX_BLACK, GFX_TRANS, strH);
    if (haveData)
    {
        char str[2] = { '0' + fH, '\0' };
        gfxPrint(GFX_FONT_5X7, 58, 7, 1, GFX_BLACK, GFX_TRANS, str);
    }
    gfxPrint(GFX_FONT_5X7,  0, 25, 1, GFX_BLACK, GFX_TRANS, "T");
    gfxPrint(GFX_FONT_5X7, 10, 18, 2, GFX_BLACK, GFX_TRANS, strT);
    if (haveData)
    {
        char str[2] = { '0' + fT, '\0' };
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

    static uint32_t sTime;
    sTime = osTaskGetMsss();

    sInitStats();

    // keep running...
    while (ENDLESS)
    {
        if (!osTaskDelayUntil(&sTime, MEAS_PERIOD))
        {
            WARNING("meas miss");
        }
        sAppCnt++;

        // do measurement and update statistics
        if (osMutexClaim(&sMeasMx, 500))
        {
            sMeasurement();
            sUpdateStats();

            osMutexRelease(&sMeasMx);
        }
        // must not happen, reset everything
        else
        {
            ERROR("meas mx (meas)");
            hwPanic(HW_PANIC_OTHER, 0, 0);
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
    if (osMutexClaim(&sMeasMx, 50))
    {
        snprintf_P(str, size,
            PSTR("sAppCnt=%"PRIu32", sMeasCnt=%"PRIu32", RH=%.2f (%.2f, %.2f, %.2f), T=%.2f (%.2f, %.2f, %.2f)"),
            sAppCnt, sMeasCnt,
            (float)sLastMeasH * 0.01f, (float)sMinuteAvgH * 0.01f, (float)sHourAvgH * 0.01f, (float)sDayAvgH * 0.01f,
            (float)sLastMeasT * 0.01f, (float)sMinuteAvgT * 0.01f, (float)sHourAvgT * 0.01f, (float)sDayAvgT * 0.01f);
        osMutexRelease(&sMeasMx);
    }
    else
    {
        WARNING("meas mx (mon)");
    }
}

//@}
// eof
