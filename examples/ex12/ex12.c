/*!
    \file
    \brief flipflip's Arduino Uno stuff: twelfth example application (see \ref EXAMPLES_EX12)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX12
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "ex12.h"


/* ***** rotary encoder ****************************************************** */

#define ROTENC_PIN1 _PD2 // pin 1 (must be PD2 = INT0)
#define ROTENC_PIN2 _PD4 // pin 2

static OS_SEMAPHORE_t sRotEncSem;

typedef enum ROTENC_DIR_e
{
    ROTENC_INC,
    ROTENC_DEC,
} ROTENC_DIR_t;

static volatile ROTENC_DIR_t svRotEncDir;

static volatile uint32_t svRotEncLast;
static volatile uint16_t svRotEncDt;

ISR(INT0_vect) // external interrupt 0
{
    osIsrEnter();

    svRotEncDir = PIN_GET(ROTENC_PIN2) ? ROTENC_DEC : ROTENC_INC;
    const uint32_t msss = osTaskGetMsss();
    svRotEncDt = msss - svRotEncLast;
    svRotEncLast = msss;

    osSemaphoreGive(&sRotEncSem, true);

    osIsrLeave();
}


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex12: init");

    PIN_INPUT(ROTENC_PIN1);
    PIN_PULLUP_ON(ROTENC_PIN1);
    PIN_INPUT(ROTENC_PIN2);
    PIN_PULLUP_ON(ROTENC_PIN2);

    osSemaphoreCreate(&sRotEncSem, 0);

    // falling-edge triggers interrupt
    SETBITS(EICRA, BIT(ISC01));
    CLRBITS(EICRA, BIT(ISC00));
    SETBITS(EIMSK, BIT(INT0)); // enable INT0
    SETBITS(EIFR, BIT(INTF0)); // clear INT0

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

/* ***** application task **************************************************** */


// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    static int16_t count;

    while (ENDLESS)
    {
        if (osSemaphoreTake(&sRotEncSem, 2))
        {
            count += svRotEncDir == ROTENC_INC ? 1 : -1;

            PRINT("rotenc %+8"PRIi16" %6"PRIu32" %5"PRIu16, count, svRotEncLast, svRotEncDt);
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("status..."));
}


//@}
// eof
