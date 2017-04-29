/*!
    \file
    \brief flipflip's Arduino Uno stuff: u-blox binary protocol (see \ref UBX)

    - Copyright (c) 2014-2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup UBX u-blox binary protocol
    \ingroup FF

    This implements some of the protocol used by u-blox positioning (GNSS) receivers according to
    [u-blox M8 Receiver Description Including Protocol Specification (UBX-13003221)](../../projects/ffclock/docs/u-bloxM8_ReceiverDescriptionProtocolSpec_Public_(UBX-13003221).pdf).

    @{
*/

#ifndef __UBX_H__
#define __UBX_H__ //!< multiple inclusion guard

#include "stdstuff.h"      // ff: useful macros and types

/* ************************************************************************** */

//! initialise the u-blox binary protocol parser
void ubxInit(void);


/*!
    \name u-blox binary protocol (UBX) messages
    @{
*/

//! UBX protocol message classes
typedef enum UBX_MSG_CLS_e
{
    UBX_MSG_CLS_NAV  = 0x01,  //!< UBX-NAV message class (navigation)
    UBX_MSG_CLS_INF  = 0x04   //!< UBX-INF message class (information)
} UBX_MSG_CLS_t;

//! UBX protocol message IDs
typedef enum UBX_MSG_ID_e
{
    UBX_MSG_ID_NAV_PVT       = 0x07, //!< UBX-NAV-PVT message ID
    UBX_MSG_ID_NAV_AOPSTATUS = 0x60, //!< UBX-NAV-AOPSTATUS message ID
    UBX_MSG_ID_NAV_CLOCK     = 0x22, //!< UBX-NAV-CLOCK message ID
    UBX_MSG_ID_NAV_POSECEF   = 0x02, //!< UBX-NAV-POSECEF message ID
    UBX_MSG_ID_NAV_VELECEF   = 0x11, //!< UBX-NAV-VELECEF message ID
    UBX_MSG_ID_NAV_STATUS    = 0x03, //!< UBX-NAV-STATUS message ID
    UBX_MSG_ID_INF_ERROR     = 0x00, //!< UBX-INF-ERROR message ID
    UBX_MSG_ID_INF_WARNING   = 0x01, //!< UBX-INF-WARNING message ID
    UBX_MSG_ID_INF_NOTICE    = 0x02, //!< UBX-INF-NOTICE message ID
    UBX_MSG_ID_INF_TEST      = 0x03, //!< UBX-INF-TEST message ID
    UBX_MSG_ID_INF_DEBUG     = 0x04  //!< UBX-INF-DEBUG message ID
} UBX_MSG_ID_t;

//! UBX-NAV-PVT message payload (navigation position/velocity/time solution, see #FF_UBX_NAV_PVT_USE)
typedef struct UBX_NAV_PVT_PAYLOAD_s
{
    uint32_t iTOW;                  //!< timestamp (for UBX-NAV-* message grouping)
    uint16_t year;                  //!< UTC year
    uint8_t  month;                 //!< UTC month
    uint8_t  day;                   //!< UTC day
    uint8_t  hour;                  //!< UTC hour
    uint8_t  min;                   //!< UTC minute
    uint8_t  sec;                   //!< UTC second
    uint8_t  valid;                 //!< validity flags
    uint32_t tAcc;                  //!< time accuracy [ns]
    int32_t  nano;                  //!< UTC fraction of second -1e-9..+1e9 [ns]
    uint8_t  fixType;               //!< GNSS fix type
    uint8_t  flags;                 //!< GNSS fix status flags
    uint8_t  reserved1;             //!< reserved
    uint8_t  numSV;                 //!< number of SVs used in navigation solution
    int32_t  lon;                   //!< longitude [1e-7 deg]
    int32_t  lat;                   //!< latitude [1e-7 deg]
    int32_t  height;                //!< height above ellipsoid [mm]
    int32_t  hMSL;                  //!< height above sea level [mm]
    uint32_t hAcc;                  //!< horizontal accuracy estimate [mm]
    uint32_t vAcc;                  //!< vertical accuracy estimate [mm]
    int32_t  velN;                  //!< North velocity [mm/s]
    int32_t  velE;                  //!< East velocity [mm/s]
    int32_t  velD;                  //!< down velocity [mm/s]
    int32_t  gSpeed;                //!< speed over ground [mm/s]
    int32_t  headMot;               //!< course over ground (heading) [1e-5 deg]
    uint32_t sAcc;                  //!< speed accuracy estimate [mm/s]
    uint32_t headAcc;               //!< heading accuracy [1e-5 deg]
    uint16_t pDOP;                  //!< position dillution of precision (DOP)
    uint16_t reserved2;             //!< reserved
    uint32_t reserved3;             //!< reserved
    int32_t  headVeh;               //!< vehicle course over ground (heading) [1e-5 deg]
    uint32_t reserved4;             //!< reserved
} __PACKED UBX_NAV_PVT_PAYLOAD_t;

