/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFCLOCK)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFCLOCK
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
#include <avr/pgmspace.h>  // avr-libc: program Space Utilities
#include <avr/eeprom.h>    // avr-libc. EEPROM handling

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ubx.h"           // ff: u-blox binary protocol
#include "gnss.h"          // ff: GNSS receiver abstraction

#include "ffclock.h"


/* ***** configuration ******************************************************* */
// see also ffclock.h for the full pin list

// clock stepper motor signal pins
#define CLKMOT_PIN_I1 _PB1
#define CLKMOT_PIN_I2 _PB3
#define CLKMOT_PIN_I3 _PB2
#define CLKMOT_PIN_I4 _PB0

// minutes and hours "zero" position sensors (switches)
#define CLKZERO_MIN   _PD6
#define CLKZERO_HRS   _PD7

// minimum time between button presses
#define BUTTON_PRESS_TIME 150

// seconds display LEDs
#define SECONDS_LED_PIN_1 _PC0
#define SECONDS_LED_PIN_2 _PC1
#define SECONDS_LED_PIN_3 _PC2
#define SECONDS_LED_PIN_4 _PC3
#define SECONDS_LED_PIN_5 _PC4
#define SECONDS_LED_PIN_6 _PC5


/* ***** application functions *********************************************** */

static inline bool sIsMinutesZeroPos(void)
{
    return PIN_GET(CLKZERO_MIN);
}

static inline bool sIsHoursZeroPos(void)
{
    return PIN_GET(CLKZERO_HRS);
}

//------------------------------------------------------------------------------

static volatile bool svButtonAPressed;

static inline bool sButtonAPressed(void)
{
    const bool res = svButtonAPressed;
    svButtonAPressed = false;
    return res;
}

ISR(INT0_vect) // external interrupt 1
{
    osIsrEnter();
    static volatile uint32_t msss0;
    const uint32_t msss1 = osTaskGetTicks();
    if ( (msss1 - msss0) >= BUTTON_PRESS_TIME )
    {
        svButtonAPressed = true;
    }
    msss0 = msss1;
    osIsrLeave();
}

static volatile bool svButtonBPressed;

static inline bool sButtonBPressed(void)
{
    const bool res = svButtonBPressed;
    svButtonBPressed = false;
    return res;
}

ISR(INT1_vect) // external interrupt 0
{
    osIsrEnter();
    static volatile uint32_t msss0;
    const uint32_t msss1 = osTaskGetTicks();
    if ( (msss1 - msss0) >= BUTTON_PRESS_TIME )
    {
        svButtonBPressed = true;
    }
    msss0 = msss1;
    osIsrLeave();
}

//------------------------------------------------------------------------------

static void sSecondsSet(const uint8_t sec)
{
    if (sec & BIT(0)) { PIN_HIGH(SECONDS_LED_PIN_1); } else { PIN_LOW(SECONDS_LED_PIN_1); }
    if (sec & BIT(1)) { PIN_HIGH(SECONDS_LED_PIN_2); } else { PIN_LOW(SECONDS_LED_PIN_2); }
    if (sec & BIT(2)) { PIN_HIGH(SECONDS_LED_PIN_3); } else { PIN_LOW(SECONDS_LED_PIN_3); }
    if (sec & BIT(3)) { PIN_HIGH(SECONDS_LED_PIN_4); } else { PIN_LOW(SECONDS_LED_PIN_4); }
    if (sec & BIT(4)) { PIN_HIGH(SECONDS_LED_PIN_5); } else { PIN_LOW(SECONDS_LED_PIN_5); }
    if (sec & BIT(5)) { PIN_HIGH(SECONDS_LED_PIN_6); } else { PIN_LOW(SECONDS_LED_PIN_6); }
}

//------------------------------------------------------------------------------

#define CLOCK_EEPROM_MAGIC 0x55

// clock timezone offset
static int8_t sClockTzOffset;

static void sClockReadConfig(void)
{
    // read timezone offset
    while (!eeprom_is_ready())
    {
        WARNING("clock: eeprom");
        osTaskDelay(1000);
    }
    uint16_t cfg = eeprom_read_word((uint16_t *)0);
    uint8_t ck = (cfg & 0xff00) >> 8;
    int8_t pl = (int8_t)(cfg & 0x00ff);
    DEBUG("clock: cfg 0x%04"PRIx16" (0x%02"PRIu8", %"PRIi8")", cfg, ck, pl);
    if ( (ck != CLOCK_EEPROM_MAGIC) || (pl < -12) || (pl > 12) )
    {
        WARNING("clock: cfg tz offs");
        sClockTzOffset = 0;
    }
    else
    {
        sClockTzOffset = pl;
    }

    // display config
    DEBUG("clock: tzOffs=%"PRIi8, sClockTzOffset);
    uint8_t n = 30;
    while (n--)
    {
        sSecondsSet( (n % 2 ? 0x20 : 0x00) | (sClockTzOffset & 0x1f) );
        osTaskDelay(100);
    }
}

