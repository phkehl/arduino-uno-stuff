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
    static uint8_t stack[FF_APP_STACK_SIZE];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

/* ***** application functions *********************************************** */

// measurement period [ms]
#define MEAS_PERIOD 1000

// number of measurements so far
static uint32_t sMeasCnt = 0;

// latest measurement
static int16_t sLastMeasH = SI7021_NODATA;
static int16_t sLastMeasT = SI7021_NODATA;
static uint32_t sLastMeasTime = 0;

// measurements of the last minute
#define MEAS_PER_MIN (60 * (uint16_t)1000 / MEAS_PERIOD)
static int16_t sMinuteMeasT[MEAS_PER_MIN];
static int16_t sMinuteMeasH[MEAS_PER_MIN];
static int16_t sMinuteMeasIx;
static int16_t sMinuteAvgT;
static int16_t sMinuteMinT;
static int16_t sMinuteMaxT;
static int16_t sMinuteAvgH;
static int16_t sMinuteMinH;
static int16_t sMinuteMaxH;

// minute measurements of last last hour
#define MIN_PER_HOUR 60
static int16_t sHourMeasT[MIN_PER_HOUR];
static int16_t sHourMeasH[MIN_PER_HOUR];
static int16_t sHourMeasIx;
static int16_t sHourAvgT;
static int16_t sHourMinT;
static int16_t sHourMaxT;
static int16_t sHourAvgH;
static int16_t sHourMinH;
static int16_t sHourMaxH;

// hour measurements of last last day
#define HOUR_PER_DAY 24
static int16_t sDayMeasT[HOUR_PER_DAY];
static int16_t sDayMeasH[HOUR_PER_DAY];
static int16_t sDayMeasIx;
static int16_t sDayAvgT;
static int16_t sDayMinT;
static int16_t sDayMaxT;
static int16_t sDayAvgH;
static int16_t sDayMinH;
static int16_t sDayMaxH;

