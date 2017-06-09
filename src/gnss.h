/*!
    \file
    \brief flipflip's Arduino Uno stuff: GNSS receiver abstraction (see \ref GNSS)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup GNSS GNSS Receiver Abstraction
    \ingroup FF

    This implements a GNSS receiver abstraction.

    Configuration: #FF_GNSS_TASK_PRIO, #FF_GNSS_TASK_STACK and #FF_GNSS_PARSER configuration.

    See \ref EXAMPLES_EX06 for a demonstration of \ref PROJECTS_FFCLOCK for an application.

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

//! fix type
typedef enum GNSS_FIXTYPE_e
{
    GNSS_FIXTYPE_NONE,   //!< no fix
    GNSS_FIXTYPE_DR,     //!< dead-reckoning only fix
    GNSS_FIXTYPE_2D,     //!< 2d GNSS fix
    GNSS_FIXTYPE_3D,     //!< 3d GNSS fix
    GNSS_FIXTYPE_SF,     //!< GNSS and dead-reckoning (sensor fusion) fix
    GNSS_FIXTYPE_TIME,   //!< time only fix
} GNSS_FIXTYPE_t;

//! time accuracy estimate
typedef enum GNSS_TACC_e
{
    GNSS_TACC_100NS,     //!< time uncertainty better than 100ns
    GNSS_TACC_1MS,       //!< time uncertainty better than 1ms
    GNSS_TACC_10MS,      //!< time uncertainty better than 10ms
    GNSS_TACC_100MS,     //!< time uncertainty better than 100ms
    GNSS_TACC_500MS,     //!< time uncertainty better than 500ms
    GNSS_TACC_1S,        //!< time uncertainty better than 1s
    GNSS_TACC_10S,       //!< time uncertainty better than 10s
    GNSS_TACC_BIG,       //!< time uncertainty 10s or more
} GNSS_TACC_t;

//! position accuracy estimate
typedef enum GNSS_PACC_e
{
    GNSS_PACC_1CM,       //!< position uncertainty better than 1cm
    GNSS_PACC_10CM,      //!< position uncertainty better than 10cm
    GNSS_PACC_1M,        //!< position uncertainty better than 1m
    GNSS_PACC_2M,        //!< position uncertainty better than 2m
    GNSS_PACC_5M,        //!< position uncertainty better than 5m
    GNSS_PACC_10M,       //!< position uncertainty better than 10m
    GNSS_PACC_100M,      //!< position uncertainty better than 100m
    GNSS_PACC_BIG,       //!< position uncertainty 100m or more
} GNSS_PACC_t;

//! dillution of precision (DOP)
typedef enum GNSS_DOP_e
{
    GNSS_DOP_0_5,       //!< DOP better than 0.5
    GNSS_DOP_1_0,       //!< DOP better than 1.0
    GNSS_DOP_1_5,       //!< DOP better than 1.5
    GNSS_DOP_2_0,       //!< DOP better than 2.0
    GNSS_DOP_2_5,       //!< DOP better than 2.5
    GNSS_DOP_5_0,       //!< DOP better than 5.0
    GNSS_DOP_10_0,      //!< DOP better than 10.0
    GNSS_DOP_BIG,       //!< DOP 10.0 or more
} GNSS_DOP_t;

//! navigation solution, epoch
typedef struct GNSS_EPOCH_s
{
    uint32_t       count;          //!< epoch counter
//----
    GNSS_FIXTYPE_t fixType   : 3;  //!< fix type
    uint8_t        numSv     : 5;  //!< number of satellites used in navigation solution [0..63]
//-
    bool           dateValid : 1;  //!< date is valid (\c day, \c month, \c year fields)
    bool           timeValid : 1;  //!< time is valid (\c hour, \c min, \c sec fields)
    bool           leapValid : 1;  //!< leap seconds known (time is proper UTC time)
    uint8_t        day       : 5;  //!< day of month [1..31]
//--
    GNSS_PACC_t    hAcc      : 4;  //!< horizontal position accuracy estimate
    GNSS_PACC_t    vAcc      : 4;  //!< vertical position accuracy estimate
//---
    GNSS_TACC_t    tAcc      : 4;  //!< time accuracy estimate
    uint8_t        month     : 4;  //!< month of year [1..12]
//----
    uint16_t       sec       : 6;  //!< second [0..61]
    uint16_t       min       : 6;  //!< minute [0..59]
    GNSS_DOP_t     pDop      : 4;  //!< positon DOP
//--
    uint16_t       year      : 7;  //!< year (since 2000) [0..127]
    bool           fixOk     : 1;  //!< fix considered valid
    uint16_t       hour      : 5;  //!< hour [0..23]
    uint16_t       _pad1     : 3;  // padding
//----
    float          lat;            //!< latitude [deg] [-90.0..+90.0]
//----
    float          lon;            //!< longitude [deg] [-180.0..+180.0]
//----
    int16_t        alt;            //!< altitude, height above ellipsoid [m]
//--
    uint16_t       ttff;           //!< time since last #GNSS_FIXTYPE_NONE [0.1s]
//----
} GNSS_EPOCH_t;


//! get epoch data
/*!
    \param[out] pEpoch   epoch structure, or #NULL
    \param[in]  timeout  number of ticks to wait at most (0 to wait forever,
                         or -1 to not wait at all, i.e. get latest known time)
    \returns true if a new epoch was available within the \c timeout
*/
bool gnssGetEpoch(GNSS_EPOCH_t *pEpoch, int32_t timeout);

//! stringify epoch data for PRINT()ing it
/*!
    \param[in]  pkEpoch  epoch data to stringify
    \param[out] str      the target string
    \param[in]  size     the maximum size of the string (suggested size: 120)

    \note This will need float support in printf() format strings (see Makefile/build.mk).
*/
void gnssStringifyEpoch(const GNSS_EPOCH_t *pkEpoch, char *str, const uint8_t size);

//! stringify time-of-day epoch data for PRINT()ing it
/*!
    \param[in]  pkEpoch  epoch data to stringify
    \param[out] str      the target string
    \param[in]  size     the maximum size of the string (suggested size: 25)
*/
void gnssStringifyTime(const GNSS_EPOCH_t *pkEpoch, char *str, const uint8_t size);


/* *************************************************************************** */

#endif // __GNSS_H__

//@}
// eof
