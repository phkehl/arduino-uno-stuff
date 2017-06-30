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
          - 1 byte: #LMX_GEN_ERROR_e */
    LMX_OPCODE_RESTORE_FACTORY_SETTINGS = 0x1a,

    /*! store class of device [LMX9830, p.172]
        - #LMX_PTYPE_REQ:
          - 3 bytes (LSB first) class of device à la http://www.bluetooth.org/Technical/AssignedNumbers/baseband.htm
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e */
    LMX_OPCODE_STORE_CLASS_OF_DEVICE = 0x28,

    /*! request the user-friendly name for the local Bluetooth device
        - #LMX_PTYPE_REQ:
          - no parameters
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e
          - 1 byte: name length (max. 40)
          - n bytes: nul terminated string  */
    LMX_OPCODE_GAP_READ_LOCAL_NAME = 0x03,

    /*! change the user-friendly name of the device [LMX9830, p.171]
        - #LMX_PTYPE_REQ:
          - max. 40 bytes: null-terminated character string
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e */
    LMX_OPCODE_GAP_WRITE_LOCAL_NAME = 0x04,

    /*! read the Bluetooth device address of the local Bluetooth device
        - #LMX_PTYPE_REQ:
          - no parameters
          - 1 byte: #LMX_GEN_ERROR_e
          - 6 bytes: address
    */
    LMX_OPCODE_GAP_READ_LOCAL_BDA = 0x05,

    /*! store new PIN code in NVS [LMX9830, p.182]
        - #LMX_PTYPE_REQ:
          - 1 byte: length of PIN in bytes
          - max. 16 bytes: character (!) string of the PIN
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e */
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
          - 1 byte: #LMX_GEN_ERROR_e
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
          - 1 byte: #LMX_GEN_ERROR_e */
    LMX_OPCODE_DELETE_SDP_RECORDS = 0x2a,

    /*! create new service (SDP) record and store to NVS [LMX9830, p.168]
        - #LMX_PTYPE_REQ:
          - 1 byte: local RFCOMM port [1..30]
          - 1 byte: #LMX_AUTH_e
          - 1 byte: #LMX_ENC_e
          - 2 bytes (uint16_t): size of SDP record
          - n bytes: SDP record
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e
          - 1 byte: unique identifier ("slot" in NVS) */
    LMX_OPCODE_STORE_SDP_RECORD = 0x31,

    /*! enable stored SDP record [LMX9830, p.169]
        - #LMX_PTYPE_REQ:
          - 1 byte: #LMX_SDPSTATE_e
          - 1 byte: unique identifier (see #LMX_OPCODE_STORE_SDP_RECORD)
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e
          - 1 byte: unique identifier*/
    LMX_OPCODE_ENABLE_SDP_RECORD = 0x29,

    /*! set RFCOMM ports to initialise and store to NVS [LMX9830, p.167]
        - #LMX_PTYPE_REQ:
          - 4 bytes (uint32_t): bit mask (bits [0..29]) of RFCOMM ports ([1..30]) to open (bit set) / close (bit cleared)
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e */
    LMX_OPCODE_SET_PORTS_TO_OPEN = 0x22,

    /*! list of paired devices [LMX9830, p.181]
        - #LMX_PTYPE_REQ:
          - no parameters
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e
          - 1 byte: device count
          - N * 6 bytes: device addresses */
    LMX_OPCODE_GAP_LIST_PAIRED_DEVICES = 0x1c,

    /*! remote device name [LMX9830, p.122]
        - #LMX_PTYPE_REQ:
          - 6 bytes. device address
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e
          - 6 bytes: bluetooth address
          - 1 byte: size of device name (0 unless #LMX_GEN_ERROR_OK)
          - N (max. 40) bytes: device name (NOT null-terminated, in contradiction to [LMX9830]) */
    LMX_OPCODE_GAP_REMOTE_DEVICE_NAME = 0x02,

    /*! send data on SPP link to remote bluetooth device [LMX9830, p.130]
        - #LMX_PTYPE_REQ:
          - 1 byte: local RFCOMM port [1..30]
          - 2 bytes: payload size
          - N bytes: payload
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e
          - 1 byte: local RFCOMM port [1..30] */
    LMX_OPCODE_SPP_SEND_DATA = 0x0f,

    /*! get RSSI value for remote device
        - #LMX_PTYPE_REQ:
          - 6 bytes. device address
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e
          - 1 byte: #LMX_RSSI_e */
    LMX_OPCODE_READ_RSSI = 0x20,

    /*! set event filter
        - #LMX_PTYPE_REQ:
          - 1 byte: #LMX_EVENTFILTER_e
        - #LMX_PTYPE_CFM:
          - 1 byte: #LMX_GEN_ERROR_e */
    LMX_OPCODE_SET_EVENT_FILTER = 0x4e,

    /*! ACL link established
        - #LMX_PTYPE_IND:
          - 6 bytes: address of remote device
          - 1 byte: #LMX_ACL_ERROR_e */
    LMX_OPCODE_GAP_ACL_ESTABLISHED = 0x50,

    /*! ACL link terminated
        - #LMX_PTYPE_IND:
          - 6 bytes: address of remote device
          - 1 byte: #LMX_ACL_ERROR_e */
    LMX_OPCODE_GAP_ACL_TERMINATED = 0x51,

} LMX_OPCODE_t;

