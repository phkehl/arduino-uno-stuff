/*!
    \file
    \brief flipflip's Arduino Uno stuff: hardware abstraction (see \ref HW)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

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

static char sHwTxBuf[FF_HW_TX_BUFSIZE]; //!< output buffer
static uint8_t sHwTxBufHead;               //!< write-to-buffer pointer (index)
static uint8_t sHwTxBufTail;               //!< read-from-buffer pointer (index)
static uint8_t sHwTxBufSize;               //!< size of buffered data
static uint8_t sHwTxBufPeak;               //!< peak output buffer size
static uint16_t  sHwTxBufDrop;               //!< number of dropped bytes

static uint8_t sHwTxFlush(void)
{
    // send queued characters
    if (sHwTxBufSize)
    {
        // character to send
        const char c = /*~*/sHwTxBuf[sHwTxBufTail];

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
static int16_t sHwOutputPutChar(char c, FILE *pFile)
{
    int16_t res = 1;
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

static volatile char svHwRxBuf[FF_HW_RX_BUFSIZE]; //!< data rx buffer
static volatile uint8_t svHwRxBufHead;               //!< write-to-buffer pointer (index)
static volatile uint8_t svHwRxBufTail;               //!< read-from-buffer pointer (index)
static volatile uint8_t svHwRxBufSize;               //!< size of buffered data
static volatile uint8_t svHwRxBufPeak;               //!< peak input buffer size
static volatile uint16_t  svHwRxBufDrop;               //!< number of dropped bytes

static OS_SEMAPHORE_t sHwRxReadySem;

//! reads a character from the rx buffer
/*
    Returns the next character from the rx buffer, possibly waiting until a
    character has been received.

    \note This is used by the stdio subsystem, (printf() et al.).

    \param pFile  the file handle (not actually used)

    \returns  the character
*/
static char sHwInputGetChar(FILE *pFile)
{
    UNUSED(pFile);

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

    char c;

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

    const char c = UDR0; // always read this or this ISR will fire continuously
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

inline uint8_t hwGetRxBufSize(const uint32_t timeout)
{
    if (svHwRxBufSize == 0)
    {
        osSemaphoreTake(&sHwRxReadySem, timeout);
    }
    return svHwRxBufSize;
}

char hwReadNextChar(void)
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
inline uint8_t hwGetRxBufSize(const uint32_t timeout) { UNUSED(timeout); return 0; }
char hwReadNextChar(void) { return '\0'; }
static void sHwRxInit(void) { }
#endif // (FF_HW_RX_BUFSIZE > 0)


/* ***** debug signal LEDs ************************************************** */

#if (PIN_TO_BIT(FF_HW_TICK_PIN) != PIN_BIT_NONE)
static void sHwLedTickInit(void)
{
    PIN_OUTPUT(FF_HW_TICK_PIN);
    PIN_LOW(FF_HW_TICK_PIN);
}

__FORCEINLINE void hwLedTickToggle(void)
{
    PIN_TOGGLE(FF_HW_TICK_PIN);
}

#else
 void sHwLedTickInit(void) { }
__FORCEINLINE void hwLedTickToggle(void) { }
#endif

#if (PIN_TO_BIT(FF_HW_LOAD_PIN) != PIN_BIT_NONE)

static void sHwLedLoadInit(void)
{
    PIN_OUTPUT(FF_HW_LOAD_PIN);
    PIN_LOW(FF_HW_LOAD_PIN);
}

__FORCEINLINE void hwLedLoadOn(void)
{
    PIN_HIGH(FF_HW_LOAD_PIN);
}

__FORCEINLINE void hwLedLoadOff(void)
{
    PIN_LOW(FF_HW_LOAD_PIN);
}

#else
static void sHwLedLoadInit(void) { }
inline void hwLedLoadOn(void) { }
inline void hwLedLoadOff(void) { }
#endif


/* ***** exception handling & reset debugging ******************************* */

const char skHwPanicStr0[] PROGMEM = "NONE";
const char skHwPanicStr1[] PROGMEM = "HW";
const char skHwPanicStr2[] PROGMEM = "OS";
const char skHwPanicStr3[] PROGMEM = "OTHER";

const PGM_P const skHwPanicStr[] PROGMEM =
{
    skHwPanicStr0, skHwPanicStr1, skHwPanicStr2, skHwPanicStr3
};

void hwPanic(const HW_PANIC_t reason, const uint32_t u0, const uint32_t u1)
{
    cli();
    osTaskSuspendScheduler();
    wdt_reset();
    wdt_disable();

    uint16_t n = 0, time = 0;
    uint16_t msss = osTaskGetTicks();

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
        ERROR("PANIC @ %"PRIu32": 0x%"PRIx16" %S (0x%08"PRIx32", 0x%08"PRIx32")",
              (uint32_t)msss, (uint16_t)reason,
              (PGM_P)pgm_read_word(&skHwPanicStr[reason]), u0, u1);


        sHwLedLoadInit();
        uint32_t timeLeft = time ? time : 1;
        const uint32_t period = (n * 400) + (5 * 400);
        while (timeLeft)
        {
            int16_t i = n;
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
    const uint8_t low  = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);

    // FUSE_BOOTRST, FUSE_BOOTSZ0, FUSE_BOOTSZ1, FUSE_EESAVE, FUSE_WDTON, FUSE_SPIEN, FUSE_DWEN, FUSE_RSTDISBL
    const uint8_t high = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);

    // FUSE_BODLEVEL0, FUSE_BODLEVEL1, FUSE_BODLEVEL2
    const uint8_t ext  = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);

    // there are no definitions for these in the header files it seems
    const uint8_t lock = boot_lock_fuse_bits_get(GET_LOCK_BITS);

    DEBUG("hw: fuses: low=0x%02"PRIx8" high=0x%02"PRIx8" ext=0x%02"PRIx8" lock=0x%02"PRIx8,
        low, high, ext, lock);

    // [AVR8271, chapter 27.3 "Signature Bytes"]
    //DEBUG("hw: signature: 0x%02"PRIx8" 0x%02"PRIx8" 0x%02"PRIx8, __signature[2], __signature[1], __signature[0]);
    const uint8_t sig0 = boot_signature_byte_get(SIGNATURE_0);
    const uint8_t sig1 = boot_signature_byte_get(SIGNATURE_1);
    const uint8_t sig2 = boot_signature_byte_get(SIGNATURE_2);
    DEBUG("hw: signature: 0x%02"PRIx8" 0x%02"PRIx8" 0x%02"PRIx8, sig0, sig1, sig2);
}

