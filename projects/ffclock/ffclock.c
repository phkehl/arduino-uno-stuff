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
#include <avr/eeprom.h>    // avr-libc: EEPROM handling

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
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

// recalibration threshold [min]
#define CLOCK_RECALIB_THRS 10


/* ***** application functions *********************************************** */

static inline bool sIsMinutesZeroPos(void)
{
    return PIN_GET(CLKZERO_MIN) ? false : true;
}

static inline bool sIsHoursZeroPos(void)
{
    return PIN_GET(CLKZERO_HRS) ? false : true;
}

//------------------------------------------------------------------------------

static volatile bool svButtonAPressed;

static inline bool sButtonAPressed(void)
{
    const bool res = svButtonAPressed;
    svButtonAPressed = false;
    return res;
}

ISR(INT0_vect) // external interrupt 0
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

ISR(INT1_vect) // external interrupt 1
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

// FIXME: use timers?

static int8_t sEffectState;

// do the "Larson scanner" thing on the seconds display
static void sLarsonScanner(const bool init)
{
    if (init)
    {
        sEffectState = 1;
    }
    else
    {
        sEffectState++;
        if (sEffectState > 6)
        {
            sEffectState = -5;
        }
        else if (sEffectState == 0)
        {
            sEffectState = 2;
        }
    }
    sSecondsSet(1 << (ABS(sEffectState)-1));
}

// do a blinking progress bar style thing
static void sClockProgress(const uint8_t p)
{
    if (p == 0)
    {
        sEffectState = 0;
    }
    else
    {
        sEffectState++;
    }
    if (sEffectState % 2)
    {
        switch (p)
        {
            case 0: sSecondsSet(0x00); break; // ......
            case 1: sSecondsSet(0x20); break; // *.....
            case 2: sSecondsSet(0x30); break; // **....
            case 3: sSecondsSet(0x38); break; // ***...
            case 4: sSecondsSet(0x3c); break; // ****..
            case 5: sSecondsSet(0x3e); break; // *****.
            default:
            case 6: sSecondsSet(0x3f); break; // ******
        }
    }
    else
    {
        sSecondsSet(0x00);
    }
}

