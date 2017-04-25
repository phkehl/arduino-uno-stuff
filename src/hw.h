/*!
    \file
    \brief flipflip's Arduino Uno stuff: hardware abstraction (see \ref HW)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \defgroup HW Hardware
    \ingroup FF

    This implements the hardware abstraction.

    @{
*/

#ifndef __HW_H__
#define __HW_H__

#include <stdarg.h>        // libc: variable argument lists

#include "stdstuff.h"      // ff: useful macros and types


/* ************************************************************************** */
// Hardware Initialisation (internal)

// initialise the hardware
void hwInit(void);

// make string containing the hardware status
void hwStatus(char *str, const uint16_t size);


/* ************************************************************************** */
// Debug Signals (internal)

// toggle tick debug LED, configurable via #FF_HW_TICK_PIN
void hwLedTickToggle(void);

// switch load debug LED on, configurable via #FF_HW_LOAD_PIN
void hwLedLoadOn(void);

// switch load debug LED off, configurable via #FF_HW_LOAD_PIN
void hwLedLoadOff(void);

// flush tx buffer to serial port, driven by system sSysTask()
void hwTxFlush(void);


/* ************************************************************************** */
/*!
    \name UART Input and Output

    The system is setup so that normal \ref avr_stdio functions, such as printf(), can be used.  The
    output is configurable via #FF_HW_TX_BUFSIZE. The input is configurable via
    #FF_HW_RX_BUFSIZE. The baudrate for rx and tx is configurable via #FF_HW_RXTX_BAUDRATE.

    See also \ref DEBUG functions PRINT() etc.

    @{
*/

//! get number of pending input bytes
/*
    Returns the number of pending input bytes, optionally waiting until at least
    one byte is available.

    \param[in] timeout  nunber of ticks to wait at most

    \returns the number of pending input bytes
*/
uint8_t hwGetRxBufSize(const uint32_t timeout);

//! get next pending input character
/*!
    Returns the next pending input character (if hwGetRxBufSize() is > 0)
    or '\0'.

    \returns next input character
*/
char hwReadNextChar(void);

//@}


/* ************************************************************************** */
/*!
    \name Exception Handler & Reset Debugging
    @{
*/

typedef enum HW_PANIC_e
{
    HW_PANIC_NONE           = 0,  //!< null problemo
    HW_PANIC_HW             = 1,  //!< \ref HW problem (blinks two times)
    HW_PANIC_OS             = 2,  //!< \ref OS problem (blinks three times)
    HW_PANIC_OTHER          = 3,  //!< other problem (blinks four times)
} HW_PANIC_t;

//! panic! (and display load LED blink code)
/*!
    Panics (halts the system), display debug info (UART, load LED) and resets the system after a
    while.

    \param reason  the reason for the panic
    \param u0      user display word 0
    \param u1      user display word 1
*/
void hwPanic(const HW_PANIC_t reason, const uint32_t u0, const uint32_t u1);

//! assert the watchdog
/*!
    Asserts (resets) the watchdog (counter).

    Called by osMonitorTask() on a regular basis (unless something stinks).

    Configurable via #FF_HW_USE_WATCHDOG.
*/
void hwAssertWatchdog(void);

//@}


/* ************************************************************************** */
/*!
    \name Utility Functions
    @{
*/

//! precise delay (blocking, busy-wait)
/*!
    Precisely delays by disabling all (?) interrupts (!) and busy-waiting (!).

    It will delay in bits of 10us delays. E.g. hwDelay(15) will delay 10us, hwDelay(21) will delay
    20us, hwDelay(8) won't delay, etc.

    \param[in] usDelay  delay in [us]
*/
void hwDelay(const uint32_t usDelay);

//! get good random number (slow!)
/*!
    Returns a good random number, suitable for seeding the fast generator via
    hwMathSeedRandom().

    Uses the MCU's built-in random number generator or other means (watchdog
    jitter).

    \todo it seems this only works when the scheduler is running.. why?

    \returns a fairly random number
*/
uint32_t hwGetRandomSeed(void);

//! start runtime measurement
/*!
    Starts a runtime measurement.

    \param[in] reg  register (< #FF_HW_NUM_TICTOC)
*/
void hwTic(const uint16_t reg);

//! stop runtime measurement
/*!
    Stops a runtime measurement and return the measurement.

    \param[in] reg  register (< #FF_HW_NUM_TICTOC)

    \return  the measurement [0.1ticks]
*/
uint16_t hwToc(const uint16_t reg);

//@}


/* ***** analog to digital conversion (ADC) ********************************* */
/*!
    \name Analog to Digital Conversion (ADC)
    @{
*/

//! ADC input pins
typedef enum HW_ADC_e
{
    HW_ADC_A0 = BIT(0), //!< pin A0 (PC0)
    HW_ADC_A1 = BIT(1), //!< pin A1 (PC1)
    HW_ADC_A2 = BIT(2), //!< pin A2 (PC2)
    HW_ADC_A3 = BIT(3), //!< pin A3 (PC3)
    HW_ADC_A4 = BIT(4), //!< pin A4 (PC4)
    HW_ADC_A5 = BIT(5), //!< pin A5 (PC5)
} HW_ADC_t;

//! initialise ADC hardware and configure pins
/*!
    \param[in] pins     mask of pins to configure
    \param[in] useAref  \c true if AREF pin is supplied with a reference voltage,
                        \c false if normal V_CC is to be used (typically: false)
*/
void hwAdcInit(const HW_ADC_t pins, const bool useAref);

//! read input on ADC pin and scale output value accordingly
/*!
    \param pin  the pin to read
    \param min  minimum value to output
    \param max  maximum value to output

    \returns value scaled from min..max, or raw ADC value if min == max
*/
int32_t hwAdcGetScaled(const HW_ADC_t pin, const int32_t min, const int32_t max);


/* ***** fast math functions ************************************************ */
/*!
    \name Fast(er) Math Functions
    @{
*/

//! fast cosf()
/*!
    Fast float cosine function.

    \todo implement something actually faster than cosf()

    \param phi  angle 0..2pi
    \return cos(phi) [rad]
*/
float hwMathFastCosf(const float phi);

//! fast sinf()
/*!
    Fast float sine function.

    \todo implement something actually faster than sinf()

    \param phi  angle 0..2pi
    \return sin(phi) [rad]
*/
float hwMathFastSinf(const float phi);

//! fast sqrtf()
/*!
    Fast float square root function.

    \todo implement something actually faster than sqrtf()

    \param x  value
    \return sqrt(x)
*/
float hwMathFastSqrtf(const float x);

//! seed random number generator
/*!
    Seeds a random number generator.

    \param seed  seed value
*/
void hwMathSeedRandom(const uint32_t seed);

//! get random number
/*!
    Returns a random number from the generator seeded by hwMathSeedRandom().

    \return  random number
*/
uint32_t hwMathGetRandom(void);

//@}


/* ************************************************************************** */

#endif // __HW_H__

//@}
// eof
