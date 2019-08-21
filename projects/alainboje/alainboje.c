/*!
    \file
    \brief flipflip's Arduino Uno stuff: Boje für Alain (see \ref PROJECTS_ALAINBOJE)

    - Copyright (c) 2019 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_ALAINBOJE
    @{
*/

#include <string.h>        // libc: string operations
#include <stdlib.h>

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects

#include "alainboje.h"

#define LED_IX_PS 1 // portside
#define LED_IX_SB 2 // starboard
#define LED_IX_TP 0 // top

#define HUE_GREEN (255 / (6 / 2))
#define HUE_RED   (0)

#define LEDS_FPS 100
#define LED_PERIOD (1000 / LEDS_FPS)

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    //ledfxSetIxRGB(0, 255, 0, 0);
    //ledfxSetIxRGB(1, 0, 255, 0);
    //ledfxSetIxRGB(2, 0, 0, 255);
    //ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
    //osTaskDelay(5000);

    // fancy init
#if 0
    for (uint8_t b = 1; b < 150; b += 5)
    {
        ledfxSetBrightness(b);
        const uint32_t r = hwMathGetRandom();
        ledfxSetIxHSV(0, r         % 0xff, 255, 255);
        ledfxSetIxHSV(1, (r >>  8) % 0xff, 255, 255);
        ledfxSetIxHSV(2, (r >> 16) % 0xff, 255, 255);
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(100);
    }
#endif

    ledfxSetBrightness(0);
    ledfxClear(0, 0);
    ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

    while (ENDLESS)
    {
        ledfxClear(0, 0);

        {
            static uint8_t phase = 0;
            phase++;
            phase %= 100;
            if (phase == 0)
            {
                DEBUG("boje..");
            }
        }

        {
            static uint8_t blink = 0;
            blink++;
            blink %= (800 / LED_PERIOD);
            if (blink < (100 / LED_PERIOD))
            {
                ledfxSetIxHSV(LED_IX_PS, HUE_RED, 255, 255);
                ledfxSetIxHSV(LED_IX_SB, HUE_GREEN, 255, 255);
            }
            else if (blink < (190 / LED_PERIOD))
            {
                // ...
            }
            else if (blink < (220 / LED_PERIOD))
            {
                ledfxSetIxHSV(LED_IX_PS, HUE_RED, 255, 100);
                ledfxSetIxHSV(LED_IX_SB, HUE_GREEN, 255, 100);
            }
        }

        {
            static uint8_t flick = 0;
            static uint8_t val = 0;
            static uint8_t hue = 200;
            // probabilites by "Eric", commented on
            // https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/#comment-1809
            // Probability  Random LED Brightness                               value 0..255
            //  50%          77% -  80% (its barely noticeable)                 196..204
            //  30%          80% - 100% (very noticeable, sim. air flicker)     204..255
            //   5%          50% -  80% (very noticeable, blown out flame)      128..204
            //   5%          40% -  50% (very noticeable, blown out flame)      102..128
            //  10%          30% -  40% (very noticeable, blown out flame)       77..102
            // Probability  Random Time (Duration)
            //  90%          20 ms
            //   3%          20 - 30 ms
            //   3%          10 - 20 ms
            //   4%           0 - 10 ms
            if (flick == 0)
            {
                const uint8_t bright = rand() % 100;
                if      (bright < 50)  { val = 196 + (rand() % (204 - 196)); }
                else if (bright < 80)  { val = 204 + (rand() % (255 - 204)); }
                else if (bright < 85)  { val = 128 + (rand() % (204 - 128)); }
                else if (bright < 90)  { val = 102 + (rand() % (128 - 102)); }
                else                   { val =  77 + (rand() % (102 -  77)); }
                val /= (256 / 180);

                const uint8_t time = rand() % 100;
                if      (time < 90) { flick =  20                         / (LED_PERIOD / 2); }
                else if (time < 93) { flick = (20 + (rand() % (30 - 20))) / (LED_PERIOD / 2); }
                else if (time < 96) { flick = (10 + (rand() % (20 - 10))) / (LED_PERIOD / 2); }
                else                { flick = (      rand() %  10       ) / (LED_PERIOD / 2) + 1; }
                flick *= 2;
                hue++;
            }
            ledfxSetIxHSV(LED_IX_TP, hue, 255, val);
            flick--;
        }


        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
        osTaskDelay(LED_PERIOD);
    }
}

// initialise the user application
void appInit(void)
{
    DEBUG("alainboje: init");
    ws2801Init();
}

// starts the user application tasks
void appCreateTask(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}



//@}
// eof