static void sClockDoConfig(void)
{
    PRINT("clock: config");

    bool done = FALSE;
    uint8_t n = 0;
    while (!done)
    {
        sSecondsSet( (n++ % 2 ? 0x20 : 0x00) | (sClockTzOffset & 0x1f) );

        if (sButtonBPressed())
        {
            sClockTzOffset++;
            if (sClockTzOffset > 12)
            {
                sClockTzOffset = -12;
            }
            DEBUG("clock: config tzOffs=%"PRIi8, sClockTzOffset);
        }
        else if (sButtonAPressed())
        {
            while (!eeprom_is_ready())
            {
                WARNING("clock: eeprom");
                osTaskDelay(1000);
            }
            const uint16_t cfg = (CLOCK_EEPROM_MAGIC << 8) | (sClockTzOffset & 0xff);
            DEBUG("clock: config store 0x%04"PRIx16, cfg);
            eeprom_update_word((uint16_t)0, cfg);
            done = TRUE;
        }
        osTaskDelay(100);
    }
}

//------------------------------------------------------------------------------

typedef enum CLKMOT_STEP_e
{
    CLKMOT_STEP_1   = 0, // step 1
    CLKMOT_STEP_2   = 1, // step 2
    CLKMOT_STEP_3   = 2, // step 3
    CLKMOT_STEP_4   = 3, // step 4
    CLKMOT_STEP_N   = 4, // number of steps
    CLKMOT_STEP_OFF = 5  // coils off
} CLKMOT_STEP_t;

