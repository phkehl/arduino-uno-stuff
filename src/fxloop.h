/*!
    \file
    \brief flipflip's Arduino Uno stuff: effect loops (see \ref FXLOOP)

    - Copyright (c) 2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \defgroup FXLOOP Effect Loops
    \ingroup FF

    This implements an effect loops system that allows to run loops of (e.g. LED) effects (while
    doing other stuff, too). It will run user-supplied effect functions at appropriate times.

    See \ref EXAMPLES_EX5 for usage instructions.

    \todo implement fx period adjustment (speed)

    @{
*/

#ifndef __FXLOOP_H__
#define __FXLOOP_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! effect function signature
/*!
    \param[in] frame  the frame number to render (0 is the first frame)
    \returns user return value, passed through to application via fxloopRun()
*/
typedef U2 (*FXLOOP_FUNC_t)(const U2 frame);

//! effect info
typedef struct FXLOOP_INFO_s
{
    CH            fxName[16];  //!< effect name
    FXLOOP_FUNC_t fxFunc;      //!< effect function
    U2            fxPeriod;    //!< update period (= 1 / refresh rate) [ms], how often to call the \c fxFunc
    U4            fxDuration;  //!< how long to play the effect [ms]
} FXLOOP_INFO_t;

//! fxloopRun() special meaning return value
#define FXLOOP_NEXT 0xffff

//! initialise effects loop
/*!
    \param[in] pkFxInfo  list of effects
    \param[in] nFxInfo   number of effects in the list
    \param[in] verbose   be verbose and print list of effects
*/
void fxloopInit(const FXLOOP_INFO_t *pkFxInfo, const uint16_t nFxInfo, const bool verbose);

//! run effect
/*!
    \param[in] forceNext  force transition to next effect
                          (after running the current effect one last time)

    \returns the return value of the effect function (#FXLOOP_FUNC_t),
        or #FXLOOP_NEXT if in transition to next effect
*/
U2 fxloopRun(const L forceNext);

//! wait until it's time for the next call to fxloopRun()
/*!
    \returns true if the effect will change with the next call to fxloopInit()
*/
L fxloopWait(void);

//! get effects loop status
/*!
    \param[in] str   string buffer
    \param[in] size  size of string buffer
*/
void fxloopStatus(char *str, const size_t size);


/* *************************************************************************** */

#endif // __FXLOOP_H__

//@}
// eof
