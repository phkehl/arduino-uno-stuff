/*!
    \file
    \brief flipflip's Arduino Uno stuff: LMX9830 bluetooth chip (see \ref PROJECTS_FFPHONE_LMX9830)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    - All trademarks property of their respective owners.

    \addtogroup PROJECTS_FFPHONE_LMX9830
    @{
*/


#include <avr/pgmspace.h>  // avr: program space utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "debug.h"         // ff: debugging

#include "lmx9830.h"


PGM_P /* const char * */lmxPtypeStr(const LMX_PTYPE_t ptype)
{
    switch (ptype)
    {
        case LMX_PTYPE_REQ:            return PSTR("REQ");
        case LMX_PTYPE_CFM:            return PSTR("CFM");
        case LMX_PTYPE_IND:            return PSTR("IND");
        case LMX_PTYPE_RES:            return PSTR("RES");
        default:                       return PSTR("UNK_PTYPE");
    }
}

PGM_P /* const char * */lmxOpcodeStr(const LMX_OPCODE_t opcode)
{
    switch (opcode)
    {
        case LMX_OPCODE_NONE:                            return PSTR("NONE");
        case LMX_OPCODE_RESET:                           return PSTR("RESET");
        case LMX_OPCODE_DEVICE_READY:                    return PSTR("DEVICE_READY");
        case LMX_OPCODE_RESTORE_FACTORY_SETTINGS:        return PSTR("RESTORE_FACTORY_SETTINGS");
        case LMX_OPCODE_STORE_CLASS_OF_DEVICE:           return PSTR("STORE_CLASS_OF_DEVICE");
        case LMX_OPCODE_GAP_WRITE_LOCAL_NAME:            return PSTR("GAP_WRITE_LOCAL_NAME");
        case LMX_OPCODE_GAP_READ_LOCAL_NAME:             return PSTR("GAP_READ_LOCAL_NAME");
        case LMX_OPCODE_GAP_READ_LOCAL_BDA:              return PSTR("GAP_READ_LOCAL_BDA");
        case LMX_OPCODE_GAP_SET_FIXED_PIN:               return PSTR("GAP_SET_FIXED_PIN");
        case LMX_OPCODE_SPP_INCOMING_LINK_ESTABLISHED:   return PSTR("SPP_INCOMING_LINK_ESTABLISHED");
        case LMX_OPCODE_SPP_LINK_RELEASED:               return PSTR("SPP_LINK_RELEASED");
        case LMX_OPCODE_SPP_TRANSPARENT_MODE:            return PSTR("SPP_TRANSPARENT_MODE");
        case LMX_OPCODE_SET_DEFAULT_AUDIO_CONFIG:        return PSTR("SET_DEFAULT_AUDIO_CONFIG");
        case LMX_OPCODE_DELETE_SDP_RECORDS:              return PSTR("DELETE_SDP_RECORDS");
        case LMX_OPCODE_STORE_SDP_RECORD:                return PSTR("STORE_SDP_RECORD");
        case LMX_OPCODE_ENABLE_SDP_RECORD:               return PSTR("ENABLE_SDP_RECORD");
        case LMX_OPCODE_SET_PORTS_TO_OPEN:               return PSTR("SET_PORTS_TO_OPEN");
        case LMX_OPCODE_GAP_LIST_PAIRED_DEVICES:         return PSTR("GAP_LIST_PAIRED_DEVICES");
        case LMX_OPCODE_GAP_REMOTE_DEVICE_NAME:          return PSTR("GAP_REMOTE_DEVICE_NAME");
        case LMX_OPCODE_SPP_SEND_DATA:                   return PSTR("SPP_SEND_DATA");
        case LMX_OPCODE_READ_RSSI:                       return PSTR("READ_RSSI");
        case LMX_OPCODE_SET_EVENT_FILTER:                return PSTR("SET_EVENT_FILTER");
        case LMX_OPCODE_GAP_ACL_ESTABLISHED:             return PSTR("GAP_ACL_ESTABLISHED");
        case LMX_OPCODE_GAP_ACL_TERMINATED:              return PSTR("GAP_ACL_TERMINATED");
        case LMX_OPCODE_GAP_POWER_SAVE_MODE_CHANGED:     return PSTR("GAP_POWER_SAVE_MODE_CHANGED");
        case LMX_OPCODE_SPP_ESTABLISH_LINK:              return PSTR("SPP_ESTABLISH_LINK");
        case LMX_OPCODE_SPP_LINK_ESTABLISHED:            return PSTR("SPP_LINK_ESTABLISHED");
        case LMX_OPCODE_SPP_RELEASE_LINK:                return PSTR("SPP_RELEASE_LINK");
        case LMX_OPCODE_SDAP_CONNECT:                    return PSTR("SDAP_CONNECT");
        case LMX_OPCODE_SDAP_DISCONNECT:                 return PSTR("SDAP_DISCONNECT");
        case LMX_OPCODE_SDAP_CONNECTION_LOST:            return PSTR("SDAP_CONNECTION_LOST");
        case LMX_OPCODE_SDAP_SERVICE_BROWSE:             return PSTR("SDAP_SERVICE_BROWSE");
        case LMX_OPCODE_SDAP_SERVICE_SEARCH:             return PSTR("SDAP_SERVICE_SEARCH");
        case LMX_OPCODE_SDAP_SERVICE_REQUEST:            return PSTR("SDAP_SERVICE_REQUEST");
        case LMX_OPCODE_SDAP_ATTRIBUTE_REQUEST:          return PSTR("SDAP_ATTRIBUTE_REQUEST");
    }
    return PSTR("UNKN_OPCODE");
}

