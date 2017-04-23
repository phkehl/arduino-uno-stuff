/*!
    \file
    \brief flipflip's Arduino Uno stuff: hardware abstraction (see \ref HW)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup HW
    @{
*/

#include <stdio.h>         // libc: standard buffered input/output
#include <stdarg.h>        // libc: variable argument lists
#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions
#include <stdlib.h>        // libc: general utilities

#include <avr/io.h>        // avr: AVR device-specific IO definitions
#include <util/delay.h>    // avr: convenience functions for busy-wait delay loops
#include <avr/wdt.h>       // avr: watchdog timer handling
#include <avr/pgmspace.h>  // avr: program space utilities
#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
#include <avr/boot.h>      // avr: boot loader support
//#include <avr/signature.h> // avr: signature support

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system
#include "hw.h"            // ff: hardware


/* ***** serial port input and output *************************************** */

#if ( (FF_HW_RX_BUFSIZE > 0) && (FF_HW_RX_BUFSIZE < 50) )
#  error FF_HW_RX_BUFSIZE must be >= 50 bytes or it will not be useful.
#endif

#if ( (FF_HW_TX_BUFSIZE > 0) && (FF_HW_TX_BUFSIZE < 50) )
#  error FF_HW_TX_BUFSIZE must be >= 50 bytes or it will not be useful.
#endif

#if ( (FF_HW_RX_BUFSIZE > 0) || (FF_HW_TX_BUFSIZE > 0) )

// calculate baudrate parameters
#  define BAUD FF_HW_RXTX_BAUDRATE
#  define BAUD_TOL 5 // STFU, utils/setbaud.h!
#  include <util/setbaud.h> // avr: helper macros for baud rate calculations

static void sHwRxTxInit(void)
{
    // disable all while setting baud rate
    UCSR0A = 0x00;
    UCSR0B = 0x00;

    // set baudrate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#   if USE_2X
    SETBITS(UCSR0A, BIT(U2X0));
#   else
    CLRBITS(UCSR0A, BIT(U2X0));
#   endif

    // set mode to asynchronous USART, no parity, 1 stop bit, 8bit (i.e. 8N1)
    UCSR0C = BIT(UCSZ00) | BIT(UCSZ01);

    // enable transmitter & receiver
    SETBITS(UCSR0B, 0
#   if (FF_HW_TX_BUFSIZE > 0)
        | BIT(TXEN0)
#   endif
#   if (FF_HW_RX_BUFSIZE > 0)
        | BIT(RXEN0)
#   endif
        );
}

#else
static void sHwRxTxInit(void) { }
#endif // ( (FF_HW_RX_BUFSIZE > 0) || (FF_HW_TX_BUFSIZE > 0) )


#if (FF_HW_TX_BUFSIZE > 0)

static CH sHwTxBuf[FF_HW_TX_BUFSIZE]; //!< output buffer
static U1 sHwTxBufHead;               //!< write-to-buffer pointer (index)
static U1 sHwTxBufTail;               //!< read-from-buffer pointer (index)
static U1 sHwTxBufSize;               //!< size of buffered data
static U1 sHwTxBufPeak;               //!< peak output buffer size
static U  sHwTxBufDrop;               //!< number of dropped bytes

static U1 sHwTxFlush(void)
{
    // send queued characters
    if (sHwTxBufSize)
    {
        // character to send
        const CH c = /*~*/sHwTxBuf[sHwTxBufTail];

        // cancel and arm the tx complete interrupt and load next byte into the data register
        if (osTaskIsSchedulerRunning())
        {
            SETBITS(UCSR0A, BIT(TXC0));
            SETBITS(UCSR0B, BIT(TXCIE0));
        }
        UDR0 = c;

        // move tail
        CS_ENTER;
        sHwTxBufTail += 1;
        sHwTxBufTail %= sizeof(sHwTxBuf);
        sHwTxBufSize--;
        CS_LEAVE;
    }

    return sHwTxBufSize;
}