//! LMX9830 ACL error codes [LMX9830, pp. 197]
typedef enum LMX_ACL_ERROR_e
{
    LMX_ACL_ERROR_OK                    = 0x00, //!< OK, no error
    LMX_ACL_ERROR_UNKN_HCI_CMD          = 0x01, //!< Unknown HCI Command
    LMX_ACL_ERROR_NO_CONN               = 0x02, //!< No Connection
    LMX_ACL_ERROR_HW_FAILURE            = 0x03, //!< Hardware Failure
    LMX_ACL_ERROR_PG_TIMEOUT            = 0x04, //!< Page Timeout
    LMX_ACL_ERROR_AUTH_FAIL             = 0x05, //!< Authentication Failure
    LMX_ACL_ERROR_KEY_MISSING           = 0x06, //!< Key Missing
    LMX_ACL_ERROR_MEM_FULL              = 0x07, //!< Memory Full
    LMX_ACL_ERROR_CONN_TIMEOUT          = 0x08, //!< Connection Timeout
    LMX_ACL_ERROR_MAX_CONN              = 0x09, //!< Max Number Of Connections
    LMX_ACL_ERROR_MAX_SCO_CONN          = 0x0a, //!< Max Number Of SCO Connections To A Device
    LMX_ACL_ERROR_CONN_EXISTS           = 0x0b, //!< ACL connection already exists
    LMX_ACL_ERROR_CMD_DISALLOW          = 0x0c, //!< Command Disallowed
    LMX_ACL_ERROR_REJECT_RSRC           = 0x0d, //!< Host Rejected due to limited resources
    LMX_ACL_ERROR_REJECT_SEC            = 0x0e, //!< Host Rejected due to security reasons
    LMX_ACL_ERROR_REJECT_PERS           = 0x0f, //!< Host Rejected due to remote device is only a personal device
    LMX_ACL_ERROR_HOST_TIMEOUT          = 0x10, //!< Host Timeout
    LMX_ACL_ERROR_UNSUPP_PARAM          = 0x11, //!< Unsupported Feature or Parameter Value
    LMX_ACL_ERROR_INVAL_CMD_PAR         = 0x12, //!< Invalid HCI Command Parameters
    LMX_ACL_ERROR_CONN_USR_END          = 0x13, //!< Other End Terminated Connection: User Ended Connection
    LMX_ACL_ERROR_CONN_LOW_RES          = 0x14, //!< Other End Terminated Connection: Low Resources
    LMX_ACL_ERROR_CONN_POWEROFF         = 0x15, //!< Other End Terminated Connection: About to Power Off
    LMX_ACL_ERROR_CONN_TERM_LOCAL       = 0x16, //!< Connection Terminated by Local Host
    LMX_ACL_ERROR_REPEAT_ATTEMPT        = 0x17, //!< Repeated Attempts
    LMX_ACL_ERROR_PAIR_NOT_ALLOW        = 0x18, //!< Pairing Not Allowed
    LMX_ACL_ERROR_UNKN_LPM_PDU          = 0x19, //!< Unknown LMP PDU
    LMX_ACL_ERROR_UNSUPP_REM_FEAT       = 0x1a, //!< Unsupported Remote Feature
    LMX_ACL_ERROR_SCO_OFFS_REJ          = 0x1b, //!< SCO Offset Rejected
    LMX_ACL_ERROR_SCO_INT_REJ           = 0x1c, //!< SCO Interval Rejected
    LMX_ACL_ERROR_SCO_AIRM_REJ          = 0x1d, //!< SCO Air Mode Rejected
    LMX_ACL_ERROR_INV_LPM_PARAM         = 0x1e, //!< Invalid LMP Parameters
    LMX_ACL_ERROR_UNSPEC_ERROR          = 0x1f, //!< Unspecified Error
    LMX_ACL_ERROR_UNSUP_LPM_PARAM       = 0x20, //!< Unsupported LMP Parameter Value
    LMX_ACL_ERROR_ROLE_NOT_ALLOW        = 0x21, //!< Role Change Not Allowed
    LMX_ACL_ERROR_LPM_RESP_TIMEOUT      = 0x22, //!< LMP Response Timeout
    LMX_ACL_ERROR_LPM_TRANS_COLL        = 0x23, //!< LMP Error Transaction Collision
    LMX_ACL_ERROR_LPM_PDU_NOT_ALLOW     = 0x24, //!< LMP PDU Not Allowed
    LMX_ACL_ERROR_ENC_MODE              = 0x25, //!< Encryption Mode Not Acceptable
    LMX_ACL_ERROR_UNIT_KEY              = 0x26, //!< Unit Key Used
    LMX_ACL_ERROR_QOS_UNSUPP            = 0x27, //!< QoS is Not Supported
    LMX_ACL_ERROR_INST_PASSWD           = 0x28, //!< Instant Passed
    LMX_ACL_ERROR_PAIR_UNIT_KEY         = 0x29, //!< Pairing with Unit Key Not Supported
    LMX_ACL_ERROR_DIFF_TRANS_COLL       = 0x2a, //!< Different Transaction Collision
    LMX_ACL_ERROR_RESERVED1             = 0x2b, //!< Reserved
    LMX_ACL_ERROR_QOS_PARAM             = 0x2c, //!< QoS Unacceptable Parameter
    LMX_ACL_ERROR_QOS_REJECT            = 0x2d, //!< QoS Rejected
    LMX_ACL_ERROR_CH_CLASS_UNSUPP       = 0x2e, //!< Channel Classification Not Supported
    LMX_ACL_ERROR_INSUFF_SEC            = 0x2f, //!< Insufficient Security
    LMX_ACL_ERROR_PARAM_RANGE           = 0x30, //!< Parameter out of Mandatory Range
    LMX_ACL_ERROR_RESERVED2             = 0x31, //!< Reserved
    LMX_ACL_ERROR_ROLE_SWITCH_PEND      = 0x32, //!< Role Switch Pending
    LMX_ACL_ERROR_RESERVED3             = 0x33, //!< Reserved
    LMX_ACL_ERROR_RES_SLOT_VIOL         = 0x34, //!< Reserved Slot Violation
    LMX_ACL_ERROR_ROLE_SWITCH_FAIL      = 0x35, //!< Role Switch Failed
} LMX_ACL_ERROR_t;

