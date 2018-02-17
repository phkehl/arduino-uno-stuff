/*!
    \file
    \brief flipflip's Arduino Uno stuff: hardware abstraction (see \ref HW)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup HW
    @{
*/

#include <stdio.h>         // libc: standard buffered input/output
#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions
#include <stdlib.h>        // libc: general utilities

#include <avr/io.h>        // avr: AVR device-specific IO definitions
#include <util/delay.h>    // avr: convenience functions for busy-wait delay loops
#include <avr/wdt.h>       // avr: watchdog timer handling
#include <avr/pgmspace.h>  // avr: program space utilities
#include <avr/interrupt.h> // avr: interrupt things
#include <avr/boot.h>      // avr: boot loader support
//#include <avr/signature.h> // avr: signature support

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system
#include "hw.h"            // ff: hardware



/* ***** serial port input and output *************************************** */

#if ( (FF_HW_RX_BUFSIZE > 0) && (FF_HW_RX_BUFSIZE < 50) )
#  error FF_HW_RX_BUFSIZE must be >= 50 bytes or it will not be useful.
#endif
#if (FF_HW_RX_BUFSIZE > 255)
#  error FF_HW_RX_BUFSIZE must be <= 255.
#endif

#if ( (FF_HW_TX_BUFSIZE > 0) && (FF_HW_TX_BUFSIZE < 50) )
#  error FF_HW_TX_BUFSIZE must be >= 50 bytes or it will not be useful.
#endif
#if (FF_HW_TX_BUFSIZE > 255)
#  error FF_HW_TX_BUFSIZE must be <= 255.
#endif

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

#if (FF_HW_TX_BUFSIZE > 0)

static volatile char      svHwTxBuf[FF_HW_TX_BUFSIZE]; // output buffer
static volatile uint8_t   svHwTxBufHead;               // write-to-buffer pointer (index)
static volatile uint8_t   svHwTxBufTail;               // read-from-buffer pointer (index)
static volatile uint8_t   svHwTxBufSize;               // size of buffered data
static volatile uint8_t   svHwTxBufPeak;               // peak output buffer size
static volatile uint16_t  svHwTxBufDrop;               // number of dropped bytes

void hwTxWaitEmpty(void)
{
    if (osTaskIsSchedulerRunning())
    {
        while (svHwTxBufSize != 0)
        {
            osTaskDelay(1);
        }
    }
    else
    {
        while (svHwTxBufSize != 0)
        {
            // busy-wait
        }
    }
}

// adds a character to the tx buffer
static int16_t sHwOutputPutChar(char c, FILE *pFile)
{
    int16_t res = 1;
    UNUSED(pFile);

    // guarantee output if the scheduler is not running
    if (!osTaskIsSchedulerRunning())
    {
        // we can use the buffer if interrupts are enabled
        if (SREG & BIT(7))
        {
            while (svHwTxBufSize == sizeof(svHwTxBuf))
            {
                // busy-wait
            }
        }
        // we must send right away if interrupts are not enabled
        else
        {
            CLRBITS(UCSR0B, BIT(UDRIE0));
            loop_until_bit_is_set(UCSR0A, UDRE0);
            UDR0 = c;
            return 0;
        }
    }
    // FIXME: and what when IRQs are off and the scheduler is running?

    CS_ENTER;

    // add to buffer
    if ( (svHwTxBufSize == 0) || (svHwTxBufHead != svHwTxBufTail) )
    {
        svHwTxBuf[svHwTxBufHead] = c;
        svHwTxBufHead += 1;
        svHwTxBufHead %= sizeof(svHwTxBuf);
        svHwTxBufSize++;
        if (svHwTxBufSize > svHwTxBufPeak)
        {
            svHwTxBufPeak = svHwTxBufSize;
        }
        res = 0;
        SETBITS(UCSR0B, BIT(UDRIE0));
    }
    // drop the char if the buffer is full
    else
    {
        svHwTxBufDrop++;
    }

    CS_LEAVE;

    return res;
}

// the output file handle (write-only)
static FILE sHwOutputDev = FDEV_SETUP_STREAM(sHwOutputPutChar, NULL, _FDEV_SETUP_WRITE);

