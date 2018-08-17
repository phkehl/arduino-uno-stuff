/*!
    \file
    \brief flipflip's Arduino Uno stuff: 2-wire serial interface (I2C) driver (see \ref I2C)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup I2C 2-wire serial interface (I2C) Driver
    \ingroup FF

    This implements a 2-wire serial interface (I2C) driver, master mode only. It uses the following
    pins:

    - pin A4 is SDA (serial data)
    - pin A5 is SCL (serial clock)

    Configuration:
    - #FF_I2C_MASTER_SPEED

    See \ref EXAMPLES_EX14 (and \ref SI7021) for a demonstration.

    @{
*/

#ifndef __I2C_H__
#define __I2C_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

uint32_t i2cCnt(void);

//! initialise things
void i2cInit(void);

//! read or write indication for i2cStart()
typedef enum I2C_DIR_e { I2C_READ, I2C_WRITE } I2C_DIR_t;

//! start transaction
/*!
    \param[in]  addr     slave device address
    \param[in]  dir      direction
    \param[in]  timeout  (approximate) timeout [ms], 0 = endless, 1 = repeated start
    \returns true if device is accessible
*/
bool i2cStart(const uint8_t addr, const I2C_DIR_t dir, uint32_t timeout);

//! terminate transaction
void i2cStop(void);

//! write one byte
/*!
    \param[in]  data  the byte to write
    \returns true unless an error was encountered (e.g. bus arbitration lost)
*/
bool i2cWrite(const uint8_t data);

//! read into buffer
/*!
    \param[in]  num    number of bytes to read
    \param[in]  pData  pointer to memory receiving the data (must be of at least size \c num)
    \returns true unless an error was encountered (e.g. bus arbitration lost)
*/
bool i2cRead(const uint8_t num, uint8_t *pData);


bool i2cTransmit(const uint8_t *data, const uint8_t len, const bool cont);

#define I2C_WRITE_ADDR(addr) ( (((addr) << 1) & 0xfe) | (0 /* = TW_WRITE */) )

/* *************************************************************************** */

#endif // __I2C_H__

//@}
// eof