//! LMX9830 generic error codes [LMX9830, pp.198]
typedef enum LMX_GEN_ERROR_e
{
    LMX_GEN_ERROR_OK                                    = 0x00, //!< no error
    LMX_GEN_ERROR_INVALID_NO_OF_PARAMETERS              = 0x01, //!< illegal number of bytes in the #LMX_PTYPE_REQ
    LMX_GEN_ERROR_DURATION_OUT_OF_RANGE                 = 0x02, //!< The given duration value is not valid according to the specification
    LMX_GEN_ERROR_INVALID_MODE                          = 0x03, //!< The selected mode is not valid according to the specification
    LMX_GEN_ERROR_TIMEOUT                               = 0x04, //!< a timeout occured
    LMX_GEN_ERROR_UNKNOWN_ERROR                         = 0x05, //!< An unknown error occurred
    LMX_GEN_ERROR_NAME_TOO_LONG                         = 0x06, //!< string in the #LMX_PTYPE_REQ too long
    LMX_GEN_ERROR_INVALID_DISCOVERABILITY_PARAMETER     = 0x07, //!< The given discoverability parameter does not contain a valid value according to the specification
    LMX_GEN_ERROR_INVALID_CONNECTABILITY_PARAMETER      = 0x08, //!< The given connectability parameter does not contain a valid value according to the specification
    LMX_GEN_ERROR_INVALID_SECURITY_MODE                 = 0x09, //!< The given security mode is not a valid Bluetooth security mode
    LMX_GEN_ERROR_LINKKEY_DOES_NOT_EXISTS               = 0x0a, //!< No link key exists for the given Bluetooth address
    LMX_GEN_ERROR_CONNECTION_FAILED                     = 0x0b, //!< The connection setup failed due to unknown reasons
    LMX_GEN_ERROR_TRUNCATED_ANSWER                      = 0x0c, //!< The returned number of services is too large to be handled by the LMX9830. The answer is truncated
    LMX_GEN_ERROR_RESULT_TOO_LARGE                      = 0x0d, //!< The SDP result from the remote device is too large to be handled by the LMX9830 due to ram limitations
    LMX_GEN_ERROR_NOT_POSSIBLE_TO_ENTER_TESTMODE        = 0x0e, //!< It is currently not possible to enter the selected testmode
    LMX_GEN_ERROR_ILLEGAL_TESTMODE                      = 0x0f, //!< The given test mode is not a valid test mode according to the specification
    LMX_GEN_ERROR_RESET_TO_NSC_BDADDRESS                = 0x10, //!< The LMX9830 will change the Bluetooth address to the NSC address
    LMX_GEN_ERROR_UART_SPEED_OUT_OF_RANGE               = 0x11, //!< The selected UART speed value is not valid according to the specification
    LMX_GEN_ERROR_INVALID_PORT                          = 0x12, //!< illegal port value
    LMX_GEN_ERROR_ILLEGAL_STATE_VALUE                   = 0x13, //!< invalid state value
    LMX_GEN_ERROR_IDENTIFIER_OUT_OF_RANGE               = 0x14, //!< identifier larger than allowed
    LMX_GEN_ERROR_RECORD_ALREADY_IN_SELECTED_STATE      = 0x15, //!< SDP record is already enabled/disabled
    LMX_GEN_ERROR_INVALID_AUTHENTICATION_VALUE          = 0x16, //!< illegal authentication setting
    LMX_GEN_ERROR_INVALID_ENCRYPTION_VALUE              = 0x17, //!< illegal encryption setting
    LMX_GEN_ERROR_MAXIMUM_NO_OF_SERVICE_RECORDS_REACHED = 0x18, //!< The maximum number of service records, which the LMX9830 is able to store, is reached
    LMX_GEN_ERROR_WRITING_TO_NVS                        = 0x19, //!< An error occurred while writing to flash. The service record may not be stored
    LMX_GEN_ERROR_INVALID_ROLE                          = 0x1a, //!< The given role value is not a valid value according to the specification
    LMX_GEN_ERROR_LIMIT                                 = 0x1b, //!< Limits exceeded (Parameter(s) violates limits)
    LMX_GEN_ERROR_UNEXPECTED                            = 0x1c, //!< Unexpected at this moment
    LMX_GEN_ERROR_UNABLE_TO_SEND                        = 0x1d, //!< Could not send at this moment, no reason specified
    LMX_GEN_ERROR_CURRENTLY_NO_BUFFER                   = 0x1e, //!< Currently no room in buffer, try again later
    LMX_GEN_ERROR_NO_CONNECTION                         = 0x1f, //!< Trying to use an inexistent connection
    LMX_GEN_ERROR_SPP_INVALID_PORT                      = 0x20, //!< Port number out of range
    LMX_GEN_ERROR_SPP_PORT_NOT_OPEN                     = 0x21, //!< Port is closed
    LMX_GEN_ERROR_SPP_PORT_BUSY                         = 0x22, //!< Connection establishment on a PORT that has a connection
    LMX_GEN_ERROR_SPP_MULTIPLE_CONNECTIONS              = 0x23, //!< Transparent mode attempted while more than 1 connection active
    LMX_GEN_ERROR_SPP_MULTIPLE_TRANSPARENT              = 0x24, //!< Trying to store a default connection when a transparent default connection is already stored, or trying to store a transparent default connection when another connection is already stored
    LMX_GEN_ERROR_SPP_DEFAULT_CONNECTION_NOT_STORED     = 0x25, //!< Trying to connection to a default connection, which is not stored
    LMX_GEN_ERROR_SPP_AUTOMATIC_CONNECTIONS_PROGRESSING = 0x26, //!< Trying to start connecting to default connections when default connection establishment is already progressing
    LMX_GEN_ERROR_UNSPECIFIED_ERROR                     = 0x27, //!< Other error
    LMX_GEN_ERROR_IDENTIFIER_NOT_IN_USE                 = 0x28, //!< trying to enable a SDP record that is not stored
    LMX_GEN_ERROR_INVALID_SUPPORTED_FAXCLA              = 0x29, //!< SS_VALUE Faxclass parameter must be 0 or 1
    LMX_GEN_ERROR_TOO_MANY_SUPPORTED_FOR                = 0x2a, //!< MATS The given number of supported formats excesses the specified maximum number of supported formats
    LMX_GEN_ERROR_TOO_MANY_DATASTORES                   = 0x2b, //!< The given number of data stores excess the specified maximum number of data stores
    LMX_GEN_ERROR_ATTEMPT_FAILED                        = 0x2c, //!< Attempt to change low power mode failed
    LMX_GEN_ERROR_ILLEGAL_LINK_POLICY                   = 0x2d, //!< The given link policy value is out of range
    LMX_GEN_ERROR_PINCODE_TOO_LONG                      = 0x2e, //!< PIN code string in the #LMX_PTYPE_REQ too long
    LMX_GEN_ERROR_PARITY_BIT_OUT_OF_RANGE               = 0x2f, //!< The given parity check is out of range
    LMX_GEN_ERROR_STOP_BITS_OUT_OF_RANGE                = 0x30, //!< The given number of stop bits is out of range
    LMX_GEN_ERROR_ILLEGAL_LINK_TIMEOUT                  = 0x31, //!< The given link timeout value is out of range
    LMX_GEN_ERROR_COMMAND_DISALLOWED                    = 0x32, //!< The command is not allowed
    LMX_GEN_ERROR_ILLEGAL_AUDIO_CODEC_TYPE              = 0x33, //!< The given Audio CODEC type is out of range
    LMX_GEN_ERROR_ILLEGAL_AUDIO_AIR_FORMAT              = 0x34, //!< The given Audio Air format is out of range
    LMX_GEN_ERROR_SDP_RECORD_TOO_LONG                   = 0x35, //!< The SDP record is too long
    LMX_GEN_ERROR_SDP_FAILED_TO_CREATE_REC              = 0x36, //!< ORD The SDP server failed to create the SDP record
    LMX_GEN_ERROR_SET_VOLUME_FAILED                     = 0x37, //!< The selected codec does not support volume control
    LMX_GEN_ERROR_ILLEGAL_PACKET_TYPE                   = 0x38, //!< The packet type specified in the request is not valid
    LMX_GEN_ERROR_INVALID_CODEC_SETTING                 = 0x39, //!< The codec (slave) settings is invalid
} LMX_GEN_ERROR_t;

