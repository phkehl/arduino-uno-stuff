/*!
    \file
    \brief flipflip's Arduino Uno stuff: effect loops (see \ref FXLOOP)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup FXLOOP Effect Loops
    \ingroup FF

    This implements an effect loops system that allows to run loops of (e.g. LED) effects (while
    doing other stuff, too). It will run user-supplied effect functions at appropriate times.

    See \ref EXAMPLES_EX05 for a demonstration or \ref PROJECTS_FFMATRIX7X7 for an application.

    @{
*/

#ifndef __FXLOOP_H__
#define __FXLOOP_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! effect function signature
/*!
    \param[in] frame  the frame number to render (0 is the first frame, frames are skipped if a previous takes too long)
    \returns user return value, passed through to application via fxloopRun()
*/
typedef uint16_t (*FXLOOP_FUNC_t)(const uint16_t frame);

//! effect info (use FXLOOP_INFO() macro to initialise)
typedef struct FXLOOP_INFO_s
{
    char          fxName[16];  //!< effect name
    FXLOOP_FUNC_t fxFunc;      //!< effect function
    uint16_t      fxPeriodMin; //!< update period (= 1 / refresh rate) minimum [ms], how often to call the \c fxFunc
    uint16_t      fxPeriodMax; //!< update period (= 1 / refresh rate) maximum [ms], how often to call the \c fxFunc
    uint32_t      fxDuration;  //!< how long to play the effect [ms]
} FXLOOP_INFO_t;

//! create an effect info (#FXLOOP_INFO_t) entry \hideinitializer
/*!
    \param[in] name      the name of the effect (a string, e.g. \c "foobar")
    \param[in] func      the effect function (of type #FXLOOP_FUNC_t)
    \param[in] min       minimal update period of the effect [ms]
    \param[in] max       maximal update period of the effect [ms]
    \param[in] duration  duration of the effect [ms]
*/
#define FXLOOP_INFO(name, func, min, max, duration) \
    { .fxName = name "\0", .fxFunc = (func), .fxPeriodMin = (min), .fxPeriodMax = (max), .fxDuration = (duration) }

//! initialise effects loop
/*!
    \param[in] pkFxInfo  list of effects
    \param[in] nFxInfo   number of effects in the list
    \param[in] verbose   be verbose and print list of effects
*/
void fxloopInit(const FXLOOP_INFO_t *pkFxInfo, const uint16_t nFxInfo, const bool verbose);

//! run effect (render next frame)
/*!
    \param[in] forceNext  force transition to next effect and return immediately

    \returns the return value of the effect function (#FXLOOP_FUNC_t)
*/
uint16_t fxloopRun(const bool forceNext);

//! wait until it's time for the next call to fxloopRun()
/*!
    \param[in]  speed  speed (0..100%, meaning slow..fast) by scaling of the min/max (fast/slow)
                       duration specified in #FXLOOP_INFO_t
    \returns true if the effect will change with the next call to fxloopInit()
*/
bool fxloopWait(uint8_t speed);

//! get currently playing effect
/*!
    \returns the number of the playing effect (= index into effect list + 1)
*/
uint8_t fxloopCurrentlyPlaying(void);

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