// blink seconds display
static void sClockBlink(uint8_t n)
{
    while (n--)
    {
        sSecondsSet(0xff);
        osTaskDelay(125);
        sSecondsSet(0x00);
        osTaskDelay(125);
    }
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
            PIN_LOW( CLKMOT_PIN_I2);
            PIN_HIGH(CLKMOT_PIN_I3);
            PIN_LOW( CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_3: // 0 1 1 0  ->  - +
            PIN_LOW( CLKMOT_PIN_I1);
            PIN_HIGH(CLKMOT_PIN_I2);
            PIN_HIGH(CLKMOT_PIN_I3);
            PIN_LOW( CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_2: // 0 1 0 1  ->  - -
            PIN_LOW( CLKMOT_PIN_I1);
            PIN_HIGH(CLKMOT_PIN_I2);
            PIN_LOW( CLKMOT_PIN_I3);
            PIN_HIGH(CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_1: // 1 0 0 1  ->  + -
            PIN_HIGH(CLKMOT_PIN_I1);
            PIN_LOW( CLKMOT_PIN_I2);
            PIN_LOW( CLKMOT_PIN_I3);
            PIN_HIGH(CLKMOT_PIN_I4);
            break;
        case CLKMOT_STEP_OFF:
        case CLKMOT_STEP_N: // 0 0 0 0  ->  . .
        default:
            PIN_LOW( CLKMOT_PIN_I1);
            PIN_LOW( CLKMOT_PIN_I2);
            PIN_LOW( CLKMOT_PIN_I3);
            PIN_LOW( CLKMOT_PIN_I4);
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

#define CLOCK_THM_FMT "%02"PRIu8":%02"PRIu8

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

    DEBUG("clock: move %2"PRIu8" %2"PRIu8" "CLOCK_THM_FMT,
        calibIx, steps, CLOCK_T2H(sClockTime), CLOCK_T2M(sClockTime));

    // special move back to the minutes zero
    if (calibIx == (uint8_t)(NUMOF(skClockCalib)-1))
    {
        uint16_t steps2 = 0;
        while (sIsMinutesZeroPos())
        {
            CLOCK_MOVE_SLOW(1, 5);
            steps2++;
        }
        DEBUG("clock: zero %2"PRIu16, steps2);
    }
    else
    {
        CLOCK_MOVE_SLOW(steps, 2);
    }
}

// move clock display (quickly) to the desired time
static void sClockMoveTo(const uint16_t T)
{
    const uint16_t T24 = CLOCK_HM2T(24, 0);
#if 1
    // somewhere before the calibrated 0 (xx:16)
    const uint16_t T0 = (((T + T24 - 1 - (CLOCK_CALIB_MIN + 10)) / 60) * 60) % T24;

    // move there fast and approximately
    const uint16_t dt0 = (T0 > sClockTime) ? (T0 - sClockTime) : (T0 + T24 - sClockTime);
    uint16_t steps = (uint16_t)((float)dt0 * (float)CLOCK_APPROX_STEPS_PER_MIN);

    DEBUG("clock: fast "CLOCK_THM_FMT" -> "CLOCK_THM_FMT" -> "CLOCK_THM_FMT" dt=%"PRIu16" steps=%"PRIu16,
          CLOCK_T2H(sClockTime), CLOCK_T2M(sClockTime),
          CLOCK_T2H(T), CLOCK_T2M(T),
          CLOCK_T2H(T0), CLOCK_T2M(T0), dt0, steps);
    sLarsonScanner(true);
    while (steps--)
    {
        CLOCK_MOVE_FAST();
        if ((steps % 32) == 0)
        {
            sLarsonScanner(false);
        }
    }

    sClockBlink(5);

    // slowly move to the calibrated zero (xx:26)
    while (!sIsMinutesZeroPos())
    {
        if ((++steps % 32) == 0)
        {
            sLarsonScanner(FALSE);
        }
        CLOCK_MOVE_FAST();
    }
    osTaskDelay(250);
    while (sIsMinutesZeroPos())
    {
        CLOCK_MOVE_SLOW(1, 5);
    }

    // now we're at xx:26 and need to go a few more minutes to hh:mm
    sClockTime = T0 + CLOCK_CALIB_MIN;

#else // FIXME: fix this, should be more reliable

    // closest minutes zero point (xx:26) before the desired time
    const uint16_t T0 = ((((T + T24 - 1 - CLOCK_CALIB_MIN) / 60) * 60) % T24) + CLOCK_CALIB_MIN;

    // hours to go
    const uint16_t dt0 = (T0 > sClockTime) ? (T0 - sClockTime) : (T0 + T24 - sClockTime);
    uint8_t hours = (dt0 / 60) + 1;

    DEBUG("clock: fast "CLOCK_THM_FMT" -> "CLOCK_THM_FMT": "CLOCK_THM_FMT" %"PRIu8,
          CLOCK_T2H(sClockTime), CLOCK_T2M(sClockTime),
          CLOCK_T2H(T), CLOCK_T2M(T),
          CLOCK_T2H(T0), CLOCK_T2M(T0), hours);

    sLarsonScanner(true);
    while (hours--)
    {
        DEBUG("clock: hours=%"PRIu8, hours);
        uint16_t steps = 0;
        while (!sIsMinutesZeroPos())
        {
            CLOCK_MOVE_FAST();
            if ((++steps % 32) == 0)
            {
                sLarsonScanner(false);
            }
        }
        while (sIsMinutesZeroPos())
        {
            CLOCK_MOVE_SLOW(1, 5);
        }
    }

    // now we're at xx:26 and need to go a few more minutes to hh:mm
    sClockTime = T0;

#endif

    sClockBlink(4);

    // advance slowly (in a controlled way) until the desired time
    while (sClockTime != T)
    {
        sClockMoveOneMinute();
        sLarsonScanner(FALSE);
    }
    sSecondsSet(0);
}

// do zero calibration
static void sClockDoCalibration(void)
{
    PRINT("clock: calibrate");

    // ***** 1) move a bit slowly (for the show) *****

    {
        // forget time
        sClockTime = 0;

        // flash a bit for fun..
        sClockBlink(5);

        sClockProgress(0);
        sClockProgress(1);

        // move a bit slowly..
        CLOCK_MOVE_SLOW(5*64, 5);
    }

    // ***** 2) make sure the zero triggers is not active *****

    {
        DEBUG("clock: calib: asserting triggers low");

        // assert none of the triggers are active...
        while (sIsMinutesZeroPos() || sIsHoursZeroPos())
        {

            CLOCK_MOVE_FAST();
        }

        sClockProgress(0);
        sClockProgress(2);

        // ..and move a bit further
        uint8_t n = 32;
        while (n--)
        {
            CLOCK_MOVE_SLOW(5*64/32, 2);
            sClockProgress(2);
        }
    }

    // ***** 3) approach hours wheel zero point (fast) *****

    {
        sClockProgress(0);
        sClockProgress(3);

        DEBUG("clock: calib: approaching hours zero");

        // go until trigger
        uint16_t n = 0;
        while (!sIsHoursZeroPos())
        {
            // move a bit
            CLOCK_MOVE_FAST();
            if ((n++ % 64) == 0)
            {
                sClockProgress(3);
            }
        }
    }

    // ***** 4) approach minutes zero point (slowly) and go just beyond it (very slowly) *****

    {
        DEBUG("clock: calib: approaching minutes zero");
        sClockProgress(0);
        sClockProgress(4);

        // go until trigger
        uint16_t n = 0;
        while (!sIsMinutesZeroPos())
        {
            CLOCK_MOVE_FAST();
            if ((n++ % 64) == 0)
            {
                sClockProgress(4);
            }
        }

        sClockProgress(0);
        sClockProgress(5);

        // slowly go just beyond the minutes zero point
        osTaskDelay(500);
        while (sIsMinutesZeroPos())
        {
            CLOCK_MOVE_SLOW(1, 5);
        }

        // now we should be at 12:26h, the calibrated zero
        sClockTime = CLOCK_HM2T(CLOCK_CALIB_HOUR, CLOCK_CALIB_MIN);
        DEBUG("clock: calib: at " STRINGIFY(CLOCK_CALIB_HOUR) ":" STRINGIFY(CLOCK_CALIB_MIN));

    }

    // ***** calibration done *****

    {
        sClockProgress(0);
        sClockProgress(6);
        PRINT("clock: calibrated");
        sClockBlink(5);
        osTaskDelay(1000);
    }
}



//------------------------------------------------------------------------------

static GNSS_EPOCH_t sEpoch;
static OS_MUTEX_t sEpochMutex;

#define GNSS_TIME_OKAY(ep) (ep.timeValid && (ep.tAcc <= GNSS_TACC_500MS))


//------------------------------------------------------------------------------

// clock state machine states
typedef enum CLOCK_STATE_e
{
    CLOCK_STATE_NONE   = 0, // don't know, initial state
    CLOCK_STATE_CALIB  = 1, // calibrating
    CLOCK_STATE_WAIT   = 2, // wait for GNSS time
    CLOCK_STATE_TICK   = 3  // ticking
} CLOCK_STATE_t;

// current clock state machine state
static CLOCK_STATE_t sClockState;

#ifndef __DOXYGEN__ // STFU
static const char skClockStateStrs[][6] PROGMEM =
{
    { "NONE\0" }, { "CALIB\0" }, { "WAIT\0" }, { "TICK\0" }
};
#endif


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);


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
#if 0
    while (ENDLESS)
    {
        DEBUG("zero: min=%S hrs=%S",
            sIsMinutesZeroPos() ? PSTR("yes") : PSTR("no"),
            sIsHoursZeroPos()   ? PSTR("yes") : PSTR("no"));
        osTaskDelay(1000);
    }
#endif
#if 0
    sClockDoCalibration();
#endif
#if 0
    // to manually come up with the skClockCalib[] table
    sClockDoCalibration();
    uint16_t steps = 0;
    while (ENDLESS)
    {
        if (sButtonAPressed())
        {
            CLOCK_MOVE_SLOW(4, 5);
            steps += 4;
            DEBUG("clock: A %"PRIu16, steps);
        }
        if (sButtonBPressed())
        {
            _CLOCK_MOVE_SLOW(32, 5);
            steps += 16;
            DEBUG("clock: B %"PRIu16, steps);
        }
        osTaskDelay(5);
    }
#endif
#if 0
    while (ENDLESS)
    {
        if (gnssGetEpoch(NULL, 5) && osMutexClaim(&sEpochMutex, 5))
        {
            if (gnssGetEpoch(&sEpoch, -1))
            {
                osMutexRelease(&sEpochMutex);

                sSecondsSet(sEpoch.sec);
                static char str[120];
                gnssStringifyEpoch(&sEpoch, str, sizeof(str));
                PRINT("epoch %S %s",
                    GNSS_TIME_OKAY(sEpoch) ? PSTR(":-)") : PSTR(":-("), str);

            }
        }
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

    // states
    bool haveTime = FALSE;
    CLOCK_STATE_t clockStatePrev = CLOCK_STATE_NONE;
    sClockMotPos = CLKMOT_STEP_1;
    sClockState = CLOCK_STATE_NONE;

    // run clock
    while (ENDLESS)
    {
        // debug state transitions
        if (clockStatePrev != sClockState)
        {
            DEBUG("clock: state -> %S", skClockStateStrs[sClockState]);
            clockStatePrev = sClockState;
        }

        // run state machine
        switch (sClockState)
        {
            // ***** initial state -> go calibrate *****************************
            case CLOCK_STATE_NONE:
                sClockState = CLOCK_STATE_CALIB;
                break;


            // ***** calibrate zero offset and move display to 00:00 ***********

            case CLOCK_STATE_CALIB:
            {
                // calibrate
                sClockDoCalibration();

                // wait for updated GNSS time information
                haveTime = false;
                if (gnssGetEpoch(NULL, 5000) && osMutexClaim(&sEpochMutex, 5))
                {
                    if (gnssGetEpoch(&sEpoch, -1))
                    {
                        osMutexRelease(&sEpochMutex);
                        if (GNSS_TIME_OKAY(sEpoch))
                        {
                            haveTime = true;
                        }
                    }
                }

                // go to current time if GNSS time is known
                if (haveTime)
                {
                    const int16_t t = ((int16_t)CLOCK_HM2T(sEpoch.hour, sEpoch.min)
                        + (int16_t)sClockTzOffset * 60) % (int16_t)(24 * 60);
                    sClockMoveTo((uint16_t)t);
                    sClockState = CLOCK_STATE_TICK;
                }
                // go to 00:00 otherwise
                else
                {
                    sClockMoveTo(CLOCK_HM2T(0,0));
                    PRINT("clock: waiting for GNSS time");
                    sClockState = CLOCK_STATE_WAIT;
                }
                break;
            }

            // ***** wait for GNSS time ****************************************

            case CLOCK_STATE_WAIT:
            {
                // wait for updated GNSS time information
                if (gnssGetEpoch(NULL, 5000) && osMutexClaim(&sEpochMutex, 5))
                {
                    if (gnssGetEpoch(&sEpoch, -1))
                    {
                        osMutexRelease(&sEpochMutex);
                        if (GNSS_TIME_OKAY(sEpoch))
                        {
                            PRINT("clock: GNSS time okay");

                            // set time on display
                            const int16_t t = ((int16_t)CLOCK_HM2T(sEpoch.hour, sEpoch.min)
                                + (int16_t)sClockTzOffset * 60) % (int16_t)(24 * 60);
                            const int16_t dt = t - (int16_t)sClockTime;

                            // we're close..
                            if ( (dt > 0) && (dt < 120) )
                            {
                                sLarsonScanner(true);
                                uint16_t n = (uint16_t)dt;
                                while (n != 0)
                                {
                                    sClockMoveOneMinute();
                                    sLarsonScanner(false);
                                }
                            }
                            // way off, use fast moving
                            else
                            {
                                sClockMoveTo(t);
                            }

                            // go tick
                            sClockState = CLOCK_STATE_TICK;
                        }
                        // still waiting..
                        else
                        {
                            sClockBlink(2);
                        }
                    }
                }
                break;
            }

            // ***** tick, tock, ... *******************************************

            case CLOCK_STATE_TICK:
            {
                // wait for updated GNSS time information
                if (gnssGetEpoch(NULL, 5000) && osMutexClaim(&sEpochMutex, 5))
                {
                    if (gnssGetEpoch(&sEpoch, -1))
                    {
                        osMutexRelease(&sEpochMutex);

                        // print epoch info
                        static char str[120];
                        gnssStringifyEpoch(&sEpoch, str, sizeof(str));
                        DEBUG("epoch %S %s",
                            GNSS_TIME_OKAY(sEpoch) ? PSTR(":-)") : PSTR(":-("), str);

                        // time valid?
                        if (GNSS_TIME_OKAY(sEpoch))
                        {
                            // update the seconds display
                            sSecondsSet(sEpoch.sec);

                            // compare time with our time
                            const int16_t t = ((int16_t)CLOCK_HM2T(sEpoch.hour, sEpoch.min)
                                + (int16_t)sClockTzOffset * 60) % (int16_t)(24 * 60);
                            const int16_t dt = (int16_t)sClockTime - t;

                            // warn if clock display is ahead
                            if (dt > 0)
                            {
                                sClockBlink(3);
                            }

                            // recalibrate if the time diverged too much from the display
                            if (ABS(dt) > CLOCK_RECALIB_THRS)
                            {
                                WARNING("clock: display off by %+"PRIi16, dt);
                                sClockState = CLOCK_STATE_NONE;
                            }

                            // on time or display lagging behind
                            if (dt < 0)
                            {
                                sClockMoveOneMinute();
                            }
                        }
                        // time invalid --> wait for time
                        else
                        {
                            WARNING("clock: GNSS time invalid");
                            sClockState = CLOCK_STATE_WAIT;
                        }
                    }
                }
                break;
            }

            default:
                sClockMotSet(CLKMOT_STEP_OFF);
                osTaskDelay(100);
                break;

        } // switch (sClockState)


    } // ENDLESS

}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    ubxStatus(str, size);
    PRINT_W("mon: ubx: %s", str);
    gnssStatus(str, size);
    PRINT_W("mon: gnss: %s", str);
    const int n = snprintf_P(str, size, PSTR("clock %S %02"PRIu8":%02"PRIu8", tz %+"PRIi8", gnss "),
        skClockStateStrs[sClockState], CLOCK_T2H(sClockTime), CLOCK_T2M(sClockTime), sClockTzOffset);
    if (osMutexClaim(&sEpochMutex, 10))
    {
        gnssStringifyTime(&sEpoch, &str[n], size - n);
        osMutexRelease(&sEpochMutex);
    }
    else
    {
        ERROR("clock: mx");
    }
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

    osMutexCreate(&sEpochMutex);
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