//! LMX9830 RFCOMM status codes [LMX9830, pp. 200]
typedef enum LMX_RFCS_e
{
    LMX_RFCS_NO_ERROR             = 0x00, //!< No error
    LMX_RFCS_INVALID_DLC          = 0x01, //!< The DLC does not exist
    LMX_RFCS_INVALID_PORT         = 0x02, //!< The port does not exist
    LMX_RFCS_DLC_ESTABLISH_FAILED = 0x03, //!< The DLC establishment failed
    LMX_RFCS_ACCESS_REJECTED_SECM = 0x04, //!< did not authorize access to the requested service (DLC)
    LMX_RFCS_INVALID_CONNECTION   = 0x05, //!< There does not exist a DLC/L2CAP connection to the device
} LMX_RFCS_t;

//! LMX9830 RFCOMM release reasons [LMX9830, pp. 201]
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

//! event filter flags (#LMX_OPCODE_SET_EVENT_FILTER)
typedef enum LMX_EVENTFILTER_e
{
    LMX_EVENTFILTER_ALL   = 0x00, //!< all events reported
    LMX_EVENTFILTER_NOACL = 0x01, //!< no ACL link indicators (default)
    LMX_EVENTFILTER_NONE  = 0x02, //!< no events reported, UART break still generated and detected
    LMX_EVENTFILTER_NADA  = 0x03, //!< no events reported, UART break not generated or detected
} LMX_EVENTFILTER_t;