#define UBX_NAV_PVT_VALID_DATE_Pos             0                             //!< #UBX_NAV_PVT_VALID_DATE_Mask position
#define UBX_NAV_PVT_VALID_DATE_Mask   (0x01 << UBX_NAV_PVT_VALID_DATE_Pos)   //!< UTC date valid flag mask
#define UBX_NAV_PVT_VALID_TIME_Pos             1                             //!< #UBX_NAV_PVT_VALID_TIME_Mask position
#define UBX_NAV_PVT_VALID_TIME_Mask   (0x01 << UBX_NAV_PVT_VALID_TIME_Pos)   //!< UTC time valid flag mask
#define UBX_NAV_PVT_VALID_LEAP_Pos             2                             //!< #UBX_NAV_PVT_VALID_LEAP_Mask position
#define UBX_NAV_PVT_VALID_LEAP_Mask   (0x01 << UBX_NAV_PVT_VALID_LEAP_Pos)   //!< UTC leap seconds valid flag mask
#define UBX_NAV_PVT_FIXTYPE_NOFIX     0 //!< no fix
#define UBX_NAV_PVT_FIXTYPE_DRFIX     1 //!< dead-reckoning only fix
#define UBX_NAV_PVT_FIXTYPE_2DFIX     2 //!< 2d fix
#define UBX_NAV_PVT_FIXTYPE_3DFIX     3 //!< 3d fix
#define UBX_NAV_PVT_FIXTYPE_GSFIX     4 //!< GNSS + dead-reckoning fix
#define UBX_NAV_PVT_FIXTYPE_TIFIX     5 //!< time only fix
#define UBX_NAV_PVT_FLAGS_FIXOK_Pos            0                             //!< #UBX_NAV_PVT_FLAGS_FIXOK_Mask position
#define UBX_NAV_PVT_FLAGS_FIXOK_Mask  (0x01 << UBX_NAV_PVT_FLAGS_FIXOK_Pos)  //!< GNSS fix ok (within DOP and accuracy mask) mask
#define UBX_NAV_PVT_FLAGS_DIFF_Pos             1                             //!< #UBX_NAV_PVT_FLAGS_DIFF_Mask position
#define UBX_NAV_PVT_FLAGS_DIFF_Mask   (0x01 << UBX_NAV_PVT_FLAGS_DIFF_Pos)   //!< differential corrections applied flag mask
#define UBX_NAV_PVT_FLAGS_PSM_Pos              2                             //!< #UBX_NAV_PVT_FLAGS_PSM_Mask position
#define UBX_NAV_PVT_FLAGS_PSM_Mask    (0x07 << UBX_NAV_PVT_FLAGS_PSM_Pos)    //!< power save mode state mask

//! UBX-NAV-AOPSTATUS message payload (AssistNow Autonomous (AOP) status, see #FF_UBX_NAV_AOPSTATUS_USE)
typedef struct UBX_NAV_AOPSTATUS_PAYLOAD_s
{
    uint32_t iTOW;                  //!< timestamp (for UBX-NAV-* message grouping)
    uint8_t  aopCfg;                //!< AOP configuration
    uint8_t  status;                //!< AOP status, idle (0) or not idle (>0)
    uint8_t  reserved0;             //!< reserved
    uint8_t  reserved1;             //!< reserved
    uint32_t reserved2;             //!< reserved
    uint32_t reserved3;             //!< reserved
} __PACKED UBX_NAV_AOPSTATUS_PAYLOAD_t;

#define UBX_NAV_AOPSTATUS_AOPCFG_USEAOP_Pos     0 //!< #UBX_NAV_AOPSTATUS_AOPCFG_USEAOP_Mask position
#define UBX_NAV_AOPSTATUS_AOPCFG_USEAOP_Mask (0x01 << UBX_NAV_AOPSTATUS_AOPCFG_USEAOP_Pos)  //!< AOP enabled flag mask

//! UBX-NAV-CLOCK message payload (clock solution, see #FF_UBX_NAV_CLOCK_USE)
typedef struct UBX_NAV_CLOCK_PAYLOAD_s
{
    uint32_t iTOW;                  //!< timestamp (for UBX-NAV-* message grouping)
    int32_t  clkB;                  //!< clock bias [ns]
    int32_t  clkD;                  //!< clock drift [ns/s]
    uint32_t tAcc;                  // time accuracy estimate [ns]
    uint32_t fAcc;                  // frequency accuracy estimate [ps/s]
} __PACKED UBX_NAV_CLOCK_PAYLOAD_t;