#if defined(__AVR_ATmega328P__)
ISR(USART_UDRE_vect)
#elif defined(__AVR_ATmega2560__)
ISR(USART0_UDRE_vect)
#else
#  error Ouch!
#endif
{
    osIsrEnter();

    // load next char
    if (svHwTxBufSize != 0) // (svHwTxBufHead != svHwTxBufTail)
    {
        const char c = /*~*/svHwTxBuf[svHwTxBufTail];
        svHwTxBufTail += 1;
        svHwTxBufTail %= sizeof(svHwTxBuf);
        svHwTxBufSize--;
        UDR0 = c;
    }
    else
    {
        CLRBITS(UCSR0B, BIT(UDRIE0));
    }

    osIsrLeave();
}

static void sHwTxInit(void)
{
    // setup tx debugging
    SETBITS(DDRD, BIT(PD1)); // TXD output
    SETBITS(PORTD, BIT(PD1));

    // initialise output
    stdout = &sHwOutputDev;         // assign the debug port to stdout
    svHwTxBufHead = 0;           // initialise the output buffer
    svHwTxBufTail = 0;
    svHwTxBufSize = 0;
    svHwTxBufPeak = 0;
    svHwTxBufDrop = 0;
}

#else
static void sHwTxInit(void) { }
void hwTxWaitEmpty(void) { }
#endif // (FF_HW_TX_BUFSIZE > 0)

//------------------------------------------------------------------------------

#if (FF_HW_RX_BUFSIZE > 0)

static volatile char      svHwRxBuf[FF_HW_RX_BUFSIZE]; // data rx buffer
static volatile uint8_t   svHwRxBufHead;               // write-to-buffer pointer (index)
static volatile uint8_t   svHwRxBufTail;               // read-from-buffer pointer (index)
static volatile uint8_t   svHwRxBufSize;               // size of buffered data
static volatile uint8_t   svHwRxBufPeak;               // peak input buffer size
static volatile uint16_t  svHwRxBufDrop;               // number of dropped bytes

static OS_SEMAPHORE_t sHwRxReadySem;

