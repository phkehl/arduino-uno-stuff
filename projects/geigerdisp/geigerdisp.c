/*!
    \file
    \brief flipflip's Arduino Uno stuff: Geiger Counter display (see \ref PROJECTS_GEIGERDISP)

    - Copyright (c) 2019 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_GEIGERDISP
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "geigerdisp.h"


static void sProcessGeigerLine(const char *line);

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    static char    lineBuf[100];
    static uint8_t lineIx;

    while (ENDLESS)
    {
        // read data from geiger counter
        const uint8_t nRx = hwGetRxBufSize(2000);
        if (nRx == 0)
        {
            WARNING("no data!");
            continue;
        }

        for (uint8_t rIx = 0; (rIx < nRx) && (lineIx < sizeof(lineBuf)); rIx++)
        {
            const char c = hwReadNextChar();
            // ignore
            if (c == '\r')
            {
            }
            // have complete line?
            else if (c == '\n')
            {
                lineBuf[lineIx++] = '\0';
                sProcessGeigerLine(lineBuf);
                lineBuf[0] = '\0';
                lineIx = 0;
            }
            // store char
            else
            {
                lineBuf[lineIx++] = c;
            }
        }
    }
}

static void sProcessGeigerLine(const char *line)
{
    const int len = strlen(line);
    if (len < 1)
    {
        WARNING("no line!");
        return;
    }
    DEBUG("%s", line);

    // comments
    if (line[0] == '#')
    {
        return;
    }

    // otherwise should be measurement like this:
    // CPS, 0, CPM, 0, uSv/hr, 0.00, SLOW
    uint16_t cps = 0;
    uint16_t cpm = 0;
    if (sscanf(line, "CPS, %"PRIu16", CPM, %"PRIu16, &cps, &cpm) == 2)
    {
        DEBUG("cps=%"PRIu16" cpm=%"PRIu16, cps, cpm);
    }
    else
    {
        WARNING("fishy measurement!");
    }
}



// initialise the user application
void appInit(void)
{
    DEBUG("geigerdisp: init");
}

// starts the user application tasks
void appCreateTask(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}



//@}
// eof