//! adds a character to the tx buffer
/*!
    Adds a character to the tx buffer, possibly waiting until there is space.

    \note This is used by the stdio subsystem, (printf() et al.).

    \param c      the character to put into the buffer
    \param pFile  the file handle (not actually used)

    \returns 1 on error (no more space in output buffer),
             0 if okay (character queued for output)
*/
static I sHwOutputPutChar(CH c, FILE *pFile)
{
    I res = 1;
    UNUSED(pFile);

    CS_ENTER;

    if ( (sHwTxBufSize == 0) || (sHwTxBufHead != sHwTxBufTail) )
    {
        sHwTxBuf[sHwTxBufHead] = c;
        sHwTxBufHead += 1;
        sHwTxBufHead %= sizeof(sHwTxBuf);
        sHwTxBufSize++;
        if (sHwTxBufSize > sHwTxBufPeak)
        {
            sHwTxBufPeak = sHwTxBufSize;
        }
        res = 0;
    }
    else
    {
        sHwTxBufDrop++;
    }

    CS_LEAVE;

    // flush now unless print task is running
    if (!osTaskIsSchedulerRunning())
    {
        do {
            loop_until_bit_is_set(UCSR0A, UDRE0);
        } while (sHwTxFlush());

        return 0;
    }

    return res;
}

//! the output file handle (write-only)
static FILE sHwOutputDev = FDEV_SETUP_STREAM(sHwOutputPutChar, NULL, _FDEV_SETUP_WRITE);

static OS_SEMAPHORE_t sHwTxReadySem;

ISR(USART_TX_vect) // UART, tx complete
{
    osIsrEnter();
    CLRBITS(UCSR0B, BIT(TXCIE0)); // USART tx complete interrupt enable
    osSemaphoreGive(&sHwTxReadySem);
    osIsrLeave();
}

static void sHwTxInit(void)
{
    // setup tx debugging
    SETBITS(DDRD, BIT(PD1)); // TXD output
    SETBITS(PORTD, BIT(PD1));

    // initialise output
    stdout = &sHwOutputDev;         // assign the debug port to stdout
    sHwTxBufHead = 0;           // initialise the output buffer
    sHwTxBufTail = 0;
    sHwTxBufSize = 0;
    sHwTxBufPeak = 0;
    sHwTxBufDrop = 0;

    // output sync
    osSemaphoreCreate(&sHwTxReadySem, 0);
}

void hwTxFlush(void)
{
    // flush all pending bytes
    while (sHwTxFlush())
    {
        // wait for tx complete
        osSemaphoreTake(&sHwTxReadySem, 10);
    }
}

#else
static void sHwTxInit(void) { }
void hwTxFlush(void) { }
#endif // (FF_HW_TX_BUFSIZE > 0)


#if (FF_HW_RX_BUFSIZE > 0)

static volatile CH svHwRxBuf[FF_HW_RX_BUFSIZE]; //!< data rx buffer
static volatile U1 svHwRxBufHead;               //!< write-to-buffer pointer (index)
static volatile U1 svHwRxBufTail;               //!< read-from-buffer pointer (index)
static volatile U1 svHwRxBufSize;               //!< size of buffered data
static volatile U1 svHwRxBufPeak;               //!< peak input buffer size
static volatile U  svHwRxBufDrop;               //!< number of dropped bytes

static OS_SEMAPHORE_t sHwRxReadySem;

