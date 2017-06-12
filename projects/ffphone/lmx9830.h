/*!
    \file
    \brief flipflip's Arduino Uno stuff: LMX9830 bluetooth chip (see \ref PROJECTS_FFPHONE_LMX9830)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    - All trademarks property of their respective owners.

    \defgroup PROJECTS_FFPHONE_LMX9830 LMX9830
    \ingroup PROJECTS_FFPHONE

    This module implements configuration and control of the National Semiconductor LMX9830 bluetooth
    chip.

    - [LMX9830 Datasheet](../../projects/ffphone/lmx9830_datasheet.pdf)
    - [LMX9830 Firmware Release Notes](../../projects/ffphone/lmx9830_firmware_release_notes.pdf)
    - [LMX9830 Software User Guide](../../projects/ffphone/lmx9830_software_user_guide.pdf)

    @{
*/

#ifndef __LMX9830_H__
#define __LMX9830_H__

#include <avr/pgmspace.h>  // avr: program space utilities

#include "stdstuff.h"

//! LMX9830 packet type identification
typedef enum LMX_PTYPE_e
{
    LMX_PTYPE_REQ = 0x52, //!< 'R' / REQ / request (host -> module)
    LMX_PTYPE_CFM = 0x43, //!< 'C' / CFM / confirm (module -> host)
    LMX_PTYPE_IND = 0x69, //!< 'i' / IND / indication (module -> host)
    LMX_PTYPE_RES = 0x72  //!< 'r' / RES / response (module -> host)
} LMX_PTYPE_t;

//! LMX9830 opcode values
typedef enum LMX_OPCODE_e
{
    //! dummy, not a real opcode
    LMX_OPCODE_NONE = 0xff,

    /*! perform soft reset [LMX9830, p.185]
        - #LMX_PTYPE_REQ:
          - no parameters */
    LMX_OPCODE_RESET = 0x26,

    /*! the device is initialised and read to receive commands [LMX9830, p.185]
      - #LMX_PTYPE_IND:
        - 1 byte: number of bytes in software version string
        - n bytes: software version string */
    LMX_OPCODE_DEVICE_READY = 0x25,

    /*! restore factory settings [LMX9830, p.185]
        - #LMX_PTYPE_REQ:
          - no parameters
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e */
    LMX_OPCODE_RESTORE_FACTORY_SETTINGS = 0x1a,

    /*! store class of device [LMX9830, p.172]
        - #LMX_PTYPE_REQ:
          - 3 bytes (LSB first) class of device à la http://www.bluetooth.org/Technical/AssignedNumbers/baseband.htm
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e */
    LMX_OPCODE_STORE_CLASS_OF_DEVICE = 0x28,

    /*! request the user-friendly name for the local Bluetooth device
        - #LMX_PTYPE_REQ:
          - no parameters
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 1 byte: name length (max. 40)
          - n bytes: nul terminated string  */
    LMX_OPCODE_GAP_READ_LOCAL_NAME = 0x03,

    /*! change the user-friendly name of the device [LMX9830, p.171]
        - #LMX_PTYPE_REQ:
          - max. 40 bytes: null-terminated character string
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e */
    LMX_OPCODE_GAP_WRITE_LOCAL_NAME = 0x04,

    /*! read the Bluetooth device address of the local Bluetooth device
        - #LMX_PTYPE_REQ:
          - no parameters
          - 1 byte: #LMX_ERROR_e
          - 6 bytes: address
    */
    LMX_OPCODE_GAP_READ_LOCAL_BDA = 0x05,

    /*! store new PIN code in NVS [LMX9830, p.182]
        - #LMX_PTYPE_REQ:
          - 1 byte: length of PIN in bytes
          - max. 16 bytes: character (!) string of the PIN
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e */
    LMX_OPCODE_GAP_SET_FIXED_PIN = 0x17,

    /*! establishment of a remotely requested DLC link [LMX9830, p.132]
        - #LMX_PTYPE_RES:
          - 6 bytes: bluetooth address of the remote device
          - 1 byte: local RFCOMM port number [1..30] */
    LMX_OPCODE_SPP_INCOMING_LINK_ESTABLISHED = 0x0c,

    /*! DLC link released (or dead) [LMX9830, p.132]
        - #LMX_PTYPE_IND:
          - 1 byte: #LMX_RFCR_e
          - 1 byte: local RFCOMM port number [1..30] */
    LMX_OPCODE_SPP_LINK_RELEASED = 0x0e,

    /*! switch to / announce transparent mode [LMX9830, p.135]
        - #LMX_PTYPE_REQ:
          - 1 byte: local RFCOMM port number [1..30]
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 1 byte: local RFCOMM port number [1..30]
        - #LMX_PTYPE_IND:
          - 1 byte: local RFCOMM port number [1..30]
          - 1 byte: #LMX_MODE_e
        - #LMX_PTYPE_RES: */
    LMX_OPCODE_SPP_TRANSPARENT_MODE = 0x11,

    /*! set default audio settings, store to NVS [LMX9830, p.138]
        - #LMX_PTYPE_REQ:
          - 1 byte: #LMX_AUDIOCODEC_e
          - 1 byte: #LMX_AIRFORMAT_e */
    LMX_OPCODE_SET_DEFAULT_AUDIO_CONFIG = 0x5b,
    //LMX_OPCODE_GAP_SET_AUDIO_CONFIG = 0x59,

    /*! delete all stored SDP records [LMX9830, p.170]
        - #LMX_PTYPE_REQ:
          - no parameters
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e */
    LMX_OPCODE_DELETE_SDP_RECORDS = 0x2a,

    /*! create new service (SDP) record and store to NVS [LMX9830, p.168]
        - #LMX_PTYPE_REQ:
          - 1 byte: local RFCOMM port [1..30]
          - 1 byte: #LMX_AUTH_e
          - 1 byte: #LMX_ENC_e
          - 2 bytes (uint16_t): size of SDP record
          - n bytes: SDP record
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 1 byte: unique identifier ("slot" in NVS) */
    LMX_OPCODE_STORE_SDP_RECORD = 0x31,

    /*! enable stored SDP record [LMX9830, p.169]
        - #LMX_PTYPE_REQ:
          - 1 byte: #LMX_SDPSTATE_e
          - 1 byte: unique identifier (see #LMX_OPCODE_STORE_SDP_RECORD)
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 1 byte: unique identifier*/
    LMX_OPCODE_ENABLE_SDP_RECORD = 0x29,

    /*! set RFCOMM ports to initialise and store to NVS [LMX9830, p.167]
        - #LMX_PTYPE_REQ:
          - 4 bytes (uint32_t): bit mask (bits [0..29]) of RFCOMM ports ([1..30]) to open (bit set) / close (bit cleared)
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e */
    LMX_OPCODE_SET_PORTS_TO_OPEN = 0x22,

    /*! list of paired devices [LMX9830, p.181]
        - #LMX_PTYPE_REQ:
          - no parameters
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 1 byte: device count
          - N * 6 bytes: device addresses */
    LMX_OPCODE_GAP_LIST_PAIRED_DEVICES = 0x1c,

    /*! remote device name [LMX9830, p.122]
        - #LMX_PTYPE_REQ:
          - 6 bytes. device address
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 6 bytes: bluetooth address
          - 1 byte: size of device name
          - N (max. 40) bytes: device name (NOT null-terminated, in contradiction to [LMX9830]) */
    LMX_OPCODE_GAP_REMOTE_DEVICE_NAME = 0x02,

    /*! send data on SPP link to remote bluetooth device [LMX9830, p.130]
        - #LMX_PTYPE_REQ:
          - 1 byte: local RFCOMM port [1..30]
          - 2 bytes: payload size
          - N bytes: payload
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 1 byte: local RFCOMM port [1..30] */
    LMX_OPCODE_SPP_SEND_DATA = 0x0f,

    /*! get RSSI value for remote device
        - #LMX_PTYPE_REQ:
          - 6 bytes. device address
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_ERROR_e
          - 1 byte: RSSI */
    LMX_OPCODE_READ_RSSI = 0x20,

    //! dummy opcode
    LMX_OPCODE_ANY = 0xff

} LMX_OPCODE_t;