static void sHwCheckResetCause(void)
{
    // cannot check reset cause if we're using a boot loader,
    // i.e. if the boot reset vector is enabled (FUSE_BOOTRST=0)
    //const uint8_t high = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
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

__FORCEINLINE void hwAssertWatchdog(void)
{
    wdt_reset();
}


/* ***** utility functions ************************************************** */

void hwDelay(const uint32_t usDelay)
{
    CS_ENTER;

    uint32_t d = usDelay;
    while (d >= 10)
    {
        // the argument to _delay_us() must be a compile-time constant
        _delay_us(10);
        d -= 10;
    }

    CS_LEAVE;
}

static volatile uint32_t sHwRandomSeed = 0;

ISR(WDT_vect)
{
    sHwRandomSeed <<= 8;
    sHwRandomSeed ^= (uint32_t)TCNT1;
}

uint32_t hwGetRandomSeed(void)
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
    const uint32_t seed = sHwRandomSeed; // this should now be fairly random
    DEBUG("hw: seed 0x%08"PRIx32, seed);
    return seed;
}


#if (FF_HW_NUM_TICTOC > 0)

#define HW_TICTOC_FREQ (F_CPU/1024)
#define HW_NUM_TICTOC_COUNTERS 4
static uint16_t sHwTicTocRegs[FF_HW_NUM_TICTOC];

static void sHwTicTocInit(void)
{
    // setup TIC/TOC performance counter
    TCCR1A = 0;         // normal mode
    TCCR1B = BIT(CS12) | BIT(CS10); // prescale clk/1024
    TCCR1C = 0;
}

void hwTic(const uint16_t reg)
{
    if (reg < FF_HW_NUM_TICTOC)
    {
        sHwTicTocRegs[reg] = (uint16_t)TCNT1;
    }
}

