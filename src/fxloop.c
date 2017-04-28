/*!
    \file
    \brief flipflip's Arduino Uno stuff: template (see \ref FXLOOP)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup FXLOOP
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/pgmspace.h>  // avr: program space utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "fxloop.h"        // ff: effects loops


/* ************************************************************************** */

// effects loop status
typedef struct STATUS_s
{
    uint8_t              loopIx;      // currently selected effect
    FXLOOP_FUNC_t        fxFunc;      // currently selected effect function
    uint32_t             duration;    // how long to play the effect
    uint32_t             runtime;     // how long the effect has been playing
    uint16_t             frame;       // frame counter
    uint16_t             frameDrop;   // frame drop counter
    uint16_t             period;      // refresh period
    uint32_t             msss;        // time of start of last frame
    const FXLOOP_INFO_t *fxInfo;
    uint8_t              numFx;
} STATUS_t;

static STATUS_t sStatus;

#define INFO_NAME(ix)       (/* STFU */const wchar_t *)&sStatus.fxInfo[ix].fxName
#define INFO_FUNC(ix)       (FXLOOP_FUNC_t)pgm_read_word(&sStatus.fxInfo[ix].fxFunc)
#define INFO_PERIOD_MIN(ix) (uint32_t)pgm_read_word(&sStatus.fxInfo[ix].fxPeriodMin)
#define INFO_PERIOD_MAX(ix) (uint32_t)pgm_read_word(&sStatus.fxInfo[ix].fxPeriodMax)
#define INFO_DURATION(ix)   (uint32_t)pgm_read_dword(&sStatus.fxInfo[ix].fxDuration)

void fxloopInit(const FXLOOP_INFO_t *pkFxInfo, const uint16_t nFxInfo, const bool verbose)
{
    memset(&sStatus, 0, sizeof(sStatus));

    sStatus.loopIx    = UINT8_MAX;
    sStatus.msss      = osTaskGetTicks();
    sStatus.numFx     = nFxInfo;
    sStatus.fxInfo    = pkFxInfo;

    if (verbose)
    {
        // print the effects that available
        for (uint8_t ix = 0; ix < sStatus.numFx; ix++)
        {
            const uint16_t periodMin = INFO_PERIOD_MIN(ix);
            const uint16_t periodMax = INFO_PERIOD_MAX(ix);
            const uint8_t hzMin = periodMin ? 1000 / periodMin : 0;
            const uint8_t hzMax = periodMax ? 1000 / periodMax : 0;
            PRINT_F("fxloop: %2"PRIu16" %-16S %5"PRIu32"ms @ %3"PRIu16"ms/%3"PRIu8"Hz .. %3"PRIu16"ms/%3"PRIu8"Hz",
                ix + 1, INFO_NAME(ix), INFO_DURATION(ix), periodMin, hzMin, periodMax, hzMax);
        }
    }
}

uint16_t fxloopRun(const bool forceNext)
{
    uint16_t funcRes = 0;

    // bring up next effect?
    if ( forceNext || (sStatus.runtime >= sStatus.duration) )
    {
        // find next effect
        sStatus.loopIx++;
        sStatus.loopIx %= sStatus.numFx;

        // initialise next effect..
        sStatus.fxFunc    = INFO_FUNC(sStatus.loopIx);
        sStatus.runtime   = 0;
        sStatus.frame     = 0;
        sStatus.frameDrop = 0;
        sStatus.duration  = INFO_DURATION(sStatus.loopIx);

        PRINT("fxloop: %"PRIu8"/%"PRIu8" %S %"PRIu32"ms",
            sStatus.loopIx + 1, sStatus.numFx, INFO_NAME(sStatus.loopIx), sStatus.duration);
    }

    // retrun if forced (so that this can be called multiple times)
    if (forceNext)
    {
        // ...nothing...
    }
    // play
    else
    {
        sStatus.msss = osTaskGetTicks();
        funcRes = sStatus.fxFunc(sStatus.frame);
        sStatus.frame++;
    }

    return funcRes;
}


bool fxloopWait(uint8_t speed)
{
    // target refresh rate (period in [ms])
    const uint16_t periodMin = INFO_PERIOD_MIN(sStatus.loopIx);
    const uint16_t periodMax = INFO_PERIOD_MAX(sStatus.loopIx);
    const uint16_t speed16 = CLIP(speed, 0, 100);
    const uint16_t period = (speed16 * (periodMax - periodMin) / (uint16_t)100) + periodMin;
    sStatus.period = period;
    //DEBUG("speed %"PRIu8" %"PRIu16" %"PRIu16" %"PRIu16" %"PRIu16,
    //    speed, speed16, periodMin, periodMax, period);

    // delay until next frame
    osTaskDelayUntil(&sStatus.msss, period);

    // update runtime
    sStatus.runtime += period;

    // if we were too late, drop one frame and sleep until next period
    const uint32_t msssNow = osTaskGetTicks();
    if ( (msssNow - sStatus.msss) > 10 ) // other tasks may schedule in before us so that we may be slightly late
    {
        const uint32_t dt = (msssNow - sStatus.msss);
        const uint16_t numDropped = (dt / period) + 1;
        WARNING("fxloop: %S %"PRIu16" frames dropped! %"PRIu16"+%"PRIu16"ms",
            INFO_NAME(sStatus.loopIx), numDropped, period, dt);
        sStatus.frameDrop += numDropped;
        sStatus.frame     += numDropped;
        sStatus.msss       = (msssNow / period) * period;
        osTaskDelayUntil(&sStatus.msss, period);
        sStatus.runtime   += period;
    }

    if (sStatus.runtime >= sStatus.duration)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t fxloopCurrentlyPlaying(void)
{
    return sStatus.loopIx + 1;
}

void fxloopStatus(char *str, const size_t size)
{
    const uint8_t hz = sStatus.period ? 1000 / sStatus.period : 0;
    snprintf_P(str, size, PSTR("fx#%"PRIu8" %S %"PRIu32"/%"PRIu32" %"PRIu16"/%"PRIu16" %"PRIu16"ms/%"PRIu8"Hz"),
        sStatus.loopIx + 1,
        sStatus.loopIx == UINT8_MAX ? (/* STFU */const wchar_t *)PSTR("---") : INFO_NAME(sStatus.loopIx),
        sStatus.runtime, sStatus.duration,
        sStatus.frameDrop, sStatus.frame,
        sStatus.period, hz);
    str[size-1] = '\0';

}


/* ************************************************************************** */

//@}
// eof
