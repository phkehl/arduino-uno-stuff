/*!
    \file
    \brief flipflip's Arduino Uno stuff: seventh example application (see \ref EXAMPLES_EX07)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX07
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "alimatrix.h"     // ff: Aliexpress LED matrix driver

#include "ex07.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex7: init");

    alimatrixInit();

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[200];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

#if 0
    while (ENDLESS)
    {
        PIN_LOW(_D10);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x11; loop_until_bit_is_set(SPSR, SPIF);
        osTaskDelay(1); PIN_HIGH(_D10);
        osTaskDelay(2000);

        PIN_LOW(_D10);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x11; loop_until_bit_is_set(SPSR, SPIF);
        osTaskDelay(1); PIN_HIGH(_D10);
        osTaskDelay(2000);

        PIN_LOW(_D10);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x11; loop_until_bit_is_set(SPSR, SPIF);
        osTaskDelay(1); PIN_HIGH(_D10);
        osTaskDelay(2000);

        PIN_LOW(_D10);
        SPDR = 0xf0; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x11; loop_until_bit_is_set(SPSR, SPIF);
        osTaskDelay(1); PIN_HIGH(_D10);
        osTaskDelay(2000);

        PIN_LOW(_D10);
        SPDR = 0xf0; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x11; loop_until_bit_is_set(SPSR, SPIF);
        osTaskDelay(1); PIN_HIGH(_D10);
        osTaskDelay(2000);

        PIN_LOW(_D10);
        SPDR = 0xff; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x11; loop_until_bit_is_set(SPSR, SPIF);
        osTaskDelay(1); PIN_HIGH(_D10);
        osTaskDelay(2000);

        PIN_LOW(_D10);
        SPDR = 0xf0; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x00; loop_until_bit_is_set(SPSR, SPIF);
        SPDR = 0x11; loop_until_bit_is_set(SPSR, SPIF);
        osTaskDelay(1); PIN_HIGH(_D10);
        osTaskDelay(2000);
    }
#endif

    alimatrixStart();

    while (ENDLESS)
    {
        PRINT("all");
        alimatrixClear();
        alimatrixSetRow(7, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(6, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(5, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(4, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(3, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(2, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(1, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(0, 0xff, 0x00, 0x00); // xxxxxxxx
        osTaskDelay(10000);

        PRINT("red");
        alimatrixClear();
        alimatrixSetRow(7, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(6, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(5, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(4, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(3, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(2, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(1, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(0, 0xff, 0x00, 0x00); // xxxxxxxx
        osTaskDelay(2000);
        PRINT("green");
        alimatrixClear();
        alimatrixSetRow(7, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(6, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(5, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(4, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(3, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(2, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(1, 0x00, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(0, 0x00, 0xff, 0x00); // xxxxxxxx
        osTaskDelay(2000);
        PRINT("blue");
        alimatrixClear();
        alimatrixSetRow(7, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(6, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(5, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(4, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(3, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(2, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(1, 0x00, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(0, 0x00, 0x00, 0xff); // xxxxxxxx
        osTaskDelay(2000);
        PRINT("red+green");
        alimatrixClear();
        alimatrixSetRow(7, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(6, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(5, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(4, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(3, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(2, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(1, 0xff, 0xff, 0x00); // xxxxxxxx
        alimatrixSetRow(0, 0xff, 0xff, 0x00); // xxxxxxxx
        osTaskDelay(2000);
        PRINT("red+blue");
        alimatrixClear();
        alimatrixSetRow(7, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(6, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(5, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(4, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(3, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(2, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(1, 0xff, 0x00, 0xff); // xxxxxxxx
        alimatrixSetRow(0, 0xff, 0x00, 0xff); // xxxxxxxx
        osTaskDelay(2000);
        PRINT("green+blue");
        alimatrixClear();
        alimatrixSetRow(7, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(6, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(5, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(4, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(3, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(2, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(1, 0x00, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(0, 0x00, 0xff, 0xff); // xxxxxxxx
        osTaskDelay(2000);
        PRINT("red+green+blue");
        alimatrixClear();
        alimatrixSetRow(7, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(6, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(5, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(4, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(3, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(2, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(1, 0xff, 0xff, 0xff); // xxxxxxxx
        alimatrixSetRow(0, 0xff, 0xff, 0xff); // xxxxxxxx
        osTaskDelay(2000);
        continue;

        PRINT("some pixels");
        hwTic(0);
        alimatrixClear();
        alimatrixSetXY(1, 7, true,  true,  true);  // white   .W......
        alimatrixSetXY(5, 6, false, true,  true);  // cyan    .....C..
        alimatrixSetXY(6, 5, true,  false, true);  // magenta ......M.
        alimatrixSetXY(3, 4, true,  true,  false); // yellow  ...Y....
        alimatrixSetXY(4, 3, false, false, true);  // blue    ....B...
        //                                                    ........
        alimatrixSetXY(2, 1, false, true,  false); // green   ..G.....
        alimatrixSetXY(0, 0, true,  false, false); // red     R.......
        DEBUG("%"PRIu16, hwToc(0));
        osTaskDelay(2000);

        PRINT("green triangle");
        hwTic(0);
        alimatrixClear();
        alimatrixSetRow(7, 0x00, 0x80, 0x00); // x.......
        alimatrixSetRow(6, 0x00, 0xc0, 0x00); // xx......
        alimatrixSetRow(5, 0x00, 0xe0, 0x00); // xxx.....
        alimatrixSetRow(4, 0x00, 0xf0, 0x00); // xxxx....
        alimatrixSetRow(3, 0x00, 0xf8, 0x00); // xxxxx...
        alimatrixSetRow(2, 0x00, 0xfc, 0x00); // xxxxxx..
        alimatrixSetRow(1, 0x00, 0xfe, 0x00); // xxxxxxx.
        alimatrixSetRow(0, 0x00, 0xff, 0x00); // xxxxxxxx
        DEBUG("%"PRIu16, hwToc(0));
        osTaskDelay(2000);

        PRINT("red heart");
        hwTic(0);
        alimatrixClear();
        alimatrixSetRow(7, 0x00, 0x00, 0x00); // ........
        alimatrixSetRow(6, 0x66, 0x00, 0x00); // .xx..xx.
        alimatrixSetRow(5, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(4, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(3, 0xff, 0x00, 0x00); // xxxxxxxx
        alimatrixSetRow(2, 0x7e, 0x00, 0x00); // .xxxxxx.
        alimatrixSetRow(1, 0x3c, 0x00, 0x00); // ..xxxx..
        alimatrixSetRow(0, 0x18, 0x00, 0x00); // ...xx...
        DEBUG("%"PRIu16, hwToc(0));
        osTaskDelay(2000);

        PRINT("random");
        alimatrixClear();
        uint16_t n = 2000;
        while (n--)
        {
            uint16_t rnd = hwMathGetRandom();
            const bool red   = rnd & 0x01 ? true : false;
            const bool green = rnd & 0x02 ? true : false;
            const bool blue  = rnd & 0x04 ? true : false;
            if (red || green || blue)
            {
                rnd >>= 3;
                const uint8_t x = rnd & 0x7;
                rnd >>= 3;
                const uint8_t y = rnd & 0x7;
                alimatrixSetXY(x, y, red, green, blue);
                osTaskDelay(5);
            }
        }
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