//! reads a character from the rx buffer
/*
    Returns the next character from the rx buffer, possibly waiting until a
    character has been received.

    \note This is used by the stdio subsystem, (printf() et al.).

    \param pFile  the file handle (not actually used)

    \returns  the character
*/
static CH sHwInputGetChar(FILE *pFile)
{
    // wait until character is available
    while (svHwRxBufSize == 0)
    {
        // sleep
        if (osTaskIsSchedulerRunning())
        {
            osSemaphoreTake(&sHwRxReadySem, 10);
        }
        // or block
    }

    CH c;

    CS_ENTER;

    // get char
    c = /*~*/svHwRxBuf[svHwRxBufTail];
    // move tail
    svHwRxBufTail += 1;
    svHwRxBufTail %= sizeof(svHwRxBuf);
    svHwRxBufSize--;

    CS_LEAVE;

    return c;
}

//! the input file handle (read-only)
static FILE sHwInputDev = FDEV_SETUP_STREAM(NULL, sHwInputGetChar, _FDEV_SETUP_READ);

ISR(USART_RX_vect) // UART, rx complete
{
    osIsrEnter();

    const CH c = UDR0; // always read this or this ISR will fire continuously
    if ( (svHwRxBufSize == 0) || (svHwRxBufHead != svHwRxBufTail) )
    {
        svHwRxBuf[svHwRxBufHead] = c;
        svHwRxBufHead += 1;
        svHwRxBufHead %= sizeof(svHwRxBuf);
        svHwRxBufSize++;
        if (svHwRxBufSize > svHwRxBufPeak)
        {
            svHwRxBufPeak = svHwRxBufSize;
        }
    }
    else
    {
        svHwRxBufDrop++;
    }

    osSemaphoreGive(&sHwRxReadySem);

    osIsrLeave();
}

inline U1 hwGetRxBufSize(const U4 timeout)
{
    if (svHwRxBufSize == 0)
    {
        osSemaphoreTake(&sHwRxReadySem, timeout);
    }
    return svHwRxBufSize;
}

CH hwReadNextChar(void)
{
    return svHwRxBufSize > 0 ? fgetc(stdin) : '\0';
}

static void sHwRxInit(void)
{
    // setup receive pin
    CLRBITS(DDRD, BIT(PD0));
    SETBITS(PORTD, BIT(PD0));

    // initialise input buffer
    stdin = &sHwInputDev;          // assign the input port to stdin
    svHwRxBufHead = 0;          // initialise the output buffer
    svHwRxBufTail = 0;
    svHwRxBufSize = 0;
    svHwRxBufPeak = 0;
    svHwRxBufDrop = 0;

    // enable RX complete interrupt
    SETBITS(UCSR0B, BIT(RXCIE0));

    // input sync
    osSemaphoreCreate(&sHwRxReadySem, 0);
}

#else
inline U1 hwGetRxBufSize(const U4 timeout) { return 0; }
CH hwReadNextChar(void) { return '\0'; }
static void sHwRxInit(void) { }
#endif // (FF_HW_RX_BUFSIZE > 0)


/* ***** debug signal LEDs ************************************************** */

#if (PIN_TO_BIT(FF_HW_TICK_PIN) != PIN_BIT_NONE)
static void sHwLedTickInit(void)
{
    PIN_OUTPUT(FF_HW_TICK_PIN);
    PIN_LOW(FF_HW_TICK_PIN);
}

inline void hwLedTickToggle(void)
{
    PIN_TOGGLE(FF_HW_TICK_PIN);
}

#else
static void sHwLedTickInit(void) { }
inline void hwLedTickToggle(void) { }
#endif

#if (PIN_TO_BIT(FF_HW_LOAD_PIN) != PIN_BIT_NONE)

static void sHwLedLoadInit(void)
{
    PIN_OUTPUT(FF_HW_LOAD_PIN);
    PIN_LOW(FF_HW_LOAD_PIN);
}

inline void hwLedLoadOn(void)
{
    PIN_HIGH(FF_HW_LOAD_PIN);
}

inline void hwLedLoadOff(void)
{
    PIN_LOW(FF_HW_LOAD_PIN);
}

#else
static void sHwLedLoadInit(void) { }
inline void hwLedLoadOn(void) { }
inline void hwLedLoadOff(void) { }
#endif


