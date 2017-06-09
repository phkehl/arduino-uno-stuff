/*!
    \file
    \brief flipflip's Arduino Uno stuff: Adeunis ARF32 bluetooth module (see \ref PROJECTS_FFPHONE_ARF32)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFPHONE_ARF32 ARF32
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "arf32.h"


/* ***** init **************************************************************** */

static void sArf32Task(void *pArg);

void arf32Start(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("arf", 3, &task, stack, sizeof(stack), sArf32Task, NULL);
}




/* ***** task **************************************************************** */

static void sArf32Task(void *pArg)
{
    UNUSED(pArg);

    while (ENDLESS)
    {
        DEBUG("arf32... %"PRIu32, osTaskGetTicks());
        uint32_t n = 55555;
        while (n--)
        {

        }
        osTaskDelay(2345);
    }
}


/* ***** status ************************************************************** */

// make application status string
void arf32Status(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("arf32 status..."));
}


//@}
// eof
