/*!
    \file
    \brief flipflip's Arduino Uno stuff: Siemens DL2416T (see \ref PROJECTS_FFLEDTESTER_DL2416T)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER_DL2416T
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
#include <avr/pgmspace.h>  // avr-libc: program Space Utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "rotenc.h"        // ff: rotary encoder input

#include "dl2416t.h"

void dl2416tInit(void)
{
    PIN_OUTPUT(DL_D0_PIN);  PIN_LOW(DL_D0_PIN);
    PIN_OUTPUT(DL_D1_PIN);  PIN_LOW(DL_D1_PIN);
    PIN_OUTPUT(DL_D2_PIN);  PIN_LOW(DL_D2_PIN);
    PIN_OUTPUT(DL_D3_PIN);  PIN_LOW(DL_D3_PIN);
    PIN_OUTPUT(DL_D4_PIN);  PIN_LOW(DL_D4_PIN);
    PIN_OUTPUT(DL_D5_PIN);  PIN_LOW(DL_D5_PIN);
    PIN_OUTPUT(DL_D6_PIN);  PIN_LOW(DL_D6_PIN);
    PIN_OUTPUT(DL_A0_PIN);  PIN_LOW(DL_A0_PIN);
    PIN_OUTPUT(DL_A1_PIN);  PIN_LOW(DL_A1_PIN);
    PIN_OUTPUT(DL_WR_PIN);  PIN_HIGH(DL_WR_PIN);
    PIN_OUTPUT(DL_BL_PIN);  PIN_LOW(DL_BL_PIN);
}

void dl2416tWrite(const uint8_t pos, const char ch)
{
    // ASCII 0x20..0x5f=" !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_"
    if ( (pos > 3) || (ch < ' ') || (ch > '_') )
    {
        WARNING("dl2416tWrite(%"PRIu8", 0x%"PRIx8") illegal!", pos, ch);
    }
    //DEBUG("write %"PRIu8" '%c'", pos, ch);

    // set address
    const uint8_t addr = 3 - pos;
    if (addr & 0x01) { PIN_HIGH(DL_A0_PIN); } else { PIN_LOW(DL_A0_PIN); }
    if (addr & 0x02) { PIN_HIGH(DL_A1_PIN); } else { PIN_LOW(DL_A1_PIN); }

    // set data
    const uint8_t data = ch;
    if (data & 0x01) { PIN_HIGH(DL_D0_PIN); } else { PIN_LOW(DL_D0_PIN); }
    if (data & 0x02) { PIN_HIGH(DL_D1_PIN); } else { PIN_LOW(DL_D1_PIN); }
    if (data & 0x04) { PIN_HIGH(DL_D2_PIN); } else { PIN_LOW(DL_D2_PIN); }
    if (data & 0x08) { PIN_HIGH(DL_D3_PIN); } else { PIN_LOW(DL_D3_PIN); }
    if (data & 0x10) { PIN_HIGH(DL_D4_PIN); } else { PIN_LOW(DL_D4_PIN); }
    if (data & 0x20) { PIN_HIGH(DL_D5_PIN); } else { PIN_LOW(DL_D5_PIN); }
    if (data & 0x40) { PIN_HIGH(DL_D6_PIN); } else { PIN_LOW(DL_D6_PIN); }

    // write
    PIN_LOW(DL_WR_PIN); // WR ena
    osTaskDelay(2);
    PIN_HIGH(DL_WR_PIN); // WR dis

    PIN_LOW(DL_D0_PIN);
    PIN_LOW(DL_D1_PIN);
    PIN_LOW(DL_D2_PIN);
    PIN_LOW(DL_D3_PIN);
    PIN_LOW(DL_D4_PIN);
    PIN_LOW(DL_D5_PIN);
    PIN_LOW(DL_D6_PIN);
    PIN_LOW(DL_A0_PIN);
    PIN_LOW(DL_A1_PIN);
}

void dl2416tClear(void)
{
    dl2416tBlank(true);
    dl2416tWrite(0, ' ');
    dl2416tWrite(1, ' ');
    dl2416tWrite(2, ' ');
    dl2416tWrite(3, ' ');
    dl2416tBlank(false);
}

void dl2416tBlank(const bool blank)
{
    if (blank)
    {
        PIN_LOW(DL_BL_PIN);
    }
    else
    {
        PIN_HIGH(DL_BL_PIN);
    }
}

void dl2416tBlink(const uint8_t times, const uint32_t tOff, const uint32_t tOn)
{
    for (uint8_t n = 0; n < times; n++)
    {
        dl2416tBlank(true);
        osTaskDelay(tOff);
        dl2416tBlank(false);
        osTaskDelay(tOn);
    }
}

void dl2416tStr_P(const char *str, const uint8_t offs, const uint8_t nChars)
{
    if (offs > 3)
    {
        return;
    }
    const uint8_t len = strlen_P(str);
    const uint8_t num = nChars > 0 ? MIN(nChars, len) : MIN(4, len);
    //DEBUG("str '%S' offs=%"PRIu8" nChars=%"PRIu8" len=%"PRIu8" num=%"PRIu8, str, offs, nChars, len, num);
    uint8_t ch = 0;
    for (uint8_t ix = offs; (ch < num) && (ix < 4); ix++, ch++)
    {
        dl2416tWrite(ix, pgm_read_byte(&str[ch]));
    }
    dl2416tBlank(false);
}

void dl2416tStrScroll_P(const char *str, const uint32_t delay)
{
    const uint8_t len = strlen_P(str);
    if (len < 4)
    {
        dl2416tStr_P(str, 0, 0);
    }
    else
    {
        for (uint8_t offs = 0; offs < (len - 3); offs++)
        {
            dl2416tStr_P(&str[offs], 0, 0);
            //osTaskDelay(delay);
            if (rotencGetEvent(delay) == ROTENC_BTN)
            {
                return;
            }
        }
    }
}

void dl2416tUnsigned(const uint16_t n, const uint8_t offs, const uint8_t nDigits)
{
    if ( (nDigits < 1) || (offs > 3) )
    {
        return;
    }
    uint16_t t = n;
    uint8_t ix = 3 - offs + 1;

    if ( (ix > 0) && (nDigits > 0) )
    {
        ix--;
        dl2416tWrite(ix, '0' + (t % 10));
    }

    if ( (ix > 0) && (nDigits > 1) )
    {
        ix--;
        t /= 10;
        dl2416tWrite(ix, '0' + (t % 10));
    }

    if ( (ix > 0) && (nDigits > 2) )
    {
        ix--;
        t /= 10;
        dl2416tWrite(ix, '0' + (t % 10));
    }

    if ( (ix > 0) && (nDigits > 3) )
    {
        ix--;
        t /= 10;
        dl2416tWrite(ix, '0' + (t % 10));
    }

    dl2416tBlank(false);
}

//@}
// eof