/* ***** exception handling & reset debugging ******************************* */

const CH skHwPanicStr0[] PROGMEM = "NONE";
const CH skHwPanicStr1[] PROGMEM = "HW";
const CH skHwPanicStr2[] PROGMEM = "OS";
const CH skHwPanicStr3[] PROGMEM = "OTHER";

const PGM_P const skHwPanicStr[] PROGMEM =
{
    skHwPanicStr0, skHwPanicStr1, skHwPanicStr2, skHwPanicStr3
};

void hwPanic(const HW_PANIC_t reason, const U4 u0, const U4 u1)
{
    cli();
    osTaskSuspendScheduler();
    wdt_reset();
    wdt_disable();

    U n = 0, time = 0;
    U msss = osTaskGetTicks();

    switch (reason)
    {
        case HW_PANIC_HW:
            n = 2;
            time = 10000;
            break;
        case HW_PANIC_OS:
            n = 3;
            time = 10000;
            break;
        case HW_PANIC_OTHER:
            n = 4;
            time = 10000;
            break;
        default:
            break;
    }
    // blink load LED
    if (n > 0)
    {
        DEBUG(":-(");
        ERROR("PANIC @ %"F_U4": 0x%"F_Ux" %S (0x%08"F_U4x", 0x%08"F_U4x")",
              (U4)msss, (U)reason,
              (PGM_P)pgm_read_word(&skHwPanicStr[reason]), u0, u1);


        sHwLedLoadInit();
        U4 timeLeft = time ? time : 1;
        const U4 period = (n * 400) + (5 * 400);
        while (timeLeft)
        {
            I i = n;
            while (i--)
            {
                hwLedLoadOn();
                _delay_ms(200);
                hwLedLoadOff();
                _delay_ms(200);
            }
            i = 5;
            while (i--)
            {
                _delay_ms(200);
            }

            if (time)
            {
                if (timeLeft >= period)
                {
                    timeLeft -= period;
                }
                else
                {
                    timeLeft = 0;
                }
            }
//            wdt_reset();
        }

        ERROR("RESET");

        //void(* reset) (void) = 0;
        //reset();

        wdt_enable(WDTO_15MS);
        _delay_ms(100);
    }
}

// mirror of the MCU status register
uint8_t MCUSR_mirror __SECTION(.noinit);

// initialisation routine, called very early
void get_mcusr(void) __NAKED __SECTION(.init3);
void get_mcusr(void)
{
    if (MCUSR_mirror == 0)
    {
        // save and clear the MCU status register
        MCUSR_mirror = MCUSR;
    }
    MCUSR = 0;
}

// https://github.com/Optiboot/optiboot/issues/97
//uint8_t MCUSR_mirror_optiboot __SECTION(.noinit);
void get_mcusr_optiboot(void) __NAKED __SECTION(.init0);
void get_mcusr_optiboot(void)
{
    //__asm__ __volatile__ ("mov %0, r2\n" : "=r" (MCUSR_mirror_optiboot) :);
    __asm__ __volatile__ ("mov %0, r2\n" : "=r" (MCUSR_mirror) :);
}

