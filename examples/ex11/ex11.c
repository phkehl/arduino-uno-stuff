/*!
    \file
    \brief flipflip's Arduino Uno stuff: eleventh example application (see \ref EXAMPLES_EX11)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX11
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "tone.h"          // ff: tone and melody generator

#include "ex11.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex11: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    toneInit();
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    static uint16_t melody1[] =
    {
        TONE_NOTE_C4, 200,  TONE_NOTE_C5, 200,  TONE_NOTE_C6, 200,  TONE_NOTE_C7, 200,
        TONE_PAUSE, 100,  TONE_NOTE_C6, 200,  TONE_PAUSE, 100,  TONE_NOTE_C5, 200,  TONE_PAUSE, 100,  TONE_NOTE_C4, 200,
        TONE_END
    };
    toneMelody(melody1, false);
    osTaskDelay(2000);

    static const uint16_t melody2[] PROGMEM =
    {
        TONE_NOTE_C4, 200,  TONE_NOTE_C5, 200,  TONE_NOTE_C6, 200,  TONE_NOTE_C7, 200,
        TONE_PAUSE, 100,  TONE_NOTE_C6, 200,  TONE_PAUSE, 100,  TONE_NOTE_C5, 200,  TONE_PAUSE, 100,  TONE_NOTE_C4, 200,
        TONE_END
    };
    toneMelody(melody2, true);
    osTaskDelay(2000);

    static const uint16_t notes[] PROGMEM =
    {
        //TONE_NOTE_C1, TONE_NOTE_CS1, TONE_NOTE_D1, TONE_NOTE_DS1, TONE_NOTE_E1, TONE_NOTE_F1, TONE_NOTE_FS1, TONE_NOTE_G1, TONE_NOTE_GS1, TONE_NOTE_A1, TONE_NOTE_AS1, TONE_NOTE_B1,
        //TONE_NOTE_C2, TONE_NOTE_CS2, TONE_NOTE_D2, TONE_NOTE_DS2, TONE_NOTE_E2, TONE_NOTE_F2, TONE_NOTE_FS2, TONE_NOTE_G2, TONE_NOTE_GS2, TONE_NOTE_A2, TONE_NOTE_AS2, TONE_NOTE_B2,
        TONE_NOTE_C3, TONE_NOTE_CS3, TONE_NOTE_D3, TONE_NOTE_DS3, TONE_NOTE_E3, TONE_NOTE_F3, TONE_NOTE_FS3, TONE_NOTE_G3, TONE_NOTE_GS3, TONE_NOTE_A3, TONE_NOTE_AS3, TONE_NOTE_B3,
        TONE_NOTE_C4, TONE_NOTE_CS4, TONE_NOTE_D4, TONE_NOTE_DS4, TONE_NOTE_E4, TONE_NOTE_F4, TONE_NOTE_FS4, TONE_NOTE_G4, TONE_NOTE_GS4, TONE_NOTE_A4, TONE_NOTE_AS4, TONE_NOTE_B4,
        TONE_NOTE_C5, TONE_NOTE_CS5, TONE_NOTE_D5, TONE_NOTE_DS5, TONE_NOTE_E5, TONE_NOTE_F5, TONE_NOTE_FS5, TONE_NOTE_G5, TONE_NOTE_GS5, TONE_NOTE_A5, TONE_NOTE_AS5, TONE_NOTE_B5,
        TONE_NOTE_C6, TONE_NOTE_CS6, TONE_NOTE_D6, TONE_NOTE_DS6, TONE_NOTE_E6, TONE_NOTE_F6, TONE_NOTE_FS6, TONE_NOTE_G6, TONE_NOTE_GS6, TONE_NOTE_A6, TONE_NOTE_AS6, TONE_NOTE_B6,
        TONE_NOTE_C7, TONE_NOTE_CS7, TONE_NOTE_D7, TONE_NOTE_DS7, TONE_NOTE_E7, TONE_NOTE_F7, TONE_NOTE_FS7, TONE_NOTE_G7, TONE_NOTE_GS7, TONE_NOTE_A7, TONE_NOTE_AS7, TONE_NOTE_B7,
    };
    static const char names[][4] PROGMEM =
    {
        //"C1\0", "CS1\0", "D1\0", "DS1\0", "E1\0", "F1\0", "FS1\0", "G1\0", "GS1\0", "A1\0", "AS1\0", "B1\0",
        //"C2\0", "CS2\0", "D2\0", "DS2\0", "E2\0", "F2\0", "FS2\0", "G2\0", "GS2\0", "A2\0", "AS2\0", "B2\0",
        "C3\0", "CS3\0", "D3\0", "DS3\0", "E3\0", "F3\0", "FS3\0", "G3\0", "GS3\0", "A3\0", "AS3\0", "B3\0",
        "C4\0", "CS4\0", "D4\0", "DS4\0", "E4\0", "F4\0", "FS4\0", "G4\0", "GS4\0", "A4\0", "AS4\0", "B4\0",
        "C5\0", "CS5\0", "D5\0", "DS5\0", "E5\0", "F5\0", "FS5\0", "G5\0", "GS5\0", "A5\0", "AS5\0", "B5\0",
        "C6\0", "CS6\0", "D6\0", "DS6\0", "E6\0", "F6\0", "FS6\0", "G6\0", "GS6\0", "A6\0", "AS6\0", "B6\0",
        "C7\0", "CS7\0", "D7\0", "DS7\0", "E7\0", "F7\0", "FS7\0", "G7\0", "GS7\0", "A7\0", "AS7\0", "B7\0",
    };
    for (uint8_t ix = 0; ix < NUMOF(notes); ix++)
    {
        const uint16_t freq = pgm_read_word(&notes[ix]);
        PRINT("%S (%"PRIu16"Hz)", names[ix], freq);
        toneGenerate(freq, 200);
        osTaskDelay(500);
    }


    while (ENDLESS)
    {
        PRINT("app..");

        osTaskDelay(1234);
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("status..."));
}


//@}
// eof
