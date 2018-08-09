/*!
    \file
    \brief flipflip's Arduino Uno stuff: Silicon Labs Si 7021 A20 I2C humidity and temperature sensor driver (see \ref SI7021)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup SI7021 Si7021 humidity and temperature sensor driver
    \ingroup FF

    This implements a driver for the Silicon Labs Si 7021 A20 I2C humidity and temperature sensor
    driver. It uses the \ref I2C driver.

    pins:

    - pin A4 is SDA (serial data)
    - pin A5 is SCL (serial clock)

    See \ref EXAMPLES_EX14 for a demonstration.

    References:
    - [SI7021](../Si7021-A20.pdf) Silicon Labs Si 7021 A20 Datasheet

    @{
*/

#ifndef __SI7021_H__
#define __SI7021_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise things
void si7021Init(void);

//! temperature or humidity value to indicate no data available
#define SI7021_NODATA 0xffff

//! measure relative humidity
/*!
    \returns the relative humidity value [0.01%], or #SI7021_NODATA on error
*/
uint16_t si7021MeasHumidity(void);

//! measure temperature
/*!
    \returns the temperature value [0.01C], or #SI7021_NODATA on error
*/
uint16_t si7021MeasTemperature(void);


//! measure relative humidity and temperature at the same time
/*!
    \param[out] pHum   the relative humidity value [0.01%], or #SI7021_NODATA on error
    \param[out] pTemp  the temperature value [0.01C], or #SI7021_NODATA on error
    \returns true if both measurements are available
*/
bool si7021MeasHumidityAndTemperature(uint16_t *pHum, uint16_t *pTemp);


#endif // __SI7021_H__

//@}
// eof