static void sHwDebugFuses(void)
{
    // bit macros are in /usr/lib/avr/include/avr/iom328p.h, [AVR8271, chapter 27.2 "Fuse Bits"]

    // FUSE_CKSEL0, FUSE_CKSEL1, FUSE_CKSEL2, FUSE_CKSEL3, FUSE_SU%0, FUSE_SUT1, FUSE_CKOUT, FUSE_CKDIV8
    const U1 low  = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);

    // FUSE_BOOTRST, FUSE_BOOTSZ0, FUSE_BOOTSZ1, FUSE_EESAVE, FUSE_WDTON, FUSE_SPIEN, FUSE_DWEN, FUSE_RSTDISBL
    const U1 high = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);

    // FUSE_BODLEVEL0, FUSE_BODLEVEL1, FUSE_BODLEVEL2
    const U1 ext  = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);

    // there are no definitions for these in the header files it seems
    const U1 lock = boot_lock_fuse_bits_get(GET_LOCK_BITS);

    DEBUG("hw: fuses: low=0x%02"F_U1x" high=0x%02"F_U1x" ext=0x%02"F_U1x" lock=0x%02"F_U1x,
        low, high, ext, lock);

    // [AVR8271, chapter 27.3 "Signature Bytes"]
    //DEBUG("hw: signature: 0x%02"F_U1x" 0x%02"F_U1x" 0x%02"F_U1x, __signature[2], __signature[1], __signature[0]);
    const U1 sig0 = boot_signature_byte_get(SIGNATURE_0);
    const U1 sig1 = boot_signature_byte_get(SIGNATURE_1);
    const U1 sig2 = boot_signature_byte_get(SIGNATURE_2);
    DEBUG("hw: signature: 0x%02"F_U1x" 0x%02"F_U1x" 0x%02"F_U1x, sig0, sig1, sig2);
}

static void sHwCheckResetCause(void)
{
    // cannot check reset cause if we're using a boot loader,
    // i.e. if the boot reset vector is enabled (FUSE_BOOTRST=0)
    //const U1 high = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
    //if ( (high & ~FUSE_BOOTRST) == 0 )
    //{
    //    //return;
    //}

    // check reset cause
    if (MCUSR_mirror & BIT(WDRF))
    {
        WARNING("hw: reset: watchdog");
    }
    if ( (MCUSR_mirror & BIT(BORF)) && ((MCUSR_mirror & BIT(PORF)) == 0) )
    {
        WARNING("hw: reset: brown-out");
    }
    if (MCUSR_mirror & BIT(EXTRF))
    {
        DEBUG("hw: reset: external");
    }
    if (MCUSR_mirror & BIT(PORF))
    {
        DEBUG("hw: reset: power-on");
    }
}

inline void hwAssertWatchdog(void)
{
    wdt_reset();
}


/* ***** utility functions ************************************************** */

void hwDelay(const U4 usDelay)
{
    CS_ENTER;

    U4 d = usDelay;
    while (d >= 10)
    {
        // the argument to _delay_us() must be a compile-time constant
        _delay_us(10);
        d -= 10;
    }

    CS_LEAVE;
}

static volatile U4 sHwRandomSeed = 0;

ISR(WDT_vect)
{
    sHwRandomSeed <<= 8;
    sHwRandomSeed ^= (U4)TCNT1;
}

U4 hwGetRandomSeed(void)
{
    // temporarily disable the watchdog reset and use the
    // watchdog timer jitter to generate a better seed
    WDTCSR |= BIT(WDCE);    // allow changes
    WDTCSR = BIT(WDIE);     // set 16ms timer and enable interrupt (but not the reset)
    if (osTaskIsSchedulerRunning())
    {
        osTaskDelay(100);   // wait some time until the interrup has fired a few times
    }
    else
    {
        hwDelay(100000);
    }
    WDTCSR |= BIT(WDCE);    // disable the..
    WDTCSR = 0;             // ..watchdog
#if (FF_HW_USE_WATCHDOG > 0)
    wdt_enable(WDTO_8S);    // and enable the default watchdog again
#endif
    const U4 seed = sHwRandomSeed; // this should now be fairly random
    DEBUG("hw: seed 0x%08"F_U4x, seed);
    return seed;
}


#if (FF_HW_NUM_TICTOC > 0)

#define HW_TICTOC_FREQ (F_CPU/1024)
#define HW_NUM_TICTOC_COUNTERS 4
static U sHwTicTocRegs[FF_HW_NUM_TICTOC];

static void sHwTicTocInit(void)
{
    // setup TIC/TOC performance counter
    TCCR1A = 0;         // normal mode
    TCCR1B = BIT(CS12) | BIT(CS10); // prescale clk/1024
    TCCR1C = 0;
}

