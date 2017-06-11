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


PGM_P /* const char * */lmxGetPtypeString(const LMX_PTYPE_t ptype)
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


PGM_P /* const char * */lmxGetOpcodeString(const LMX_OPCODE_t opcode)
{
    switch (opcode)
    {
        case LMX_OPCODE_RESET:                           return PSTR("RESET");
        case LMX_OPCODE_DEVICE_READY:                    return PSTR("DEVICE_READY");
        case LMX_OPCODE_RESTORE_FACTORY_SETTINGS:        return PSTR("RESTORE_FACTORY_SETTINGS");
        case LMX_OPCODE_STORE_CLASS_OF_DEVICE:           return PSTR("STORE_CLASS_OF_DEVICE");
        case LMX_OPCODE_GAP_WRITE_LOCAL_NAME:            return PSTR("GAP_WRITE_LOCAL_NAME");
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
        default:                                         return PSTR("UNKN_OPCODE");
    }
}

PGM_P /* const char * */lmxGetErrorString(const LMX_ERROR_t error)
{
    switch (error)
    {
        case LMX_ERROR_OK:                                  return PSTR("OK");
        case LMX_ERROR_INVALID_NO_OF_PARAMETERS:            return PSTR("INVALID_NO_OF_PARAMETERS");
        case LMX_ERROR_NAME_TOO_LONG:                       return PSTR("NAME_TOO_LONG");
        case LMX_ERROR_PINCODE_TOO_LONG:                    return PSTR("PINCODE_TOO_LONG");
        case LMX_ERROR_ILLEGAL_STATE_VALUE:                 return PSTR("ILLEGAL_STATE_VALUE");
        case LMX_ERROR_IDENTIFIER_OUT_OF_RANGE:             return PSTR("IDENTIFIER_OUT_OF_RANGE");
        case LMX_ERROR_RECORD_ALREADY_IN_SELECTED_STATE:    return PSTR("RECORD_ALREADY_IN_SELECTED_STATE");
        case LMX_ERROR_IDENTIFIER_NOT_IN_USE:               return PSTR("IDENTIFIER_NOT_IN_USE");
        case LMX_ERROR_INVALID_AUTHENTICATION_VALUE:        return PSTR("INVALID_AUTHENTICATION_VALUE");
        case LMX_ERROR_INVALID_ENCRYPTION_VALUE:            return PSTR("INVALID_ENCRYPTION_VALUE");
        case LMX_ERROR_INVALID_PORT:                        return PSTR("INVALID_PORT");
        case LMX_ERROR_TIMEOUT:                             return PSTR("TIMEOUT");
        default:                                            return PSTR("UNKN_ERROR");
    }
}


PGM_P /* const char * */lmxGetLinkreleaseString(const LMX_RFCR_t linkrelease)
{
    switch (linkrelease)
    {
        case LMX_RFCR_DLC_DISC_LOCAL_DEVICE:          return PSTR("DISC_LOCAL_DEVICE");
        case LMX_RFCR_DLC_DISC_REMOTE_DEVICE:         return PSTR("DISC_REMOTE_DEVICE");
        case LMX_RFCR_DLC_DISC_ACL_FAILURE:           return PSTR("DISC_ACL_FAILURE");
        case LMX_RFCR_DLC_DISC_LOWER_LAYER:           return PSTR("DISC_LOWER_LAYER");
        default:                                      return PSTR("UNKN_DISC");
    }
}

//@}
// eof
