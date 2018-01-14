/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 8x8 (version b) (see \ref PROJECTS_FFMATRIX8X8B)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX8X8B
    @{
*/


#include <avr/io.h>        // avr: AVR device-specific IO definitions

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "tone.h"          // ff: tone and melody generator

#include "ffmatrix.h"
#include "ffmatrix8x8b.h"

void appInit(void)
{
    PIN_INPUT(_D9);
    PIN_PULLUP_ON(_D9);
    matrixInit();
}

static void sAppTask(void *pArg);

void appCreateTask(void)
{
    matrixCreateTask();

    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("snd", 7, &task, stack, sizeof(stack), sAppTask, NULL);
}

#if !defined(__DOXYGEN__)

// ../../tools/rtttl2melody.pl "HappyBday:d=4,32p,o=5,32p,b=125:8d.,32p,16d,32p,e,32p,d,32p,g,32p,2f#,32p,8d.,32p,16d,32p,e,32p,d,32p,a,32p,2g,32p,8d.,32p,16d,32p,d6,32p,b,32p,g,32p,f#,32p,2e,32p,8c.6,32p,16c6,32p,b,32p,g,32p,a,32p,2g"
// HappyBday melody
static const uint16_t skMelodyHappybday[] PROGMEM =
{
    TONE_NOTE_D5, 360, TONE_PAUSE, 60, TONE_NOTE_D5, 120, TONE_PAUSE, 60, TONE_NOTE_E5, 480,
    TONE_PAUSE, 60, TONE_NOTE_D5, 480, TONE_PAUSE, 60, TONE_NOTE_G5, 480, TONE_PAUSE, 60,
    TONE_NOTE_FS5, 960, TONE_PAUSE, 60, TONE_NOTE_D5, 360, TONE_PAUSE, 60, TONE_NOTE_D5, 120,
    TONE_PAUSE, 60, TONE_NOTE_E5, 480, TONE_PAUSE, 60, TONE_NOTE_D5, 480, TONE_PAUSE, 60,
    TONE_NOTE_A5, 480, TONE_PAUSE, 60, TONE_NOTE_G5, 960, TONE_PAUSE, 60, TONE_NOTE_D5, 360,
    TONE_PAUSE, 60, TONE_NOTE_D5, 120, TONE_PAUSE, 60, TONE_NOTE_D6, 480, TONE_PAUSE, 60,
    TONE_NOTE_B5, 480, TONE_PAUSE, 60, TONE_NOTE_G5, 480, TONE_PAUSE, 60, TONE_NOTE_FS5, 480,
    TONE_PAUSE, 60, TONE_NOTE_E5, 960, TONE_PAUSE, 60, TONE_PAUSE, 60, TONE_NOTE_C6, 120,
    TONE_PAUSE, 60, TONE_NOTE_B5, 480, TONE_PAUSE, 60, TONE_NOTE_G5, 480, TONE_PAUSE, 60,
    TONE_NOTE_A5, 480, TONE_PAUSE, 60, TONE_NOTE_G5, 960, TONE_END,
};

// ../../tools/rtttl2melody.pl "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6"
// Bond melody
static const uint16_t skMelodyBond[] PROGMEM =
{
    TONE_PAUSE, 94, TONE_NOTE_CS5, 188, TONE_NOTE_DS5, 94, TONE_NOTE_DS5, 94, TONE_NOTE_DS5, 188,
    TONE_NOTE_DS5, 375, TONE_NOTE_CS5, 188, TONE_NOTE_CS5, 188, TONE_NOTE_CS5, 188, TONE_NOTE_CS5, 188,
    TONE_NOTE_E6, 94, TONE_NOTE_E6, 94, TONE_NOTE_E6, 188, TONE_NOTE_E6, 375, TONE_NOTE_DS5, 188,
    TONE_NOTE_DS5, 188, TONE_NOTE_DS5, 188, TONE_NOTE_CS5, 188, TONE_NOTE_DS5, 94, TONE_NOTE_DS5, 94,
    TONE_NOTE_DS5, 188, TONE_NOTE_DS5, 375, TONE_NOTE_CS5, 188, TONE_NOTE_CS5, 188, TONE_NOTE_CS5, 188,
    TONE_NOTE_CS5, 188, TONE_NOTE_E6, 94, TONE_NOTE_E6, 94, TONE_NOTE_E6, 188, TONE_NOTE_E6, 375,
    TONE_NOTE_DS5, 188, TONE_NOTE_D6, 188, TONE_NOTE_CS5, 188, TONE_NOTE_CS5, 188, TONE_NOTE_GS5, 188,
    TONE_NOTE_FS5, 188, TONE_END,
};

// ../../tools/rtttl2melody.pl "ATeam:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#"
// ATeam melody
static const uint16_t skMelodyAteam[] PROGMEM =
{
    TONE_NOTE_DS5, 480, TONE_NOTE_AS5, 240, TONE_NOTE_DS5, 960, TONE_PAUSE, 120, TONE_NOTE_GS5, 240,
    TONE_NOTE_AS5, 480, TONE_NOTE_DS5, 480, TONE_PAUSE, 240, TONE_NOTE_G5, 120, TONE_NOTE_AS5, 120,
    TONE_NOTE_DS5, 240, TONE_NOTE_AS5, 240, TONE_NOTE_F6, 240, TONE_NOTE_DS5, 960, TONE_PAUSE, 120,
    TONE_NOTE_C6, 120, TONE_NOTE_AS5, 120, TONE_NOTE_GS5, 240, TONE_NOTE_AS5, 960, TONE_END,
};

// ../../tools/rtttl2melody.pl "BeethovenElise:d=8,o=5,b=125:32p,e6,d#6,e6,d#6,e6,b,d6,c6,4a.,c,e,a,4b.,e,g#,b,4c.6,e,e6,d#6,e6,d#6,e6,b,d6,c6,4a.,c,e,a,4b.,d,c6,b,2a"
// BeethovenElise melody
static const uint16_t skMelodyBeethovenelise[] PROGMEM =
{
    TONE_PAUSE, 60, TONE_NOTE_E6, 240, TONE_NOTE_DS5, 240, TONE_NOTE_E6, 240, TONE_NOTE_DS5, 240,
    TONE_NOTE_E6, 240, TONE_NOTE_B5, 240, TONE_NOTE_D6, 240, TONE_NOTE_C6, 240, TONE_NOTE_A5, 720,
    TONE_NOTE_C5, 240, TONE_NOTE_E5, 240, TONE_NOTE_A5, 240, TONE_NOTE_B5, 720, TONE_NOTE_E5, 240,
    TONE_NOTE_GS5, 240, TONE_NOTE_B5, 240, TONE_NOTE_E5, 240, TONE_NOTE_E6, 240, TONE_NOTE_DS5, 240,
    TONE_NOTE_E6, 240, TONE_NOTE_DS5, 240, TONE_NOTE_E6, 240, TONE_NOTE_B5, 240, TONE_NOTE_D6, 240,
    TONE_NOTE_C6, 240, TONE_NOTE_A5, 720, TONE_NOTE_C5, 240, TONE_NOTE_E5, 240, TONE_NOTE_A5, 240,
    TONE_NOTE_B5, 720, TONE_NOTE_D5, 240, TONE_NOTE_C6, 240, TONE_NOTE_B5, 240, TONE_NOTE_A5, 960,
    TONE_END,
};

#endif // !defined(__DOXYGEN__)


static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    osTaskDelay(4000);

    static bool sFirstPlay = true;

    while (ENDLESS)
    {
        osTaskDelay(1000);

        // sound off?
        if (PIN_GET(_D9))
        {
            continue;
        }

        PRINT("snd...");

        const uint8_t which = sFirstPlay ? 1 : (hwMathGetRandom() % 9);
        if (sFirstPlay)
        {
            sFirstPlay = false;
        }
        uint32_t duration = 0;

        switch (which)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5: duration = toneMelody(skMelodyHappybday, true);
                break;
            case 6:
            case 7: duration = toneMelody(skMelodyBond, true);
                break;
            case 8: duration = toneMelody(skMelodyAteam, true);
                break;
            case 9: duration = toneMelody(skMelodyBeethovenelise, true);
                break;
            default:
                duration = 60000;
                break;
        }
        DEBUG("playing %"PRIu8" for %"PRIu32"ms", which, duration);
        osTaskDelay(duration + 1000);

        // how many minutes delay?
        uint8_t mins = hwMathGetRandom() % 13 + 2;
        DEBUG("waiting %"PRIu8"mins", mins);
        while (mins-- > 0)
        {
            osTaskDelay(60000);
        }

    }

}

//@}
// eof