void hwTic(const U reg)
{
    if (reg < FF_HW_NUM_TICTOC)
    {
        sHwTicTocRegs[reg] = (U)TCNT1;
    }
}

U hwToc(const U reg)
{
    U res = 0;
    if (reg < FF_HW_NUM_TICTOC)
    {
        const U delta = (U)TCNT1 - sHwTicTocRegs[reg];
        res = ((U4)delta * 1000 * 10) / (U4)HW_TICTOC_FREQ;
    }
    return res;
}

#else
static void sHwTicTocInit(void) { }
void hwTic(const U reg) { UNUSED(reg); }
U hwToc(const U reg) { UNUSED(reg); return 0; }
#endif // (FF_HW_NUM_TICTOC > 0)


/* **** analog to digital conversion (ADC) ********************************** */

static OS_SEMAPHORE_t sHwAdcReadySem;

ISR(ADC_vect) // ADC conversion complete
{
    osIsrEnter();
    // disable ADC
    CLRBITS(ADCSRA, BIT(ADEN));
    // signal the waiting task that the ADC conversion has finished
    osSemaphoreGive(&sHwAdcReadySem);
    osIsrLeave();
}

void hwAdcInit(const HW_ADC_t pins, const L useAref)
{
    // setup selected pin(s) for input
    U1 didr = 0;
    if (pins & HW_ADC_A0)
    {
        PIN_INPUT(A0);
        PIN_PULLUP_OFF(A0);
        didr |= BIT(ADC0D);
    }
    if (pins & HW_ADC_A1)
    {
        PIN_INPUT(A1);
        PIN_PULLUP_OFF(A1);
        didr |= BIT(ADC1D);
    }
    if (pins & HW_ADC_A2)
    {
        PIN_INPUT(A2);
        PIN_PULLUP_OFF(A2);
        didr |= BIT(ADC2D);
    }
    if (pins & HW_ADC_A3)
    {
        PIN_INPUT(A3);
        PIN_PULLUP_OFF(A3);
        didr |= BIT(ADC3D);
    }
    if (pins & HW_ADC_A4)
    {
        PIN_INPUT(A4);
        PIN_PULLUP_OFF(A4);
        didr |= BIT(ADC4D);
    }
    if (pins & HW_ADC_A5)
    {
        PIN_INPUT(A5);
        PIN_PULLUP_OFF(A5);
        didr |= BIT(ADC5D);
    }

    // disable digital input buffers on pins used for ADC input
    DIDR0 = didr; // Digital Input Disable Register 0

    // select external AREF or normal AVCC
    ADMUX = useAref ?  // ADC Multiplexer Selection Register
        0 :            // AREF, Internal V ref turned off
        BIT(REFS0);    // AV_CC with external capacitor at AREF pin

    ADCSRA =           // ADC Control and Status Register A
      //BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0); // ADC clock: 16MHz / 128 = 125kHz
      //BIT(ADPS2) | BIT(ADPS1);              // ADC clock: 16MHz / 64 = 250kHz
        BIT(ADPS2)              | BIT(ADPS0); // ADC clock: 16MHz / 32 = 500kHz
      //             BIT(ADPS1) | BIT(ADPS0); // ADC clock: 16MHz / 16 = 1000kHz

    ADCSRB =           // ADC Control and Status Register B
        0;             // free running, no trigger

    osSemaphoreCreate(&sHwAdcReadySem, 0);

}