PGM_P /* const char * */lmxRfcommStatusStr(const LMX_RFCS_t status)
{
    switch (status)
    {
        case LMX_RFCS_NO_ERROR:             return PSTR("NO_ERROR");
        case LMX_RFCS_INVALID_DLC:          return PSTR("INVALID_DLC");
        case LMX_RFCS_INVALID_PORT:         return PSTR("INVALID_PORT");
        case LMX_RFCS_DLC_ESTABLISH_FAILED: return PSTR("DLC_ESTABLISH_FAILED");
        case LMX_RFCS_ACCESS_REJECTED_SECM: return PSTR("ACCESS_REJECTED_SECM");
        case LMX_RFCS_INVALID_CONNECTION:   return PSTR("INVALID_CONNECTION");
    }
    return PSTR("ILL_RFCS");
}

PGM_P /* const char * */lmxLinkreleaseStr(const LMX_RFCR_t linkrelease)
{
    switch (linkrelease)
    {
        case LMX_RFCR_DLC_DISC_LOCAL_DEVICE:          return PSTR("DISC_LOCAL_DEVICE");
        case LMX_RFCR_DLC_DISC_REMOTE_DEVICE:         return PSTR("DISC_REMOTE_DEVICE");
        case LMX_RFCR_DLC_DISC_ACL_FAILURE:           return PSTR("DISC_ACL_FAILURE");
        case LMX_RFCR_DLC_DISC_LOWER_LAYER:           return PSTR("DISC_LOWER_LAYER");
    }
    return PSTR("ILL_RFCR");
}

PGM_P /* const char * */lmxModeStr(const LMX_MODE_t mode)
{
    switch (mode)
    {
        case LMX_MODE_COMMAND:     return PSTR("COMMAND");
        case LMX_MODE_TRANSPARENT: return PSTR("TRANSPARENT");
        case LMX_MODE_UNKNOWN:     return PSTR("UNKNOWN");
    }
    return PSTR("ILL_MODE");
}