//! RSSI value (#LMX_OPCODE_READ_RSSI)
typedef enum LMX_RSSI_e
{
    LMX_RSSI_TOO_HIGH = 0x01,
    LMX_RSSI_GOOD     = 0x00,
    LMX_RSSI_TOO_LOW  = 0xff,
} LMX_RSSI_t;

//! LMX command frame size (incl. sync chars)
#define LMX_FRAME_SIZE     (1 + 1 + 1 + 2 + 1 + 1)

//! LMX command payload offset in message
#define LMX_PAYLOAD_OFFSET (1 + 1 + 1 + 2 + 1)

//! stringify packet type
/*!
    \param[in] ptype  packet type identifier

    \returns the description string
*/
PGM_P /* const char * */lmxPtypeStr(const LMX_PTYPE_t ptype);

//! stringify opcode
/*!
    \param[in] opcode  opcode identifier

    \returns the description string
*/
PGM_P /* const char * */lmxOpcodeStr(const LMX_OPCODE_t opcode);

//! stringify ACL error code
/*!
    \param[in] error  error code

    \returns the description string
*/
PGM_P /* const char * */lmxAclErrorStr(const LMX_ACL_ERROR_t error);

//! stringify generic error code
/*!
    \param[in] error  error code

    \returns the description string
*/
PGM_P /* const char * */lmxGenErrorStr(const LMX_GEN_ERROR_t error);

//! stringify RFCOMM link release reson code
/*!
    \param[in] linkrelease  linkrelease code

    \returns the description string
*/
PGM_P /* const char * */lmxLinkreleaseStr(const LMX_RFCR_t linkrelease);

//! stringify RFCOMM status code
/*!
    \param[in] status  status code

    \returns the description string
*/
PGM_P /* const char * */lmxRfcommStatusStr(const LMX_RFCS_t status);

//! stringify operation mode code
/*!
    \param[in] mode  mode identifier

    \returns the description string
*/
PGM_P /* const char * */lmxModeStr(const LMX_MODE_t mode);

//! stringify RSSI value
/*!
    \param[in] rssi  RSSI value

    \returns the RSSI value string
*/
PGM_P /* const char * */lmxRssiStr(const LMX_RSSI_t rssi);

#endif // __LMX9830_H__

//@}
// eof
