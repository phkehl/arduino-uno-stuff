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

void toneGenerate(const uint16_t freq, const uint16_t dur)
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

    if ( (freq < 33) || (freq > 8000) )
    {
        WARNING("tone freq %"PRIu16" out of range", freq);
        return;
    }
    else if (freq > 4000) // N=8
    {
        TCCR2B = BIT(CS21);
        const uint8_t ocr = (F_CPU / 2 / 8) / freq - 1;
        const uint32_t count = (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        //DEBUG("freq: %"PRIu16 " N=8 ocr=%"PRIu8" dur=%"PRIu16" count=%"PRIu32, freq, ocr, dur, count);
    }
    else if (freq > 3000) // N=32
    {
        TCCR2B = BIT(CS21) | BIT(CS20);
        const uint8_t ocr = (F_CPU / 2 / 32) / freq - 1;
        const uint32_t count = (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        //DEBUG("freq: %"PRIu16 " N=32 ocr=%"PRIu8" dur=%"PRIu16" count=%"PRIu32, freq, ocr, dur, count);
    }
    else if (freq > 2000) // N=64
    {
        TCCR2B = BIT(CS22);
        const uint8_t ocr = (F_CPU / 2 / 64) / freq - 1;
        const uint32_t count = (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        //DEBUG("freq: %"PRIu16 " N=64 ocr=%"PRIu8" dur=%"PRIu16" count=%"PRIu32, freq, ocr, dur, count);
    }
    else if (freq > 500) // N=128
    {
        TCCR2B = BIT(CS22) | BIT(CS20);
        const uint8_t ocr = (F_CPU / 2 / 128) / freq - 1 / 1000;
        const uint32_t count = (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        //DEBUG("freq: %"PRIu16 " N=128 ocr=%"PRIu8" dur=%"PRIu16" count=%"PRIu32, freq, ocr, dur, count);
    }
    else // if (freq > 35) // N=1024
    {
        TCCR2B = BIT(CS22) | BIT(CS21) | BIT(CS20);
        const uint8_t ocr = (F_CPU / 2 / 1024) / freq - 1 / 1000;
        const uint32_t count = (uint32_t)freq * (uint32_t)dur / 1000;
        OCR2A = ocr;
        svToneCount = count;
        //DEBUG("freq: %"PRIu16 " N=1024 ocr=%"PRIu8" dur=%"PRIu16" count=%"PRIu32, freq, ocr, dur, count);
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
        PIN_TOGGLE(FF_TONE_PIN);
    }
    else
    {
        PIN_LOW(FF_TONE_PIN);
        CLRBITS(TIMSK2, BIT(OCIE2A));
        TCCR2B = 0;
        OCR2A = 0;
    }

    osIsrLeave();
}

#endif // (FF_TONE_ENABLE > 0)

/* ************************************************************************** */

//@}
// eof