PGM_P /* const char * */lmxGenErrorStr(const LMX_GEN_ERROR_t error)
{
    switch (error)
    {
        case LMX_GEN_ERROR_OK:                                    return PSTR("OK");
        case LMX_GEN_ERROR_INVALID_NO_OF_PARAMETERS:              return PSTR("INVALID_NO_OF_PARAMETERS");
        case LMX_GEN_ERROR_DURATION_OUT_OF_RANGE:                 return PSTR("DURATION_OUT_OF_RANGE");
        case LMX_GEN_ERROR_INVALID_MODE:                          return PSTR("INVALID_MODE");
        case LMX_GEN_ERROR_TIMEOUT:                               return PSTR("TIMEOUT");
        case LMX_GEN_ERROR_UNKNOWN_ERROR:                         return PSTR("UNKNOWN_ERROR");
        case LMX_GEN_ERROR_NAME_TOO_LONG:                         return PSTR("NAME_TOO_LONG");
        case LMX_GEN_ERROR_INVALID_DISCOVERABILITY_PARAMETER:     return PSTR("INVALID_DISCOVERABILITY_PARAMETER");
        case LMX_GEN_ERROR_INVALID_CONNECTABILITY_PARAMETER:      return PSTR("INVALID_CONNECTABILITY_PARAMETER");
        case LMX_GEN_ERROR_INVALID_SECURITY_MODE:                 return PSTR("INVALID_SECURITY_MODE");
        case LMX_GEN_ERROR_LINKKEY_DOES_NOT_EXISTS:               return PSTR("LINKKEY_DOES_NOT_EXISTS");
        case LMX_GEN_ERROR_CONNECTION_FAILED:                     return PSTR("CONNECTION_FAILED");
        case LMX_GEN_ERROR_TRUNCATED_ANSWER:                      return PSTR("TRUNCATED_ANSWER");
        case LMX_GEN_ERROR_RESULT_TOO_LARGE:                      return PSTR("RESULT_TOO_LARGE");
        case LMX_GEN_ERROR_NOT_POSSIBLE_TO_ENTER_TESTMODE:        return PSTR("NOT_POSSIBLE_TO_ENTER_TESTMODE");
        case LMX_GEN_ERROR_ILLEGAL_TESTMODE:                      return PSTR("ILLEGAL_TESTMODE");
        case LMX_GEN_ERROR_RESET_TO_NSC_BDADDRESS:                return PSTR("RESET_TO_NSC_BDADDRESS");
        case LMX_GEN_ERROR_UART_SPEED_OUT_OF_RANGE:               return PSTR("UART_SPEED_OUT_OF_RANGE");
        case LMX_GEN_ERROR_INVALID_PORT:                          return PSTR("INVALID_PORT");
        case LMX_GEN_ERROR_ILLEGAL_STATE_VALUE:                   return PSTR("ILLEGAL_STATE_VALUE");
        case LMX_GEN_ERROR_IDENTIFIER_OUT_OF_RANGE:               return PSTR("IDENTIFIER_OUT_OF_RANGE");
        case LMX_GEN_ERROR_RECORD_ALREADY_IN_SELECTED_STATE:      return PSTR("RECORD_ALREADY_IN_SELECTED_STATE");
        case LMX_GEN_ERROR_INVALID_AUTHENTICATION_VALUE:          return PSTR("INVALID_AUTHENTICATION_VALUE");
        case LMX_GEN_ERROR_INVALID_ENCRYPTION_VALUE:              return PSTR("INVALID_ENCRYPTION_VALUE");
        case LMX_GEN_ERROR_MAXIMUM_NO_OF_SERVICE_RECORDS_REACHED: return PSTR("MAXIMUM_NO_OF_SERVICE_RECORDS_REACHED");
        case LMX_GEN_ERROR_WRITING_TO_NVS:                        return PSTR("WRITING_TO_NVS");
        case LMX_GEN_ERROR_INVALID_ROLE:                          return PSTR("INVALID_ROLE");
        case LMX_GEN_ERROR_LIMIT:                                 return PSTR("LIMIT");
        case LMX_GEN_ERROR_UNEXPECTED:                            return PSTR("UNEXPECTED");
        case LMX_GEN_ERROR_UNABLE_TO_SEND:                        return PSTR("UNABLE_TO_SEND");
        case LMX_GEN_ERROR_CURRENTLY_NO_BUFFER:                   return PSTR("CURRENTLY_NO_BUFFER");
        case LMX_GEN_ERROR_NO_CONNECTION:                         return PSTR("NO_CONNECTION");
        case LMX_GEN_ERROR_SPP_INVALID_PORT:                      return PSTR("SPP_INVALID_PORT");
        case LMX_GEN_ERROR_SPP_PORT_NOT_OPEN:                     return PSTR("SPP_PORT_NOT_OPEN");
        case LMX_GEN_ERROR_SPP_PORT_BUSY:                         return PSTR("SPP_PORT_BUSY");
        case LMX_GEN_ERROR_SPP_MULTIPLE_CONNECTIONS:              return PSTR("SPP_MULTIPLE_CONNECTIONS");
        case LMX_GEN_ERROR_SPP_MULTIPLE_TRANSPARENT:              return PSTR("SPP_MULTIPLE_TRANSPARENT");
        case LMX_GEN_ERROR_SPP_DEFAULT_CONNECTION_NOT_STORED:     return PSTR("SPP_DEFAULT_CONNECTION_NOT_STORED");
        case LMX_GEN_ERROR_SPP_AUTOMATIC_CONNECTIONS_PROGRESSING: return PSTR("SPP_AUTOMATIC_CONNECTIONS_PROGRESSING");
        case LMX_GEN_ERROR_UNSPECIFIED_ERROR:                     return PSTR("UNSPECIFIED_ERROR");
        case LMX_GEN_ERROR_IDENTIFIER_NOT_IN_USE:                 return PSTR("IDENTIFIER_NOT_IN_USE");
        case LMX_GEN_ERROR_INVALID_SUPPORTED_FAXCLA:              return PSTR("INVALID_SUPPORTED_FAXCLA");
        case LMX_GEN_ERROR_TOO_MANY_SUPPORTED_FOR:                return PSTR("TOO_MANY_SUPPORTED_FOR");
        case LMX_GEN_ERROR_TOO_MANY_DATASTORES:                   return PSTR("TOO_MANY_DATASTORES");
        case LMX_GEN_ERROR_ATTEMPT_FAILED:                        return PSTR("ATTEMPT_FAILED");
        case LMX_GEN_ERROR_ILLEGAL_LINK_POLICY:                   return PSTR("ILLEGAL_LINK_POLICY");
        case LMX_GEN_ERROR_PINCODE_TOO_LONG:                      return PSTR("PINCODE_TOO_LONG");
        case LMX_GEN_ERROR_PARITY_BIT_OUT_OF_RANGE:               return PSTR("PARITY_BIT_OUT_OF_RANGE");
        case LMX_GEN_ERROR_STOP_BITS_OUT_OF_RANGE:                return PSTR("STOP_BITS_OUT_OF_RANGE");
        case LMX_GEN_ERROR_ILLEGAL_LINK_TIMEOUT:                  return PSTR("ILLEGAL_LINK_TIMEOUT");
        case LMX_GEN_ERROR_COMMAND_DISALLOWED:                    return PSTR("COMMAND_DISALLOWED");
        case LMX_GEN_ERROR_ILLEGAL_AUDIO_CODEC_TYPE:              return PSTR("ILLEGAL_AUDIO_CODEC_TYPE");
        case LMX_GEN_ERROR_ILLEGAL_AUDIO_AIR_FORMAT:              return PSTR("ILLEGAL_AUDIO_AIR_FORMAT");
        case LMX_GEN_ERROR_SDP_RECORD_TOO_LONG:                   return PSTR("SDP_RECORD_TOO_LONG");
        case LMX_GEN_ERROR_SDP_FAILED_TO_CREATE_REC:              return PSTR("SDP_FAILED_TO_CREATE_REC");
        case LMX_GEN_ERROR_SET_VOLUME_FAILED:                     return PSTR("SET_VOLUME_FAILED");
        case LMX_GEN_ERROR_ILLEGAL_PACKET_TYPE:                   return PSTR("ILLEGAL_PACKET_TYPE");
        case LMX_GEN_ERROR_INVALID_CODEC_SETTING:                 return PSTR("INVALID_CODEC_SETTING");
    }
    return PSTR("ILL_ERROR");
}