//! UBX-NAV-POSECEF message payload (position solution in ECEF, see #FF_UBX_NAV_POSECEF_USE)
typedef struct UBX_NAV_POSECEF_PAYLOAD_s
{
    uint32_t iTOW;                  //!< timestamp (for UBX-NAV-* message grouping)
    int32_t  ecefX;                 //!< ECEF x component [cm]
    int32_t  ecefY;                 //!< ECEF y component [cm]
    int32_t  ecefZ;                 //!< ECEF z component [cm]
    uint32_t pAcc;                  //!< position accuracy estimate [cm]
} __PACKED UBX_NAV_POSECEF_PAYLOAD_t;

//! UBX-NAV-VELECEF message payload (velocity solution in ECEF, see #FF_UBX_NAV_VELECEF_USE)
typedef struct UBX_NAV_VELECEF_PAYLOAD_s
{
    uint32_t iTOW;                  //!< timestamp (for UBX-NAV-* message grouping)
    int32_t  ecefX;                 //!< ECEF x velocity [cm/s]
    int32_t  ecefY;                 //!< ECEF y velocity [cm/s]
    int32_t  ecefZ;                 //!< ECEF z velocity [cm/s]
    uint32_t sAcc;                  //!< velocity accuracy estimate [cm/s]
} __PACKED UBX_NAV_VELECEF_PAYLOAD_t;

//! UBX-NAV-STATUS message payload (receiver navigation status, see #FF_UBX_NAV_STATUS_USE)
typedef struct UBX_NAV_STATUS_PAYLOAD_s
{
    uint32_t iTOW;                  //!< timestamp (for UBX-NAV-* message grouping)
    uint8_t  gpsFix;                //!< "GPS" fix type
    uint8_t  flags;                 //!< navigation status flags
    uint8_t  fixStat;               //!< fix status
    uint8_t  flags2;                //!< further information about navigation output
    uint32_t ttff;                  //!< time to first fix (as measured by the receiver) [ms]
    uint32_t msss;                  //!< milliseconds since startup/reset
} __PACKED UBX_NAV_STATUS_PAYLOAD_t;

#define UBX_NAV_STATUS_GPSFIX_NOFIX      0 //!< no fix
#define UBX_NAV_STATUS_GPSFIX_DRFIX      1 //!< dead-reckoning only fix
#define UBX_NAV_STATUS_GPSFIX_2DFIX      2 //!< 2d fix
#define UBX_NAV_STATUS_GPSFIX_3DFIX      3 //!< 3d fix
#define UBX_NAV_STATUS_GPSFIX_GSFIX      4 //!< "GPS" + dead-reckoning fix
#define UBX_NAV_STATUS_GPSFIX_TIFIX      5 //!< time only fix
#define UBX_NAV_STATUS_FLAGS_FIXOK_Pos       0 //!< #UBX_NAV_STATUS_FLAGS_FIXOK_Mask position
#define UBX_NAV_STATUS_FLAGS_FIXOK_Mask  (0x01 << UBX_NAV_STATUS_FLAGS_FIXOK_Pos)   //!< "GPS" fix ok (within DOP and accuracy mask) mask
#define UBX_NAV_STATUS_FLAGS_DIFF_Pos        1 //!< #UBX_NAV_STATUS_FLAGS_DIFF_Mask position
#define UBX_NAV_STATUS_FLAGS_DIFF_Mask   (0x01 << UBX_NAV_STATUS_FLAGS_DIFF_Pos)    //!< DGPS (differential corrections) used
#define UBX_NAV_STATUS_FLAGS_WNOSET_Pos      2 //!< #UBX_NAV_STATUS_FLAGS_WNOSET_Mask position
#define UBX_NAV_STATUS_FLAGS_WNOSET_Mask (0x01 << UBX_NAV_STATUS_FLAGS_WNOSET_Pos)  //!< week number valid flag
#define UBX_NAV_STATUS_FLAGS_TOWSET_Pos      3 //!< #UBX_NAV_STATUS_FLAGS_TOWSET_Mask position
#define UBX_NAV_STATUS_FLAGS_TOWSET_Mask (0x01 << UBX_NAV_STATUS_FLAGS_TOWSET_Pos)  //!< time of week valid flag
#define UBX_NAV_STATUS_FIXSTAT_DGPS_Pos      0 //!< #UBX_NAV_STATUS_FIXSTAT_DGPS_Mask position
#define UBX_NAV_STATUS_FIXSTAT_DGPS_Mask (0x01 << UBX_NAV_STATUS_FIXSTAT_DGPS_Pos)   //!< DGPS input status (0 = none, 1 = PR+PRR corrections)
#define UBX_NAV_STATUS_FIXSTAT_MAPM_Pos      6 //!< #UBX_NAV_STATUS_FIXSTAT_MAPM_Mask position
#define UBX_NAV_STATUS_FIXSTAT_MAPM_Mask (0x03 << UBX_NAV_STATUS_FIXSTAT_MAPM_Pos)   //!< map matching status (0 = none, 1 = valid but unused, 2 = valid and used, 3 = WTF?!)
#define UBX_NAV_STATUS_FLAGS2_PSM_Pos        0 //!< #UBX_NAV_STATUS_FLAGS2_PSM_Mask position
#define UBX_NAV_STATUS_FLAGS2_PSM_Mask   (0x01 << UBX_NAV_STATUS_FLAGS__Pos)   //!< power save mode (PSM) state (0 = acquisition, 1 = tracking, 2 = power optimised tracking, 3 inactive)