// reads a character from the rx buffer
static int16_t sHwInputGetChar(FILE *pFile)
{
    UNUSED(pFile);

    // wait until character is available
    while (svHwRxBufSize == 0)
    {
        // sleep
        if (osTaskIsSchedulerRunning())
        {
            osTaskDelay(1);
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

// the input file handle (read-only)
static FILE sHwInputDev = FDEV_SETUP_STREAM(NULL, sHwInputGetChar, _FDEV_SETUP_READ);

#if defined(__AVR_ATmega328P__)
ISR(USART_RX_vect)
#elif defined(__AVR_ATmega2560__)
ISR(USART0_RX_vect)
#else
#  error Ouch!
#endif
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

    osSemaphoreGive(&sHwRxReadySem, true);

    osIsrLeave();
}

__INLINE uint8_t hwGetRxBufSize(const int32_t timeout)
{
    if (svHwRxBufSize == 0)
    {
        osSemaphoreTake(&sHwRxReadySem, timeout);
    }
    return svHwRxBufSize;
}

__FORCEINLINE char hwReadNextChar(void)
{
    //return fgetc(stdin);
    return sHwInputGetChar(NULL);
}

static void sHwRxInit(void)
{
    // setup receive pin
#  if defined(__AVR_ATmega328P__)
    PIN_INPUT(_PD0);
    PIN_PULLUP_ON(_PD0);
#  elif defined(__AVR_ATmega2560__)
    PIN_INPUT(_PE0);
    PIN_PULLUP_ON(_PE0);
#  else
#    error Ouch!
#  endif

    // initialise input buffer
    stdin = &sHwInputDev;          // assign the input port to stdin
    svHwRxBufHead = 0;          // initialise the output buffer
    svHwRxBufTail = 0;
    svHwRxBufSize = 0;
    svHwRxBufPeak = 0;
    svHwRxBufDrop = 0;

    // input sync
    osSemaphoreCreate(&sHwRxReadySem, 0);

    // enable RX complete interrupt
#if (FF_HW_RX_BUFSIZE > 0)
    SETBITS(UCSR0B, BIT(RXCIE0));
#endif
}

#else
__INLINE uint8_t hwGetRxBufSize(const int32_t timeout) { UNUSED(timeout); return 0; }
char hwReadNextChar(void) { return '\0'; }
static void sHwRxInit(void) { }
#endif // (FF_HW_RX_BUFSIZE > 0)


/* ***** debug signal LEDs ************************************************** */

#if (!PIN_IS_SAME(FF_HW_TICK_PIN, _NONE))
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
static void sHwLedTickInit(void) { }
__FORCEINLINE void hwLedTickToggle(void) { }
#endif

#if (!PIN_IS_SAME(FF_HW_LOAD_PIN, _NONE))
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
__FORCEINLINE void hwLedLoadOn(void) { }
__FORCEINLINE void hwLedLoadOff(void) { }
#endif


/* ***** exception handling & reset debugging ******************************* */

#ifndef __DOXYGEN__ // STFU
const const char skHwPanicStr[][6] PROGMEM =
{
    { "NONE\0" }, { "HW\0" }, { "OS\0" }, { "OTHER\0" }
};
#endif

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
        // FIXME: something's not working sometimes here.. :-(

        DEBUG(":-(");
        ERROR("PANIC @ %"PRIu32": 0x%"PRIx16" %S (0x%08"PRIx32", 0x%08"PRIx32")",
              (uint32_t)msss, (uint16_t)reason, skHwPanicStr[reason], u0, u1);

        sHwLedLoadInit();
        uint32_t timeLeft = time ? time : 1;
        const uint32_t period = (n * 400) + 1000;
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
            _delay_ms(500);
            _delay_ms(500);

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
            wdt_reset();
        }

        ERROR("RESET");

        //void(* reset) (void) = 0;
        //reset();

        hwReset(HW_RESET_HARD);
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
// s.a. wdt_init()

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

#if (FF_HW_NUM_TICTOC > 0)

//#define HW_TICTOC_FREQ (F_CPU/1024)
#define HW_TICTOC_FREQ (F_CPU/8)

static uint16_t sHwTicTocRegs[FF_HW_NUM_TICTOC];

static void sHwTicTocInit(void)
{
    // setup tic/toc performance counter,
    // using 16bit Timer/Counter1

    // normal operation, OC1A/OC1B disconnected.
    TCCR1A = 0;
    // no PWM stuff
    TCCR1C = 0;

    // 16_000_000 / 1024 = 15625 clk/step
    // "1 TC step" = 0.064ms, 65536 steps = 4194ms
    //TCCR1B = BIT(CS12) | BIT(CS10); // prescale clk/1024

    // 16_000_000 / 1 = 16_000_000 clk/step
    // "1 TC step" = 0.0000625, 65536 steps = 4.096ms
    //TCCR1B = BIT(CS10);

    // 16_000_000 / 8 = 2_000_000 clk/step
    // "1 TC step" = 0.0005ms, 65536 steps = 32.768ms
    TCCR1B = BIT(CS11);
}

void hwTic(const uint8_t reg)
{
    if (reg < FF_HW_NUM_TICTOC)
    {
        sHwTicTocRegs[reg] = (uint16_t)TCNT1;
    }
}

uint16_t hwToc(const uint8_t reg)
{
    uint16_t res = 0;
    if (reg < FF_HW_NUM_TICTOC)
    {
        //const uint16_t delta = (uint16_t)TCNT1 - sHwTicTocRegs[reg];
        //res = ((uint32_t)delta * 1000 * 10) / (uint32_t)HW_TICTOC_FREQ;
        const uint16_t delta = (uint16_t)TCNT1 - sHwTicTocRegs[reg];
        res = delta / (HW_TICTOC_FREQ/1000/1000);
    }
    return res;
}

#else
static void sHwTicTocInit(void) { }
void hwTic(const uint8_t reg) { UNUSED(reg); }
uint16_t hwToc(const uint8_t reg) { UNUSED(reg); return 0; }
#endif // (FF_HW_NUM_TICTOC > 0)

void hwReset(const HW_RESET_t type)
{
    switch (type)
    {
        case HW_RESET_SOFT:
            asm volatile ("jmp 0");
            break;
        case HW_RESET_HARD:
            sei();
            wdt_enable(WDTO_15MS);
            while (ENDLESS)
            {
            }
            break;
    }
}

// disable watchdog early, we may have used it to do a hard reset
// (the bootload will have to do this in case we have one)
// s.a. get_mcusr()
void wdt_init(void) __NAKED __SECTION(.init3);
void wdt_init(void)
{
    wdt_disable();
}


/* ***** random number seeding ********************************************** */

#  if (FF_HW_RANDSEED == 1) || (FF_HW_RANDSEED == 99)
static volatile uint32_t sHwRandomSeed = 0;

ISR(WDT_vect)
{
    sHwRandomSeed <<= 8;
    sHwRandomSeed ^= (uint32_t)TCNT1;
}
#  endif // (FF_HW_RANDSEED == 1) || (FF_HW_RANDSEED == 99)

static void sHwRandomInit(void)
{
    uint32_t seed = 0;
#if (FF_HW_RANDSEED > 0)

    // get random seed using watchdog jitter
#  if (FF_HW_RANDSEED == 1) || (FF_HW_RANDSEED == 99)
    const uint8_t sreg = SREG;
    sei();

    // setup timer/counter 1
    TCCR1A = 0;
    TCCR1C = 0;
    TCCR1B = BIT(CS11);

    // temporarily disable the watchdog reset and use the
    // watchdog timer jitter to generate a better seed
    WDTCSR |= BIT(WDCE);    // allow changes
    WDTCSR = BIT(WDIE);     // set 16ms timer and enable interrupt (but not the reset)

    // wait a bit to fire the watchdog a few times
    _delay_us(100000);

    WDTCSR |= BIT(WDCE);    // disable the..
    WDTCSR = 0;             // ..watchdog
#if (FF_HW_USE_WATCHDOG > 0)
    wdt_enable(WDTO_8S);    // and enable the default watchdog again
#endif

    SREG = sreg;

    seed ^= sHwRandomSeed;
    DEBUG("hw: seed 0x%08"PRIx32" (wd jitter)", sHwRandomSeed);
#  endif // (FF_HW_RANDSEED == 1) || (FF_HW_RANDSEED == 99)

    // get random seed from RAM contents
#  if (FF_HW_RANDSEED == 2) || (FF_HW_RANDSEED == 99)
    uint32_t mSeed = 0;
    const uint8_t *pkAddr = (void *)RAMEND;
    while (pkAddr > (uint8_t *)0x0100)
    {
        mSeed += ((uint16_t)*pkAddr) << 6;
        pkAddr--;
    }
    seed ^= mSeed;
    DEBUG("hw: seed 0x%08"PRIx32" (RAM)", mSeed);
#  endif // (FF_HW_RANDSEED == 2) || (FF_HW_RANDSEED == 99)



#endif // (FF_HW_RANDSEED > 0)
    DEBUG("hw: seed 0x%08"PRIx32" ("STRINGIFY(FF_HW_RANDSEED)")", seed);
    srandom(seed);
}


/* **** analog to digital conversion (ADC) ********************************** */

static OS_SEMAPHORE_t sHwAdcReadySem;

ISR(ADC_vect) // ADC conversion complete
{
    osIsrEnter();
    // disable ADC
    CLRBITS(ADCSRA, BIT(ADEN));
    // signal the waiting task that the ADC conversion has finished
    osSemaphoreGive(&sHwAdcReadySem, true);
    osIsrLeave();
}

void hwAdcInit(const HW_ADC_t pins, const bool useAref)
{
    // setup selected pin(s) for input
    uint8_t didr = 0;
    if (pins & HW_ADC_A0)
    {
        PIN_INPUT(_A0);
        PIN_PULLUP_OFF(_A0);
        didr |= BIT(ADC0D);
    }
    if (pins & HW_ADC_A1)
    {
        PIN_INPUT(_A1);
        PIN_PULLUP_OFF(_A1);
        didr |= BIT(ADC1D);
    }
    if (pins & HW_ADC_A2)
    {
        PIN_INPUT(_A2);
        PIN_PULLUP_OFF(_A2);
        didr |= BIT(ADC2D);
    }
    if (pins & HW_ADC_A3)
    {
        PIN_INPUT(_A3);
        PIN_PULLUP_OFF(_A3);
        didr |= BIT(ADC3D);
    }
    if (pins & HW_ADC_A4)
    {
        PIN_INPUT(_A4);
        PIN_PULLUP_OFF(_A4);
        didr |= BIT(ADC4D);
    }
    if (pins & HW_ADC_A5)
    {
        PIN_INPUT(_A5);
        PIN_PULLUP_OFF(_A5);
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

uint16_t hwReadVcc(void)
{
    uint16_t res = 0;

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168P__)
    // save previous ADC peripheral state
    const uint8_t admux  = ADMUX & (BIT(REFS0) | BIT(REFS1) | BIT(ADLAR));
    const uint8_t adcsra = ADCSRA;

    ADMUX = BIT(REFS0) | BIT(MUX3) | BIT(MUX2) | BIT(MUX1);
    osTaskDelay(1);
    osSemaphoreCreate(&sHwAdcReadySem, 0);
    ADCSRA = BIT(ADPS2) | BIT(ADPS0);
    ADCSRA |= BIT(ADSC) | BIT(ADEN) | BIT(ADIE) | BIT(ADIF); // start conversion

    if (osSemaphoreTake(&sHwAdcReadySem, 2))
    {
        // ADC / 1.1V = 1024 / Vcc --> Vcc = 1.1V * 1024 / ADC
        const uint32_t adc = ADC;
        res = (uint16_t)( (uint32_t)1100 * (uint32_t)1024 / adc );
    }

    ADMUX  = admux;
    ADCSRA = adcsra;
#endif

    return res;
}

int8_t hwReadTemp(void)
{
    int8_t res = 0;

#if defined(__AVR_ATmega328P__)
    // save previous ADC peripheral state
    const uint8_t admux  = ADMUX & (BIT(REFS0) | BIT(REFS1) | BIT(ADLAR));
    const uint8_t adcsra = ADCSRA;

    ADMUX = BIT(REFS1) | BIT(REFS0) | BIT(MUX3);
    osTaskDelay(1);
    osSemaphoreCreate(&sHwAdcReadySem, 0);
    ADCSRA = BIT(ADPS2) | BIT(ADPS0);
    ADCSRA |= BIT(ADSC) | BIT(ADEN) | BIT(ADIE) | BIT(ADIF); // start conversion

    if (osSemaphoreTake(&sHwAdcReadySem, 2))
    {
        res = ADC - FF_HW_TEMP_OFFS;
    }

    ADMUX  = admux;
    ADCSRA = adcsra;
#endif

    return res;
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

    sHwRandomInit(); // before sHwTicTocInit()

    sHwTicTocInit();
}

void hwStatus(char *str, const uint16_t size)
{
    int n = 0;
#if ( (FF_HW_RX_BUFSIZE > 0) && (FF_HW_TX_BUFSIZE > 0) )
    n = snprintf_P(str, size,
        PSTR("rxbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16") txbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16")"),
        svHwRxBufSize, svHwRxBufPeak, sizeof(svHwRxBuf), svHwRxBufDrop,
        svHwTxBufSize, svHwTxBufPeak, sizeof(svHwTxBuf), svHwTxBufDrop);
    svHwRxBufPeak = 0;
    svHwRxBufDrop = 0;
    svHwTxBufPeak = 0;
    svHwTxBufDrop = 0;
#elif (FF_HW_RX_BUFSIZE > 0)
    n = snprintf_P(str, size,
        PSTR("rxbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16")"),
        svHwRxBufSize, svHwRxBufPeak, sizeof(svHwRxBuf), svHwRxBufDrop);
    svHwRxBufPeak = 0;
    svHwRxBufDrop = 0;
#elif (FF_HW_TX_BUFSIZE > 0)
    n = snprintf_P(str, size,
        PSTR("txbuf=%"PRIu8"/%"PRIu8"/%"PRIu8" (%"PRIu16")"),
        svHwTxBufSize, svHwTxBufPeak, sizeof(svHwTxBuf), svHwTxBufDrop);
    svHwTxBufPeak = 0;
    svHwTxBufDrop = 0;
#endif

#if (FF_HW_MEAS_VCC > 0)
    const uint32_t vcc = hwReadVcc();
    n += snprintf_P(&str[n], size - n, PSTR(" vcc=%"PRIu16), vcc);
#endif

#if (FF_HW_MEAS_TEMP > 0)
    const uint32_t temp = hwReadTemp();
    n += snprintf_P(&str[n], size - n, PSTR(" temp=%"PRIi8), temp);
#endif

#if (FF_HW_MEAS_VCC == 0) && (FF_HW_MEAS_TEMP == 0)
    UNUSED(n);
#endif

    str[size-1] = '\0';
}

/* ************************************************************************** */
//@}
// eof