uint16_t hwToc(const uint16_t reg)
{
    uint16_t res = 0;
    if (reg < FF_HW_NUM_TICTOC)
    {
        const uint16_t delta = (uint16_t)TCNT1 - sHwTicTocRegs[reg];
        res = ((uint32_t)delta * 1000 * 10) / (uint32_t)HW_TICTOC_FREQ;
    }
    return res;
}

#else
static void sHwTicTocInit(void) { }
void hwTic(const uint16_t reg) { UNUSED(reg); }
uint16_t hwToc(const uint16_t reg) { UNUSED(reg); return 0; }
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

void hwAdcInit(const HW_ADC_t pins, const bool useAref)
{
    // setup selected pin(s) for input
    uint8_t didr = 0;
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

int32_t hwAdcGetScaled(const HW_ADC_t pin, const int32_t min, const int32_t max)
{
    // select ADC channel
    uint8_t mux = 0;
    switch (pin)
    {
        //case HW_ADC_A0:
        case HW_ADC_PC0: mux = 0         | 0         | 0        ; break;
        //case HW_ADC_A1:
        case HW_ADC_PC1: mux = 0         | 0         | BIT(MUX0); break;
        //case HW_ADC_A2:
        case HW_ADC_PC2: mux = 0         | BIT(MUX1) | 0        ; break;
        //case HW_ADC_A3:
        case HW_ADC_PC3: mux = 0         | BIT(MUX1) | BIT(MUX0); break;
        //case HW_ADC_A4:
        case HW_ADC_PC4: mux = BIT(MUX2) | 0         | 0        ; break;
        //case HW_ADC_A5:
        case HW_ADC_PC5: mux = BIT(MUX2) | 0         | BIT(MUX0); break;
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
            return (int32_t)ADC;
        }
        else
        {
            return (((int32_t)ADC * (max - min)) / ((int32_t)1023)) + min;
        }
    }
    else
    {
        return min;
    }
}


/* **** fast (or not) math functions **************************************** */

__FORCEINLINE float hwMathFastCosf(const float phi)
{
    // TODO: implement
    return cosf(phi);
}


__FORCEINLINE float hwMathFastSinf(const float phi)
{
    // TODO: implement
    return sinf(phi);
}


__FORCEINLINE float hwMathFastSqrtf(const float x)
{
    // TODO: implement
    return sqrtf(x);
}

__FORCEINLINE void hwMathSeedRandom(const uint32_t seed)
{
    srandom(seed);
}

__FORCEINLINE uint32_t hwMathGetRandom(void)
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

    DEBUG("hw: init (rx %"PRIu8", tx %"PRIu8")", FF_HW_RX_BUFSIZE, FF_HW_TX_BUFSIZE);

    sHwCheckResetCause();
    sHwDebugFuses();

    sHwLedTickInit();
    sHwLedLoadInit();

    sHwTicTocInit();
}

void hwStatus(char *str, const uint16_t size)
{
#if ( (FF_HW_RX_BUFSIZE > 0) && (FF_HW_TX_BUFSIZE > 0) )
    snprintf_P(str, size,
        PSTR("rxbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16") txbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16")"),
        svHwRxBufSize, svHwRxBufPeak, sizeof(svHwRxBuf), svHwRxBufDrop,
        sHwTxBufSize, sHwTxBufPeak, sizeof(sHwTxBuf), sHwTxBufDrop);
    svHwRxBufPeak = 0;
    sHwTxBufPeak = 0;
    sHwTxBufDrop = 0;
#elif (FF_HW_RX_BUFSIZE > 0)
    snprintf_P(str, size,
        PSTR("rxbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16")"),
        svHwRxBufSize, svHwRxBufPeak, sizeof(svHwRxBuf), svHwRxBufDrop);
    svHwRxBufPeak = 0;
    svHwRxBufDrop = 0;
#elif (FF_HW_TX_BUFSIZE > 0)
    snprintf_P(str, size,
        PSTR("txbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16")"),
        sHwTxBufSize, sHwTxBufPeak, sizeof(sHwTxBuf), sHwTxBufDrop);
    sHwTxBufPeak = 0;
    sHwTxBufDrop = 0;
#endif
    str[size-1] = '\0';
}

/* ************************************************************************** */
//@}
// eof