//! maximum length of UBX-INF-* message we'll handle (incl. ending NUL character)
#define UBX_INF_MAX_LEN     64

//! UBX-INF-* messages payload (error/warning/notice/test/debug string messages, see #FF_UBX_INF_ERROR_USE etc.)
typedef struct UBX_INF_ANY_PAYLOAD_s
{
    char str[UBX_INF_MAX_LEN];  //!< error/warning/notice/test/debug string
} __PACKED UBX_INF_ANY_PAYLOAD_t;

//! union of all UBX message payloads
typedef union UBX_PAYLOADS_u
{
#if (defined FF_UBX_NAV_PVT_USE) || defined(__DOXYGEN__)
    UBX_NAV_PVT_PAYLOAD_t       navPvt;     //!< UBX-NAV-PVT message payload
#endif
#if (defined FF_UBX_NAV_AOPSTATUS_USE) || defined(__DOXYGEN__)
    UBX_NAV_AOPSTATUS_PAYLOAD_t aopStatus;  //!< UBX-NAV-AOPSTATUS message payload
#endif
#if (defined FF_UBX_NAV_CLOCK_USE) || defined(__DOXYGEN__)
    UBX_NAV_CLOCK_PAYLOAD_t     navClock;   //!< UBX-NAV-CLOCK message payload
#endif
#if (defined FF_UBX_NAV_POSECEF_USE) || defined(__DOXYGEN__)
    UBX_NAV_POSECEF_PAYLOAD_t   navPosecef; //!< UBX-NAV-POSECEF message payload
#endif
#if (defined FF_UBX_NAV_VELECEF_USE) || defined(__DOXYGEN__)
    UBX_NAV_VELECEF_PAYLOAD_t   navVelecef; //!< UBX-NAV-VELECEF message payload
#endif
#if (defined FF_UBX_NAV_STATUS_USE) || defined(__DOXYGEN__)
    UBX_NAV_STATUS_PAYLOAD_t    navStatus;  //!< UBX-NAV-STATUS message payload
#endif
#if (defined FF_UBX_INF_ERROR_USE) || (defined FF_UBX_INF_WARNING_USE) || (defined FF_UBX_INF_NOTICE_USE) || (defined FF_UBX_INF_TEST_USE) || (defined FF_UBX_INF_DEBUG_USE) || defined(__DOXYGEN__)
    UBX_INF_ANY_PAYLOAD_t       infAny;     //!< UBX-INF-* messages payload
#endif
} UBX_PAYLOADS_t;

//! max. payload size
#define UBX_MAX_PAYLOAD_SIZE sizeof(UBX_PAYLOADS_t)

//@}


//! feed data to the UBX parser
/*!
    \param[in]  c        the character to feed
    \param[out] pMsgCls  the message class
    \param[out] pMsgId   the message ID
    \param[out] pPayload the message payload
    \returns true if a message has been received (and \c pMsgCls, \c pMsgId and \c pPayload are valid) , false otherwise
*/
bool ubxFeedByte(uint8_t c, UBX_MSG_CLS_t *pMsgCls, UBX_MSG_ID_t *pMsgId, UBX_PAYLOADS_t *pPayload);

//! make UBX protocol parser status string
/*!
    \param[out] str   the target string
    \param[in]  size  the maximum size of the string
*/
void ubxStatus(char *str, const uint8_t size);


/* ************************************************************************** */

#endif // __UBX_H__

//@}
// eof
