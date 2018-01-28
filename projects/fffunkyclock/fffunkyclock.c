/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's funky clock (see \ref PROJECTS_FFFUNKYCLOCK)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFFUNKYCLOCK
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
#include <avr/pgmspace.h>  // avr-libc: program Space Utilities
#include <avr/eeprom.h>    // avr-libc: EEPROM handling

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ubx.h"           // ff: u-blox binary protocol
#include "gnss.h"          // ff: GNSS receiver abstraction
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects

#include "fffunkyclock.h"


/* ***** configuration ******************************************************* */


/* ***** application functions *********************************************** */


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    //ledfxSetBrightness(100);

    uint8_t hue0 = 0;

    while (ENDLESS)
    {

#if 0
        ledfxSetIxRGB(0, 255, 255, 255);
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(500);

#endif
#if 1
        //DEBUG("hoihoi.. %"PRIu8" %"PRIu16"/%"PRIu16" %"PRIu16,
        //    hue0, FF_LEDFX_NUM_LED, ledfxNumLeds(), ledfxGetFrameBufferSize());
        uint8_t hue = hue0;
        for (uint16_t ix = 0; ix < FF_LEDFX_NUM_LED; ix++)
        {
            ledfxSetIxHSV(ix, hue, 255, 255);
        }
        hue0 += 1;

        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(20);

#endif
#if 0
        const uint8_t nope = (uint8_t)hwMathGetRandom() % 7;

        for (uint8_t ix = 0; ix < 7; ix++)
        {
            if (ix != nope)
            {
                ledfxSetIxHSV( (2 * ix),     hue0, 255, 255);
                ledfxSetIxHSV( (2 * ix) + 1, hue0, 255, 255);
                hue0 += 16;
            }
            else
            {
                ledfxSetIxRGB( (2 * ix),     0, 0, 0);
                ledfxSetIxRGB( (2 * ix) + 1, 0, 0, 0);
            }
        }
#endif

#if 0
        const uint8_t d0[] = { 0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13 };
        const uint8_t d1[] = { 4, 5, 12, 13 };
        const uint8_t d2[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        const uint8_t d3[] = { 2, 3, 4, 5, 6, 7, 10, 11, 12, 13 };
        const uint8_t d4[] = { 0, 1, 4, 5, 6, 7, 12, 13 };
        const uint8_t d5[] = { 0, 1, 2, 3, 6, 7, 10, 11, 12, 13 };
        const uint8_t d6[] = { 0, 1, 6, 7, 8, 9, 10, 11, 12, 13 };
        const uint8_t d7[] = { 2, 3, 4, 5, 12, 13 };
        const uint8_t d8[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
        const uint8_t d9[] = { 0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13 };

        DEBUG("0");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d0); ix++)
        {
            ledfxSetIxHSV(d0[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("1");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d1); ix++)
        {
            ledfxSetIxHSV(d1[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("2");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d2); ix++)
        {
            ledfxSetIxHSV(d2[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("3");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d3); ix++)
        {
            ledfxSetIxHSV(d3[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("4");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d4); ix++)
        {
            ledfxSetIxHSV(d4[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("5");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d5); ix++)
        {
            ledfxSetIxHSV(d5[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("6");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d6); ix++)
        {
            ledfxSetIxHSV(d6[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("7");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d7); ix++)
        {
            ledfxSetIxHSV(d7[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("8");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d8); ix++)
        {
            ledfxSetIxHSV(d8[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);

        DEBUG("9");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < NUMOF(d9); ix++)
        {
            ledfxSetIxHSV(d9[ix], hue0, 255, 255);
        }
        hue0 += 16;
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(2000);
#endif



    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("clock..."));
}


/* ***** application init **************************************************** */

// initialise the user application
void appInit(void)
{
    DEBUG("fffunkyclock: init");

    ws2801Init();

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);

    // also start the GNSS task (handles reading from the receiver and collects epochs)
    //gnssStartTask();
}

//------------------------------------------------------------------------------

//@}
// eof