//! LMX9830 generic error codes [LMX9830, pp.198]
typedef enum LMX_ERROR_e
{
    LMX_ERROR_OK                               = 0x00, //!< no error
    LMX_ERROR_INVALID_NO_OF_PARAMETERS         = 0x01, //!< illegal number of bytes in the #LMX_PTYPE_REQ
    LMX_ERROR_TIMEOUT                          = 0x04, //!< a timeout occured
    LMX_ERROR_NAME_TOO_LONG                    = 0x06, //!< string in the #LMX_PTYPE_REQ too long
    LMX_ERROR_PINCODE_TOO_LONG                 = 0x2e, //!< PIN code string in the #LMX_PTYPE_REQ too long
    LMX_ERROR_ILLEGAL_STATE_VALUE              = 0x13, //!< invalid state value
    LMX_ERROR_IDENTIFIER_OUT_OF_RANGE          = 0x14, //!< identifier larger than allowed
    LMX_ERROR_RECORD_ALREADY_IN_SELECTED_STATE = 0x15, //!< SDP record is already enabled/disabled
    LMX_ERROR_IDENTIFIER_NOT_IN_USE            = 0x28, //!< trying to enable a SDP record that is not stored
    LMX_ERROR_INVALID_AUTHENTICATION_VALUE     = 0x16, //!< illegal authentication setting
    LMX_ERROR_INVALID_ENCRYPTION_VALUE         = 0x17, //!< illegal encryption setting
    LMX_ERROR_INVALID_PORT                     = 0x12, //!< illegal port value
    LMX_ERROR_UNKNOWN                          = 0xff  //!< unknown error (dummy)
} LMX_ERROR_t;