PGM_P /* const char * */lmxAclErrorStr(const LMX_ACL_ERROR_t error)
{
    switch (error)
    {
        case LMX_ACL_ERROR_OK:                 return PSTR("OK");
        case LMX_ACL_ERROR_UNKN_HCI_CMD:       return PSTR("UNKN_HCI_CMD");
        case LMX_ACL_ERROR_NO_CONN:            return PSTR("NO_CONN");
        case LMX_ACL_ERROR_HW_FAILURE:         return PSTR("HW_FAILURE");
        case LMX_ACL_ERROR_PG_TIMEOUT:         return PSTR("PG_TIMEOUT");
        case LMX_ACL_ERROR_AUTH_FAIL:          return PSTR("AUTH_FAIL");
        case LMX_ACL_ERROR_KEY_MISSING:        return PSTR("KEY_MISSING");
        case LMX_ACL_ERROR_MEM_FULL:           return PSTR("MEM_FULL");
        case LMX_ACL_ERROR_CONN_TIMEOUT:       return PSTR("CONN_TIMEOUT");
        case LMX_ACL_ERROR_MAX_CONN:           return PSTR("MAX_CONN");
        case LMX_ACL_ERROR_MAX_SCO_CONN:       return PSTR("MAX_SCO_CONN");
        case LMX_ACL_ERROR_CONN_EXISTS:        return PSTR("CONN_EXISTS");
        case LMX_ACL_ERROR_CMD_DISALLOW:       return PSTR("CMD_DISALLOW");
        case LMX_ACL_ERROR_REJECT_RSRC:        return PSTR("REJECT_RSRC");
        case LMX_ACL_ERROR_REJECT_SEC:         return PSTR("REJECT_SEC");
        case LMX_ACL_ERROR_REJECT_PERS:        return PSTR("REJECT_PERS");
        case LMX_ACL_ERROR_HOST_TIMEOUT:       return PSTR("HOST_TIMEOUT");
        case LMX_ACL_ERROR_UNSUPP_PARAM:       return PSTR("UNSUPP_PARAM");
        case LMX_ACL_ERROR_INVAL_CMD_PAR:      return PSTR("INVAL_CMD_PAR");
        case LMX_ACL_ERROR_CONN_USR_END:       return PSTR("CONN_USR_END");
        case LMX_ACL_ERROR_CONN_LOW_RES:       return PSTR("CONN_LOW_RES");
        case LMX_ACL_ERROR_CONN_POWEROFF:      return PSTR("CONN_POWEROFF");
        case LMX_ACL_ERROR_CONN_TERM_LOCAL:    return PSTR("CONN_TERM_LOCAL");
        case LMX_ACL_ERROR_REPEAT_ATTEMPT:     return PSTR("REPEAT_ATTEMPT");
        case LMX_ACL_ERROR_PAIR_NOT_ALLOW:     return PSTR("PAIR_NOT_ALLOW");
        case LMX_ACL_ERROR_UNKN_LPM_PDU:       return PSTR("UNKN_LPM_PDU");
        case LMX_ACL_ERROR_UNSUPP_REM_FEAT:    return PSTR("UNSUPP_REM_FEAT");
        case LMX_ACL_ERROR_SCO_OFFS_REJ:       return PSTR("SCO_OFFS_REJ");
        case LMX_ACL_ERROR_SCO_INT_REJ:        return PSTR("SCO_INT_REJ");
        case LMX_ACL_ERROR_SCO_AIRM_REJ:       return PSTR("SCO_AIRM_REJ");
        case LMX_ACL_ERROR_INV_LPM_PARAM:      return PSTR("INV_LPM_PARAM");
        case LMX_ACL_ERROR_UNSPEC_ERROR:       return PSTR("UNSPEC_ERROR");
        case LMX_ACL_ERROR_UNSUP_LPM_PARAM:    return PSTR("UNSUP_LPM_PARAM");
        case LMX_ACL_ERROR_ROLE_NOT_ALLOW:     return PSTR("ROLE_NOT_ALLOW");
        case LMX_ACL_ERROR_LPM_RESP_TIMEOUT:   return PSTR("LPM_RESP_TIMEOUT");
        case LMX_ACL_ERROR_LPM_TRANS_COLL:     return PSTR("LPM_TRANS_COLL");
        case LMX_ACL_ERROR_LPM_PDU_NOT_ALLOW:  return PSTR("LPM_PDU_NOT_ALLOW");
        case LMX_ACL_ERROR_ENC_MODE:           return PSTR("ENC_MODE");
        case LMX_ACL_ERROR_UNIT_KEY:           return PSTR("UNIT_KEY");
        case LMX_ACL_ERROR_QOS_UNSUPP:         return PSTR("QOS_UNSUPP");
        case LMX_ACL_ERROR_INST_PASSWD:        return PSTR("INST_PASSWD");
        case LMX_ACL_ERROR_PAIR_UNIT_KEY:      return PSTR("PAIR_UNIT_KEY");
        case LMX_ACL_ERROR_DIFF_TRANS_COLL:    return PSTR("DIFF_TRANS_COLL");
        case LMX_ACL_ERROR_RESERVED1:          return PSTR("RESERVED1");
        case LMX_ACL_ERROR_QOS_PARAM:          return PSTR("QOS_PARAM");
        case LMX_ACL_ERROR_QOS_REJECT:         return PSTR("QOS_REJECT");
        case LMX_ACL_ERROR_CH_CLASS_UNSUPP:    return PSTR("CH_CLASS_UNSUPP");
        case LMX_ACL_ERROR_INSUFF_SEC:         return PSTR("INSUFF_SEC");
        case LMX_ACL_ERROR_PARAM_RANGE:        return PSTR("PARAM_RANGE");
        case LMX_ACL_ERROR_RESERVED2:          return PSTR("RESERVED2");
        case LMX_ACL_ERROR_ROLE_SWITCH_PEND:   return PSTR("ROLE_SWITCH_PEND");
        case LMX_ACL_ERROR_RESERVED3:          return PSTR("RESERVED3");
        case LMX_ACL_ERROR_RES_SLOT_VIOL:      return PSTR("RES_SLOT_VIOL");
        case LMX_ACL_ERROR_ROLE_SWITCH_FAIL:   return PSTR("ROLE_SWITCH_FAIL");
    }
    return PSTR("ILL_ERROR");
}

PGM_P /* const char * */lmxRssiStr(const LMX_RSSI_t rssi)
{
    switch (rssi)
    {
        case LMX_RSSI_TOO_HIGH: return PSTR("TOO_HIGH");
        case LMX_RSSI_GOOD:     return PSTR("GOOD");
        case LMX_RSSI_TOO_LOW:  return PSTR("TOO_LOW");
    }
    return PSTR("ILL_RSSI");
}

PGM_P /* const char * */lmxPsmStr(const LMX_PSM_t psm)
{
    switch (psm)
    {
        case LMX_PSM_ACTIVE: return PSTR("ACTIVE");
        case LMX_PSM_HOLD:   return PSTR("HOLD");
        case LMX_PSM_SNIFF:  return PSTR("SNIFF");
        case LMX_PSM_PARK:   return PSTR("PARK");
    }
    return PSTR("ILL_PSM");
}

//@}
// eof
