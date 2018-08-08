/*!
    \file
    \brief flipflip's Arduino Uno stuff: 2-wire serial interface (I2C) driver (see \ref I2C)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup I2C 2-wire serial interface (I2C) Driver
    \ingroup FF

    This implements a 2-wire serial interface (I2C) driver. It uses the following pins:

    - pin A4 is SDA (serial data)
    - pin A5 is SCL (serial clock)

    Configuration:
    - #FF_I2C_SPEED

    See \ref EXAMPLES_EX14 for a demonstration.

    @{
*/

#ifndef __I2C_H__
#define __I2C_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise things
void i2cInit(void);

typedef enum I2C_DIR_e { I2C_READ, I2C_WRITE } I2C_DIR_t;

bool i2cStart(const uint8_t addr, const I2C_DIR_t dir, uint32_t timeout);
void i2cStop(void);
bool i2cWrite(const uint8_t data);


/* *************************************************************************** */

#endif // __I2C_H__

//@}
// eof