//! LMX9830 RFCOMM release reasons
typedef enum LMX_RFCR_e
{
    LMX_RFCR_DLC_DISC_LOCAL_DEVICE  = 0x00, //!< the local device has disconnected the DLC
    LMX_RFCR_DLC_DISC_REMOTE_DEVICE = 0x01, //!< the remote device has disconnected the DLC
    LMX_RFCR_DLC_DISC_ACL_FAILURE   = 0x02, //!< ACL link failure / link supervision timeout
    LMX_RFCR_DLC_DISC_LOWER_LAYER   = 0x03  //!< lower layer (e.g. L2CAP) has disconnected the DLC
} LMX_RFCR_t;

//! LMX9830 mode (#LMX_OPCODE_SPP_TRANSPARENT_MODE)
typedef enum LMX_MODE_e
{
    LMX_MODE_COMMAND     = 0x00, //!< command mode
    LMX_MODE_TRANSPARENT = 0x01, //!< transparent mode
    LMX_MODE_UNKNOWN     = 0xff  //!< unknown mode (dummy)
} LMX_MODE_t;

//! LMX9830 audio codec type (#LMX_OPCODE_SET_DEFAULT_AUDIO_CONFIG)
typedef enum LMX_AUDIOCODEC_e
{
    LMX_AUDIOCODEC_NONE     = 0x00, //!< no codec available (default)
    LMX_AUDIOCODEC_MOTOROLA = 0x01, //!< Motorola MC145483
    LMX_AUDIOCODEC_OKI      = 0x02, //!< OKI MSM7717
    LMX_AUDIOCODEC_PCMSLAVE = 0x03  //!< PCM slave (req. additional config)
} LMX_AUDIOCODEC_t;

//! LMX9830 audio air format (#LMX_OPCODE_SET_DEFAULT_AUDIO_CONFIG)
typedef enum LMX_AIRFORMAT_e
{
    LMX_AIRFORMAT_CVSD = 0x00, //!< CVSD (default)
    LMX_AIRFORMAT_ULAW = 0x01, //!< μ-LAW
    LMX_AIRFORMAT_ALAW = 0x02  //!< A-law
} LMX_AIRFORMAT_t;

//! SDP record authentication requirements (#LMX_OPCODE_STORE_SDP_RECORD)
typedef enum LMX_AUTH_e
{
    LMX_AUTH_NONE  = 0x00, //!< none required
    LMX_AUTH_IN    = 0x02, //!< required for incoming connections
    LMX_AUTH_OUT   = 0x20, //!< required for outgoing connections
    LMX_AUTH_INOUT = 0x22, //!< required for both directions
} LMX_AUTH_t;

//! SDP record encryption requirements (#LMX_OPCODE_STORE_SDP_RECORD)
typedef enum LMX_ENC_e
{
    LMX_ENC_NONE  = 0x00, //!< none required
    LMX_ENC_IN    = 0x04, //!< required for incoming connections
    LMX_ENC_OUT   = 0x40, //!< required for outgoing connections
    LMX_ENC_INOUT = 0x44, //!< required for both directions
} LMX_ENC_t;

//! SDP record state (#LMX_OPCODE_ENABLE_SDP_RECORD)
typedef enum LMX_SDPSTATE_e
{
    LMX_SDPSTATE_DISABLE = 0x00, //!< disable(d)
    LMX_SDPSTATE_ENABLE  = 0x01  //!< enable(d)
} LMX_SDPSTATE_t;

//! LMX command protocol sync chars
typedef enum LMX_PROTO_e
{
    LMX_STX = 0x02, //!< STX (Start of TeXt)
    LMX_ETX = 0x03  //!< ETX (End of TeXt)
} LMX_PROTO_t;

//! LMX command frame size (incl. sync chars)
#define LMX_FRAME_SIZE     (1 + 1 + 1 + 2 + 1 + 1)

//! LMX command payload offset in message
#define LMX_PAYLOAD_OFFSET (1 + 1 + 1 + 2 + 1)

//! get description string for given packet type
/*!
    \param[in] ptype  packet type identifier

    \returns the description string
*/
PGM_P /* const char * */lmxGetPtypeString(const LMX_PTYPE_t ptype);

//! get description string for given opcode
/*!
    \param[in] opcode  opcode identifier

    \returns the description string
*/
PGM_P /* const char * */lmxGetOpcodeString(const LMX_OPCODE_t opcode);

//! get description string for given error
/*!
    \param[in] error  error identifier

    \returns the description string
*/
PGM_P /* const char * */lmxGetErrorString(const LMX_ERROR_t error);

//! get description string for given linkrelease
/*!
    \param[in] linkrelease  linkrelease identifier

    \returns the description string
*/
PGM_P /* const char * */lmxGetLinkreleaseString(const LMX_RFCR_t linkrelease);

//! get description string for given operation mode
/*!
    \param[in] mode  mode identifier

    \returns the description string
*/
PGM_P /* const char * */lmxGetModeString(const LMX_MODE_t mode);


#endif // __LMX9830_H__

//@}
// eof
