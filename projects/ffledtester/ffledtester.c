/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's LED tester (see \ref PROJECTS_FFLEDTESTER)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER

    \ref ARDUINOPINS usage:
    - D0: \ref ROTENC pin 2
    - D1: serial tx for \ref DEBUG output
    - D2: \ref ROTENC pin 1 ("CLK")
    - D3: \ref ROTENC pin 3 ("SW")
    - D4: Siemens DL2416T display: data input (D0)
    - D5: Siemens DL2416T display: data input (D1)
    - D6: Siemens DL2416T display: data input (D2)
    - D7: Siemens DL2416T display: data input (D3)
    - D8:
    - D9: \ref FF_HW_LOAD_PIN
    - D10 (SS): needed by \ref WS2801 (FIXME: really?)
    - D11 (MOSI): \ref WS2801 data ("DI")
    - D12: \ref WS2812 data pin
    - D13 (SCK): \ref WS2801 clock output ("CI")
    - A0: Siemens DL2416T display: data input (D4)
    - A1: Siemens DL2416T display: data input (D5)
    - A2: Siemens DL2416T display: data input (D6)
    - A3: Siemens DL2416T display: address (A0)
    - A4: Siemens DL2416T display: address (A1)
    - A5: Siemens DL2416T display: write (~WR)

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
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects
#include "rotenc.h"        // ff: rotary encoder input

#include "ffledtester.h"


/* ***** configuration ******************************************************* */

#define DL_D0_PIN   _D4
#define DL_D1_PIN   _D5
#define DL_D2_PIN   _D6
#define DL_D3_PIN   _D7
#define DL_D4_PIN   _A0
#define DL_D5_PIN   _A1
#define DL_D6_PIN   _A2
#define DL_A0_PIN   _A3
#define DL_A1_PIN   _A4
#define DL_WR_PIN   _A5
#define WS2812_PIN  _D12


/* ***** Siemens DL2416T functions ******************************************* */

static void dl2416tInit(void)
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
}

static void dl2416tWrite(const uint8_t pos, const char ch)
{
    if ( (pos > 3) || (ch < ' ') || (ch > '_') )
    {
        WARNING("dl2416tWrite(%"PRIu8", 0x%"PRIx8") illegal!", pos, ch);
    }

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
}

static void dl2416tStr(const char *str)
{
    const uint8_t len = strlen(str);
    for (uint8_t ix = 0; (ix < len) && (ix < 4); ix++)
    {
        dl2416tWrite(ix, str[ix]);
    }
}

static void dl2416tStr_P(const char *str)
{
    const uint8_t len = strlen_P(str);
    for (uint8_t ix = 0; (ix < len) && (ix < 4); ix++)
    {
        dl2416tWrite(ix, pgm_read_byte(&str[ix]));
    }
}

static void dl2416tUnsigned(const uint16_t n)
{
    uint16_t t = n;
    dl2416tWrite(3, '0' + (t % 10));
    t /= 10;
    dl2416tWrite(2, '0' + (t % 10));
    t /= 10;
    dl2416tWrite(1, '0' + (t % 10));
    t /= 10;
    dl2416tWrite(0, '0' + (t % 10));
}

/* ***** application functions *********************************************** */


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // clear event queue
    rotencClearEvents();

    dl2416tWrite(0, '0');
    dl2416tWrite(1, '1');
    dl2416tWrite(2, '2');
    dl2416tWrite(3, '3');
    osTaskDelay(1000);
    dl2416tStr("FOO!");
    osTaskDelay(1000);
    dl2416tStr_P(PSTR("BAR!"));
    osTaskDelay(1000);

    static bool update;
    static int16_t count;
    update = true;

    while (ENDLESS)
    {
        DEBUG("wait");
        const ROTENC_EVENT_t ev = rotencGetEvent(100);
        switch (ev)
        {
            case ROTENC_INC_DN:
                if (count < 9990)
                {
                    count += 9;
                }
                FALLTHROUGH;
            case ROTENC_INC:
                if (count < 9999)
                {
                    count++;
                }
                update = true;
                DEBUG("INC %"PRIu16, count);
                break;
            case ROTENC_DEC_DN:
                if (count >= 9)
                {
                    count -= 9;
                }
                FALLTHROUGH;
            case ROTENC_DEC:
                if (count >= 1)
                {
                    count--;
                }
                update = true;
                DEBUG("DEC %"PRIu16, count);
                break;
            case ROTENC_BTN:
                DEBUG("BTN");
                break;
            case ROTENC_BTN_LONG:
                DEBUG("LONG");
                break;
            case ROTENC_NONE:
                //DEBUG("NONE");
                break;
        }

        if (update)
        {
            //dl2416tUnsigned(count);
            update = false;
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("ffledtester..."));
}


/* ***** application init **************************************************** */

// initialise the user application
void appInit(void)
{
    DEBUG("ffledtester: init");

    rotencInit();

    ws2801Init();

    PIN_OUTPUT(WS2812_PIN); PIN_LOW(WS2812_PIN);

    dl2416tInit();

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

//------------------------------------------------------------------------------

//@}
// eof