I4 hwAdcGetScaled(const HW_ADC_t pin, const I4 min, const I4 max)
{
    // select ADC channel
    U1 mux = 0;
    switch (pin)
    {
        case HW_ADC_A0: mux = 0         | 0         | 0        ; break;
        case HW_ADC_A1: mux = 0         | 0         | BIT(MUX0); break;
        case HW_ADC_A2: mux = 0         | BIT(MUX1) | 0        ; break;
        case HW_ADC_A3: mux = 0         | BIT(MUX1) | BIT(MUX0); break;
        case HW_ADC_A4: mux = BIT(MUX2) | 0         | 0        ; break;
        case HW_ADC_A5: mux = BIT(MUX2) | 0         | BIT(MUX0); break;
    }
    ADMUX = (ADMUX & 0xf0) | mux;

    // start conversion
    ADCSRA |=   // ADC Control and Status Register A
        BIT(ADEN) | // ADC enable
        BIT(ADSC) | // ADC start conversion
        BIT(ADIE) | // ADC interrupt enable
        BIT(ADIF);  // clear ADC interrupt flag

    // wait for conversion to finish
    if (osSemaphoreTake(&sHwAdcReadySem, 2))
    {
        if (min == max)
        {
            return (I4)ADC;
        }
        else
        {
            return (((I4)ADC * (max - min)) / ((I4)1023)) + min;
        }
    }
    else
    {
        return min;
    }
}


/* **** fast (or not) math functions **************************************** */

inline R4 hwMathFastCosf(const R4 phi)
{
    // TODO: implement
    return cosf(phi);
}


inline R4 hwMathFastSinf(const R4 phi)
{
    // TODO: implement
    return sinf(phi);
}


inline R4 hwMathFastSqrtf(const R4 x)
{
    // TODO: implement
    return sqrtf(x);
}

inline void hwMathSeedRandom(const U4 seed)
{
    srandom(seed);
}

inline U4 hwMathGetRandom(void)
{
    return random();
}

/* **** hardware initialisation ********************************************* */

void hwInit(void)
{
    // wait for stuff to power up, and don't mess with the ISP programmer
    _delay_ms(250);

#if (FF_HW_USE_WATCHDOG > 0)
    wdt_enable(WDTO_8S);
#endif

    sHwRxTxInit();
    sHwTxInit();
    sHwRxInit();

    DEBUG("hw: init (rx %"F_U1", tx %"F_U1")", FF_HW_RX_BUFSIZE, FF_HW_TX_BUFSIZE);

    sHwCheckResetCause();
    sHwDebugFuses();

    sHwLedTickInit();
    sHwLedLoadInit();

    sHwTicTocInit();
}

void hwStatus(CH *str, const U size)
{
#if ( (FF_HW_RX_BUFSIZE > 0) && (FF_HW_TX_BUFSIZE > 0) )
    snprintf_P(str, size,
        PSTR("rxbuf=%"F_U1"/%"F_U1"/%"F_U1" (%"F_U") txbuf=%"F_U1"/%"F_U1"/%"F_U1" (%"F_U")"),
        svHwRxBufSize, svHwRxBufPeak, sizeof(svHwRxBuf), svHwRxBufDrop,
        sHwTxBufSize, sHwTxBufPeak, sizeof(sHwTxBuf), sHwTxBufDrop);
    svHwRxBufPeak = 0;
    sHwTxBufPeak = 0;
#elif (FF_HW_RX_BUFSIZE > 0)
    snprintf_P(str, size,
        PSTR("rxbuf=%"F_U1"/%"F_U1"/%"F_U1" (%"F_U")"),
        svHwRxBufSize, svHwRxBufPeak, sizeof(svHwRxBuf), svHwRxBufDrop);
    svHwRxBufPeak = 0;
#elif (FF_HW_TX_BUFSIZE > 0)
    snprintf_P(str, size,
        PSTR("txbuf=%"F_U1"/%"F_U1"/%"F_U1" (%"F_U")"),
        sHwTxBufSize, sHwTxBufPeak, sizeof(sHwTxBuf), sHwTxBufDrop);
    sHwTxBufPeak = 0;
#endif
    str[size-1] = '\0';
}

/* ************************************************************************** */
//@}
// eof
