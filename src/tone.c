/*!
    \file
    \brief flipflip's Arduino Uno stuff: tone and melody generator (see \ref TONE)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup TONE
    @{

    \todo verify if everything is accurate
*/

#include <avr/io.h>        // avr: AVR device-specific IO definitions
#include <avr/interrupt.h> // avr: interrupt things

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "tone.h"          // ff: tone and melody generator

/* ************************************************************************** */

#if (FF_TONE_ENABLE > 0)

void toneInit(void)
{
    DEBUG("tone: init (pin "STRINGIFY(FF_TONE_PIN)")");
    PIN_OUTPUT(FF_TONE_PIN);
    PIN_LOW(FF_TONE_PIN);
}

#if (F_CPU != 16000000)
#  warning F_CPU not 16MHz. Tones will be wrong!
#endif

static volatile uint32_t svToneCount;
static const volatile uint16_t *pskvToneMelody;
static volatile bool svToneIsProgmem;
static volatile bool svToneIsSilent;

static void sToneArm(const uint16_t freq, const uint16_t dur)
{
    // stop Timer/Counter2
    CLRBITS(TIMSK2, BIT(OCIE2A));
    TCCR2B = 0;
    OCR2A = 0;

    // normal operation (OC0A, OC2A, OC2B disconnection, CTC mode)
    TCCR2A = BIT(WGM21);

    // calculate timer parameters

    // the number of time we have to toggle the pin to achieve the desired frequency:
    // formula from datasheet:
    //   freq = F_CPU / (2 * N * (1 + ocr))
    // we want to know the ocr value, and it must be <= 255, so we must choose a prescale factor (N) accordingly
    //   ocr = 16e6 / f / 2 / N - 1;
    // see tone.ods spreadsheet

    if (freq == TONE_PAUSE)
    {
        const uint8_t ocr = (F_CPU / 2 / 128) / 1000 - 1 / 1000;
        const uint32_t count = 2 * 1000 * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        svToneIsSilent = true;
        TCCR2B = BIT(CS22) | BIT(CS20);
    }
    else if ( (freq < 33) || (freq > 8000) )
    {
        WARNING("tone freq %"PRIu16" out of range", freq);
        return;
    }
    else if (freq > 4000) // N=8
    {
        const uint8_t ocr = (F_CPU / 2 / 8) / freq - 1;
        const uint32_t count = 2 * (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        svToneIsSilent = false;
        TCCR2B = BIT(CS21);
    }
    else if (freq > 3000) // N=32
    {
        const uint8_t ocr = (F_CPU / 2 / 32) / freq - 1;
        const uint32_t count = 2 * (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        svToneIsSilent = false;
        TCCR2B = BIT(CS21) | BIT(CS20);
    }
    else if (freq > 2000) // N=64
    {
        const uint8_t ocr = (F_CPU / 2 / 64) / freq - 1;
        const uint32_t count = 2 * (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        svToneIsSilent = false;
        TCCR2B = BIT(CS22);
    }
    else if (freq > 500) // N=128
    {
        const uint8_t ocr = (F_CPU / 2 / 128) / freq - 1 / 1000;
        const uint32_t count = 2 * (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        svToneIsSilent = false;
        TCCR2B = BIT(CS22) | BIT(CS20);
    }
    else // if (freq > 35) // N=1024
    {
        const uint8_t ocr = (F_CPU / 2 / 1024) / freq - 1 / 1000;
        const uint32_t count = 2 * (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        svToneIsSilent = false;
        TCCR2B = BIT(CS22) | BIT(CS21) | BIT(CS20);
    }

    // enable interrupt, start tone generation
    TIMSK2 = BIT(OCIE2A);

}

ISR(TIMER2_COMPA_vect)
{
    osIsrEnter();

    svToneCount--;
    if (svToneCount > 0)
    {
        if (!svToneIsSilent)
        {
            PIN_TOGGLE(FF_TONE_PIN);
        }
    }
    else
    {
        // single tone or melody done
        if ( (pskvToneMelody == NULL) ||
            ((svToneIsProgmem ? (uint16_t)pgm_read_word(pskvToneMelody) : *pskvToneMelody) == TONE_END) )
        {
            PIN_LOW(FF_TONE_PIN);
            CLRBITS(TIMSK2, BIT(OCIE2A));
            TCCR2B = 0;
            OCR2A = 0;
        }
        // next note in melody
        else
        {
            const uint16_t freq = svToneIsProgmem ? (uint16_t)pgm_read_word(pskvToneMelody) : *pskvToneMelody;
            pskvToneMelody++;
            const uint16_t dur  = svToneIsProgmem ? (uint16_t)pgm_read_word(pskvToneMelody) : *pskvToneMelody;
            pskvToneMelody++;
            sToneArm(freq, dur);
        }
    }

    osIsrLeave();
}

void toneGenerate(const uint16_t freq, const uint16_t dur)
{
    pskvToneMelody = NULL;
    svToneIsProgmem = false;
    sToneArm(freq, dur);
}

uint32_t toneMelody(const uint16_t *pkMelody, const bool isProgmem)
{
    uint32_t totalDuration = 0;

    const uint16_t *pkM = pkMelody;
    while ((isProgmem ? (uint16_t)pgm_read_word(pkM) : *pkM) != TONE_END)
    {
        pkM++;
        totalDuration += isProgmem ? (uint16_t)pgm_read_word(pkM) : *pkM;
        pkM++;
    }

    pskvToneMelody = pkMelody;
    svToneIsProgmem = isProgmem;

    svToneCount = 1;
    TIMSK2 = BIT(OCIE2A);
    //SETBITS(TIFR2, BIT(OCF2A)); // FIXME: should trigger interrupt, but doesn't, it seems, WTF?!

    // so we do this instead:
    TCCR2B = BIT(CS21);
    OCR2A = 1;

    return totalDuration;
}




#endif // (FF_TONE_ENABLE > 0)

/* ************************************************************************** */

//@}
// eof