static void sClockMotSet(const CLKMOT_STEP_t step)
{
    // http://www.tigoe.net/pcomp/code/circuits/motors/stepper-motors/
    switch (step)
    {
        case CLKMOT_STEP_4: // 1 0 1 0  ->  + +
            PIN_HIGH(CLKMOT_PIN_I1);
            PIN_LOW(CLKMOT_PIN_I2);
            PIN_HIGH(CLKMOT_PIN_I3);
            PIN_LOW(CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_3: // 0 1 1 0  ->  - +
            PIN_LOW(CLKMOT_PIN_I1);
            PIN_HIGH(CLKMOT_PIN_I2);
            PIN_HIGH(CLKMOT_PIN_I3);
            PIN_LOW(CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_2: // 0 1 0 1  ->  - -
            PIN_LOW(CLKMOT_PIN_I1);
            PIN_HIGH(CLKMOT_PIN_I2);
            PIN_LOW(CLKMOT_PIN_I3);
            PIN_HIGH(CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_1: // 1 0 0 1  ->  + -
            PIN_HIGH(CLKMOT_PIN_I1);
            PIN_LOW(CLKMOT_PIN_I2);
            PIN_LOW(CLKMOT_PIN_I3);
            PIN_HIGH(CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_OFF: // 0 0 0 0
        case CLKMOT_STEP_N:
        default:
            PIN_LOW(CLKMOT_PIN_I1);
            PIN_LOW(CLKMOT_PIN_I2);
            PIN_LOW(CLKMOT_PIN_I3);
            PIN_LOW(CLKMOT_PIN_I4);
            break;
    }
}

// configuration 1: old HP motor, small wheel:
//     - 48 steps
//     - 20 cogs
//     wheel:
//     - 50 cogs
//     clock:
//     - 60 minutes
//     - 24 hours
//     ==> 60 minutes: 50 / 20 * 48 = 120 steps
//     ==> 24 hours: 120 * 24 = 2880 steps
//
// configuration 2: old HP motor, big wheel:
//     - 48 steps
//     - 20 cogs
//     wheel:
// -     96 cogs
//     clock:
//     - 60 minutes
//     - 24 hours
//     ==> 60 minutes: 96 / 20 * 48 = 230.4 steps
//     ==> 1 minute: 230.4 / 60 = 3.84 steps
//     ==> 24 hours: 230.4 * 24 = 5529.6 steps
//     ==> 5 days: 5529.6 * 5 = 27648 steps
//
// configuration 3: China "28BYJ-48" motor with reduction gear
//     the datasheet claims 64 steps and reduction 1:64, but it's ~63.7:
//     http://forum.arduino.cc/index.php?topic=71964.msg1149466#msg1149466
//     (31*32*26*22)/(11*10*9*9) = 283712/4455 = 25792/405 = 63.68395...
//
//     time:          phase (steps):   (motor is 32 steps for one revolution)
//     00:00     0         0
//     00:01     1         33.96            1 / 60 * 32 * 63.68
//     01:00    60       2037.89           60 / 60 * 32 * 63.68
//     12:00   720      24454.64
//     18:00  1080      36681.96
//     24:00  1440      48909.27         1440 / 60 * 32 * 63.68
//

// current clock stepper motor step
static CLKMOT_STEP_t sClockMotPos;

// move fast
#define CLOCK_MOVE_FAST() \
    do { sClockMotSet(sClockMotPos); osTaskDelay(2);   \
         sClockMotPos++; sClockMotPos %= CLKMOT_STEP_N; } while (0)

// move slower but more precisely (?)
#define CLOCK_MOVE_SLOW(steps, delay) \
    do { uint16_t _s = steps; \
         while (_s--) { \
           sClockMotSet(sClockMotPos); osTaskDelay(delay); \
           sClockMotPos++; sClockMotPos %= CLKMOT_STEP_N; \
           sClockMotSet(sClockMotPos); osTaskDelay(delay); \
         } \
         sClockMotSet(CLKMOT_STEP_OFF); } while (0)

//------------------------------------------------------------------------------

// clock display zero calibration
#define CLOCK_CALIB_HOUR 12
#define CLOCK_CALIB_MIN  26

// approximate stepper steps required to advance one minute
#define CLOCK_APPROX_STEPS_PER_MIN 33.7 // 2036 / 60 = 33.9

// actual stepper steps required to advance one minute
const uint8_t skClockCalib[] PROGMEM =
{
    // 26 -> 27, 27 -> 28, ...
    32, 32, 36, 36, 32, 32, 36, 32, 36, 32, //  0 ..  9  -->  26 .. 35
    36, 32, 36, 32, 32, 36, 32, 36, 32, 36, // 10 .. 19  -->  36 .. 45
    32, 36, 32, 36, 36, 36, 32, 36, 32, 32, // 20 .. 29  -->  46 .. 55
    36, 36, 32, 32, 32, 36, 36, 36, 36, 36, // 30 .. 39  -->  56 .. 59 / 00 .. 05
    36, 32, 36, 32, 32, 32, 36, 32, 36, 32, // 40 .. 49  -->             06 .. 15
    32, 36, 36, 32, 32, 32, 32, 36, 36, 32  // 50 .. 59  -->             16 .. 25
    //                        ... 25 -> 26
};

//! currently displayed clock time (in minutes)
static uint16_t sClockTime;

// convert hours/minutes to minutes time
#define CLOCK_HM2T(h, m) (((uint16_t)(h) * (uint16_t)60) + (uint16_t)(m))

// convert minutes time to hours
#define CLOCK_T2H(t)     ((uint8_t)((uint16_t)(t) / 60))

// convert minutes time to minutes
#define CLOCK_T2M(t)     ((uint8_t)((uint16_t)(t) % 60))

// advance clock one minute
static void sClockMoveOneMinute(void)
{
    const uint8_t min = CLOCK_T2M(sClockTime);
    const uint8_t calibIx = min >= 26 ?
        min - CLOCK_CALIB_MIN    :
        (uint8_t)NUMOF(skClockCalib) - CLOCK_CALIB_MIN + min;
    uint8_t steps = pgm_read_byte(&skClockCalib[calibIx]);

    sClockTime++;
    if (sClockTime == CLOCK_HM2T(24,0))
    {
        sClockTime = CLOCK_HM2T(0,0);
    }

    DEBUG("clock: move %2"PRIu8" %2"PRIu8" %02"PRIu8":%02"PRIu8,
        calibIx, steps, CLOCK_T2H(sClockTime), CLOCK_T2M(sClockTime));

    /*
    // special move back to the minutes zero
    if (calibIx == (uint8_t)(NUMOF(skClockCalib)-1))
    {
        uint16_t steps2 = 0;
        while (hwClkGetZeroMinutes())
        {
            CLOCK_MOVE_SLOW(1, 5);
            steps2++;
        }
        DEBUG("clock: zero %2u", steps2);
    }
    else
    */    {
        CLOCK_MOVE_SLOW(steps, 2);
    }
}

//------------------------------------------------------------------------------

static GNSS_EPOCH_t sEpoch;


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());


#if 0
    while (ENDLESS)
    {
        if (sButtonAPressed())
        {
            DEBUG("A");
        }
        if (sButtonBPressed())
        {
            DEBUG("B");
        }
        osTaskDelay(10);
    }
#endif
#if 0
    static uint8_t sec;
    while (ENDLESS)
    {
        sSecondsSet(sec);
        sec++;
        osTaskDelay(1000);
    }
#endif
#if 0
    while (ENDLESS)
    {
        for (uint16_t steps = 1; steps <= 200; steps += 10)
        {
            DEBUG("steps=%"PRIu16, steps);
            CLOCK_MOVE_SLOW(steps, 2);
            osTaskDelay(500);
        }
        uint16_t steps = 5000;
        while (steps--)
        {
            CLOCK_MOVE_FAST();
        }
        sClockMotSet(CLKMOT_STEP_OFF);
    }
#endif
#if 0
    while (ENDLESS)
    {
        sClockMoveOneMinute();
        osTaskDelay(2000);
    }
#endif

    // enter config mode if both buttons are pressed on boot
    if (sButtonAPressed() && sButtonBPressed())
    {
        sClockDoConfig();
    }
    // load config from eeprom
    else
    {
        sClockReadConfig();
    }


    // keep running...
    while (ENDLESS)
    {
        if (gnssGetEpoch(&sEpoch, 5000))
        {
            sSecondsSet(sEpoch.sec);
            static char str[120];
            gnssStringifyEpoch(&sEpoch, str, sizeof(str));
            PRINT("epoch %s", str);
        }
        else
        {
            WARNING("no epoch :-(");
        }
    } // ENDLESS
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    ubxStatus(str, size);
    hwTxFlush();
    PRINT("mon: ubx: %s", str);
    gnssStatus(str, size);
    hwTxFlush();
    PRINT("mon: gnss: %s", str);
    const int n = snprintf_P(str, size, PSTR("clock %02"PRIu8":%02"PRIu8", gnss "),
        CLOCK_T2H(sClockTime), CLOCK_T2M(sClockTime));
    gnssStringifyTime(&sEpoch, &str[n], size - n);
}


/* ***** application init **************************************************** */

// initialise the user application
void appInit(void)
{
    DEBUG("ffclock: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    // initialise clock motor pins
    PIN_OUTPUT(CLKMOT_PIN_I1);
    PIN_OUTPUT(CLKMOT_PIN_I2);
    PIN_OUTPUT(CLKMOT_PIN_I3);
    PIN_OUTPUT(CLKMOT_PIN_I4);
    sClockMotSet(CLKMOT_STEP_OFF);

    // minutes and hours "zero" position sensors (switches)
    PIN_INPUT(CLKZERO_MIN);
    PIN_PULLUP_ON(CLKZERO_MIN);
    PIN_INPUT(CLKZERO_HRS);
    PIN_PULLUP_ON(CLKZERO_HRS);

    // buttons on INT0 & INT1
    PIN_INPUT(_PD2);
    PIN_PULLUP_ON(_PD2);
    PIN_INPUT(_PD3);
    PIN_PULLUP_ON(_PD3);
    EICRA = BIT(ISC01) | BIT(ISC11);   // falling-edge triggers interrupt
    EIMSK = BIT(INT0)  | BIT(INT1);    // enable INT0 & INT1 interrupts
    EIFR  = BIT(INTF0) | BIT(INTF1);   // clear interrups
    hwDelay(8000);

    // read button state on boot
    svButtonAPressed = !PIN_GET(_PD2);
    svButtonBPressed = !PIN_GET(_PD3);

    // seconds display LEDs
    PIN_OUTPUT(SECONDS_LED_PIN_1);
    PIN_LOW(SECONDS_LED_PIN_1);
    PIN_OUTPUT(SECONDS_LED_PIN_2);
    PIN_LOW(SECONDS_LED_PIN_2);
    PIN_OUTPUT(SECONDS_LED_PIN_3);
    PIN_LOW(SECONDS_LED_PIN_3);
    PIN_OUTPUT(SECONDS_LED_PIN_4);
    PIN_LOW(SECONDS_LED_PIN_4);
    PIN_OUTPUT(SECONDS_LED_PIN_5);
    PIN_LOW(SECONDS_LED_PIN_5);
    PIN_OUTPUT(SECONDS_LED_PIN_6);
    PIN_LOW(SECONDS_LED_PIN_6);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);

    // also start the GNSS task (handles reading from the receiver and collects epochs)
    gnssStartTask();
}

//------------------------------------------------------------------------------

//@}
// eof
