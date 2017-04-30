/*!
    \file
    \brief flipflip's Arduino Uno stuff: GNSS receiver abstraction (see \ref GNSS)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup GNSS GNSS Receiver Abstraction
    \ingroup FF

    This implements a GNSS receiver abstraction.

    Configuration: #FF_GNSS_TASK_PRIO, #FF_GNSS_TASK_STACK and #FF_GNSS_PARSER configuration.

    See \ref EXAMPLES_EX6 for a demonstration of \ref PROJECTS_FFCLOCK for an application.

    @{
*/

#ifndef __GNSS_H__
#define __GNSS_H__


/* *************************************************************************** */

#include "stdstuff.h"      // ff: useful macros and types


//! start the GNSS task
void gnssStartTask(void);


//! make GNSS status string
/*!
    \param[out] str   the target string
    \param[in]  size  the maximum size of the string
*/
void gnssStatus(char *str, const uint8_t size);


//! time (of day)
typedef struct GNSS_TIME_s
{
    uint8_t  hour;   //!< hour [0..23]
    uint8_t  min;    //!< minute [0..59]
    uint8_t  sec;    //!< second [0..61]
    uint16_t acc;    //!< time accuracy [ms]
    bool     valid;  //!< true if time valid
    bool     leap;   //!< true if leap seconds are resolved
} GNSS_TIME_t;


//! get current time
/*!
    \param[in] pTime    time structure
    \param[in] tzOffs   timezone offset (0 = UTC
    \param[in] timeout  number of ticks to wait at most (0 to wait forever,
                        or -1 to not wait at all, i.e. get latest known time)
    \returns true if time was available within timeout
*/
bool gnssGetTime(GNSS_TIME_t *pTime, const int8_t tzOffs, int32_t timeout);


/* *************************************************************************** */

#endif // __GNSS_H__

//@}
// eof