static void sInitStats(void)
{
    uint8_t ix;

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
        int32_t sumH = 0;
        int16_t numH = 0;
        int16_t minH = INT16_MAX;
        int16_t maxH = INT16_MIN;

        int32_t sumT = 0;
        int16_t numT = 0;
        int16_t minT = INT16_MAX;
        int16_t maxT = INT16_MIN;

        int16_t measIx = NUMOF(sMinuteMeasH);
        while (measIx > 0)
        {
            measIx--;
            const int16_t measH = sMinuteMeasH[measIx];
            if (measH != SI7021_NODATA)
            {
                sumH += measH;
                numH++;
                if (measH > maxH) { maxH = measH; }
                if (measH < minH) { minH = measH; }
            }
            const int16_t measT = sMinuteMeasT[measIx];
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
        int32_t sumH = 0;
        int16_t numH = 0;
        int16_t minH = INT16_MAX;
        int16_t maxH = INT16_MIN;

        int32_t sumT = 0;
        int16_t numT = 0;
        int16_t minT = INT16_MAX;
        int16_t maxT = INT16_MIN;

        uint16_t measIx = NUMOF(sHourMeasH);
        while (measIx > 0)
        {
            measIx--;
            const int16_t measH = sHourMeasH[measIx];
            if (measH != SI7021_NODATA)
            {
                sumH += measH;
                numH++;
                if (measH > maxH) { maxH = measH; }
                if (measH < minH) { minH = measH; }
            }
            const int16_t measT = sHourMeasT[measIx];
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
        int32_t sumH = 0;
        int16_t numH = 0;
        int16_t minH = INT16_MAX;
        int16_t maxH = INT16_MIN;

        int32_t sumT = 0;
        int16_t numT = 0;
        int16_t minT = INT16_MAX;
        int16_t maxT = INT16_MIN;

        int16_t measIx = NUMOF(sDayMeasH);
        while (measIx > 0)
        {
            measIx--;
            const int16_t measH = sDayMeasH[measIx];
            if (measH != SI7021_NODATA)
            {
                sumH += measH;
                numH++;
                if (measH > maxH) { maxH = measH; }
                if (measH < minH) { minH = measH; }
            }
            const int16_t measT = sDayMeasT[measIx];
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
    gfxPrint(GFX_FONT_5X7_1,  3,  3, GFX_BLACK, GFX_TRANS, "Temperatur");
    gfxPrint(GFX_FONT_5X7_1, 17, 11, GFX_BLACK, GFX_TRANS, "Dings");
    //gfxLineH(0, 20, gfxWidth(), GFX_BLACK);
    gfxPrint(GFX_FONT_5X7_1, 11, 23, GFX_BLACK, GFX_TRANS, "(c) ffi");
    gfxPrint(GFX_FONT_5X7_1, 9, 41, GFX_BLACK, GFX_TRANS, FF_BUILDVER);
    gfxUpdate();
}

static int16_t sRenderHead(const char *title, const uint32_t count)
{
    int16_t y = 0, x = 0;

    // title
    gfxPrint_P(GFX_FONT_5X7_1, 0, y, GFX_BLACK, GFX_TRANS, title);

    // count
    const uint8_t strLen = strlen_P(title);
    const int16_t strWidth = (5 + 1) * strLen;
    const uint8_t num = (gfxWidth() - strWidth) / 6;
    static char cStr[12];
    switch (num)
    {
        case 11: snprintf_P(cStr, sizeof(cStr), PSTR("%11"PRIu32), count); break;
        case 10: snprintf_P(cStr, sizeof(cStr), PSTR("%10"PRIu32), count); break;
        case  9: snprintf_P(cStr, sizeof(cStr), PSTR("%9"PRIu32),  count %   999999999); break;
        case  8: snprintf_P(cStr, sizeof(cStr), PSTR("%8"PRIu32),  count %    99999999); break;
        case  7: snprintf_P(cStr, sizeof(cStr), PSTR("%7"PRIu32),  count %     9999999); break;
        case  6: snprintf_P(cStr, sizeof(cStr), PSTR("%6"PRIu32),  count %      999999); break;
        case  5: snprintf_P(cStr, sizeof(cStr), PSTR("%5"PRIu32),  count %       99999); break;
        case  4: snprintf_P(cStr, sizeof(cStr), PSTR("%4"PRIu32),  count %        9999); break;
        case  3: snprintf_P(cStr, sizeof(cStr), PSTR("%3"PRIu32),  count %         999); break;
        case  2: snprintf_P(cStr, sizeof(cStr), PSTR("%2"PRIu32),  count %          99); break;
        case  1: snprintf_P(cStr, sizeof(cStr), PSTR("%1"PRIu32),  count %           9); break;
        default: cStr[0] = '\0'; break;
    }

    if (num > 0)
    {
        x = gfxWidth() - (num * 6) + 1;
        gfxPrint(GFX_FONT_5X7_1, x, y, GFX_BLACK, GFX_TRANS, cStr);
    }

    y += 7 + 1;
    //gfxLineH(0, y, gfxWidth() - 1, GFX_BLACK);
    for (x = 0; x < gfxWidth(); x += 2)
    {
        gfxPixel(x, y, GFX_BLACK);
    }
    y += 1;

    return y;
}

static void sFormatMeas(char *str, const uint8_t size, int16_t meas)
{
    if (size > 5)
    {
        if (meas == SI7021_NODATA)
        {
            strcpy_P(str, PSTR("?????"));
        }
        else
        {
            // limit display to 5 chars (from "-9.99" to "99.99")
            const float val = (float)CLIP(meas, -999, 9999) * 0.01f;
            snprintf_P(str, size, PSTR("%.2f"), val);
        }
    }
    else
    {
        *str = '\0';
    }
}

static int16_t sRenderMeas(const int16_t y, char label, const int16_t meas)
{
    char str[10];

    str[0] = label;
    str[1] = '\0';
    gfxPrint(GFX_FONT_5X7_1,  0, y + 7, GFX_BLACK, GFX_TRANS, str);

    sFormatMeas(str, sizeof(str), meas);
    const char frac = str[4];
    str[4] = '\0';
    gfxPrint(GFX_FONT_5X7_2, 10, y,     GFX_BLACK, GFX_TRANS, str);

    str[0] = frac;
    str[1] = '\0';
    gfxPrint(GFX_FONT_5X7_1, 58, y + 7, GFX_BLACK, GFX_TRANS, str);

    return y + (2 * 7);
}

static void sDisplayMeas(void)
{
    gfxClear();

    int16_t measT, measH;
    if (osMutexClaim(&sMeasMx, 50))
    {
        measT = sLastMeasT;
        measH = sLastMeasH;

        osMutexRelease(&sMeasMx);
    }
    else
    {
        WARNING("meas mx (disp)");

        measT = SI7021_NODATA;
        measH = SI7021_NODATA;
    }

    // render things
    int16_t y;
    y = sRenderHead(PSTR("now"), sMeasCnt);
    y = sRenderMeas(y + 5, 'T', measT);
    y = sRenderMeas(y + 5, 'H', measH);

    gfxUpdate();
}

static void sDisplayMinute(void)
{
    gfxClear();

    // render things
    int16_t y;
    y = sRenderHead(PSTR("minute"), sMinuteMeasIx);
    y = sRenderMeas(y + 5, 'T', sMinuteAvgT);
    y = sRenderMeas(y + 5, 'H', sMinuteAvgH);

    gfxUpdate();
}

static void sDisplayHour(void)
{
    gfxClear();

    // render things
    int16_t y;
    y = sRenderHead(PSTR("hour"), sHourMeasIx);
    y = sRenderMeas(y + 5, 'T', sHourAvgT);
    y = sRenderMeas(y + 5, 'H', sHourAvgH);

    gfxUpdate();
}

static void sDisplayDay(void)
{
    gfxClear();

    // render things
    int16_t y;
    y = sRenderHead(PSTR("day"), sDayMeasIx);
    y = sRenderMeas(y + 5, 'T', sDayAvgT);
    y = sRenderMeas(y + 5, 'H', sDayAvgH);

    gfxUpdate();
}

static int16_t sRenderStats(
    const int16_t y, const char label, const int16_t avg, const int16_t min, const int16_t max)
{
    char str[10];

    str[0] = label;
    str[1] = '\0';

    gfxPrint(GFX_FONT_5X7_1, 0, y, GFX_BLACK, GFX_TRANS, str);

    sFormatMeas(str, sizeof(str), avg);
    gfxPrint(GFX_FONT_5X7_1, 20, y, GFX_BLACK, GFX_TRANS, str);

    sFormatMeas(str, sizeof(str), min);
    str[4] = '\0';
    gfxPrint(GFX_FONT_5X7_1, 7, y + 7 + 2, GFX_BLACK, GFX_TRANS, str);

    sFormatMeas(str, sizeof(str), max);
    str[4] = '\0';
    gfxPrint(GFX_FONT_5X7_1, gfxWidth() / 2 + 7, y + 7 + 2, GFX_BLACK, GFX_TRANS, str);

    return y + 7 + 2 + 7 + 1;
}

static void sDisplayStatsMinute(void)
{
    gfxClear();

    // render things
    int16_t y;

    y = sRenderHead(PSTR("stats (min)"), 0);

    y = sRenderStats(y + 3, 'T', sMinuteAvgT, sMinuteMinT, sMinuteMaxT);
    y = sRenderStats(y + 3, 'H', sMinuteAvgH, sMinuteMinH, sMinuteMaxH);

    gfxUpdate();
}

static void sDisplayStatsHour(void)
{
    gfxClear();

    // render things
    int16_t y;

    y = sRenderHead(PSTR("stats (hr)"), 0);

    y = sRenderStats(y + 3, 'T', sMinuteAvgT, sMinuteMinT, sMinuteMaxT);
    y = sRenderStats(y + 3, 'H', sMinuteAvgH, sMinuteMinH, sMinuteMaxH);

    gfxUpdate();
}

static void sDisplayStatsDay(void)
{
    gfxClear();

    // render things
    int16_t y;

    y = sRenderHead(PSTR("stats (day)"), 0);

    y = sRenderStats(y + 3, 'T', sMinuteAvgT, sMinuteMinT, sMinuteMaxT);
    y = sRenderStats(y + 3, 'H', sMinuteAvgH, sMinuteMinH, sMinuteMaxH);

    gfxUpdate();
}

/* ***** application task **************************************************** */

static uint32_t sAppCnt = 0;


// display sequence
// (c = hello, x = meas., . = no update, _ = clear, m/h/d = avg minute/hour/day, M/H/D = stats minute/hour/day)
//                                                 1         2         3         4         5         6         7         8
//                                        12345678901234567890123456789012345678901234567890123456789012345678901234567890
const __flash char skDisplaySequence[] = "ccxxxxxxxxxxmmhhddxxxxxxxxxxMMHHDDxxxxxxxxxx";

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
        // FIXME: need to use sw watchdog as sometimes the i2c stuff hangs :-(
        sysAssertSwWatchdog();

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

        // update display
        static uint8_t seqIx;
        const char seqChar = pgm_read_byte(&skDisplaySequence[seqIx]);
        //DEBUG("seqIx=%"PRIu8" seqChar=%c", seqIx, seqChar);
        seqIx++;
        seqIx %= (sizeof(skDisplaySequence) - 1);

        switch (seqChar)
        {
            case 'c': sDisplayHello();         break;

            case 'x': sDisplayMeas();          break;

            case 'm': sDisplayMinute();        break;
            case 'h': sDisplayHour();          break;
            case 'd': sDisplayDay();           break;

            case 'M': sDisplayStatsMinute();   break;
            case 'H': sDisplayStatsHour();     break;
            case 'D': sDisplayStatsDay();      break;

            case '_': gfxClear(); gfxUpdate(); break;

            case '.':
            default:
                break;
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    if (osMutexClaim(&sMeasMx, 50))
    {
        //snprintf_P(str, size,
        //    PSTR("sAppCnt=%"PRIu32", sMeasCnt=%"PRIu32", RH=%.2f (%.2f, %.2f, %.2f), T=%.2f (%.2f, %.2f, %.2f)"),
        //    sAppCnt, sMeasCnt,
        //    (float)sLastMeasH * 0.01f, (float)sMinuteAvgH * 0.01f, (float)sHourAvgH * 0.01f, (float)sDayAvgH * 0.01f,
        //    (float)sLastMeasT * 0.01f, (float)sMinuteAvgT * 0.01f, (float)sHourAvgT * 0.01f, (float)sDayAvgT * 0.01f);
        snprintf_P(str, size,
            PSTR("sAppCnt=%"PRIu32", sMeasCnt=%"PRIu32", H=%.1f, T=%.1f"),
            sAppCnt, sMeasCnt, (float)sMinuteAvgH * 0.01f, (float)sMinuteAvgT * 0.01f);
        osMutexRelease(&sMeasMx);
    }
    else
    {
        WARNING("meas mx (mon)");
    }
}

//@}
// eof
