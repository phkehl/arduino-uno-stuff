/*!
    \file
    \brief flipflip's Arduino Uno stuff: Silicon Labs Si 7021 A20 I2C humidity and temperature sensor driver (see \ref SI7021)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup SI7021
    @{

*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "i2c.h"           // ff: i2c driver
#include "si7021.h"        // ff: Si7021 driver


/* ************************************************************************** */

#define SI7021_I2C_ADDR              0x40  // 7bit address of the Si7021 chip
#define SI7021_CMD_RESET             0xfe  // reset command
#define SI7021_CMD_READ_RHT_REG      0xe7  // read RH/T user register 1
#define SI7021_CMD_RHUM_NOHOLD       0xf5  // measure relative humidity, no hold master mode
#define SI7021_CMD_TEMP_NOHOLD       0xf3  // measure temperature, no hold master mode
#define SI7021_CMD_TEMP_PREV         0xe0  // read temperature value from previous RH measurement
#define SI7021_CMD_READ_ID1_0        0xfa  // read electronic ID 1
#define SI7021_CMD_READ_ID1_1        0x0f
#define SI7021_CMD_READ_ID2_0        0xfc  // read electronic ID 2
#define SI7021_CMD_READ_ID2_1        0xc9
#define SI7021_CMD_READ_FWVER_0      0x84  // read firmware version
#define SI7021_CMD_READ_FWVER_1      0xb8
//#define SI7021_CMD_


static void sSi7021Delay(const uint32_t ms)
{
    if (osTaskIsSchedulerRunning())
    {
        osTaskDelay(ms);
    }
    else
    {
        hwDelay(ms * 1000);
    }
}

static bool sSi7021Ready = false;

void si7021Init(void)
{
    i2cInit();

    // reset
    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_RESET);
    i2cStop();

    sSi7021Delay(25);

    // read user register 1
    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_READ_RHT_REG);
    i2cStart(SI7021_I2C_ADDR, I2C_READ, 1);
    uint8_t reg;
    i2cRead(1, &reg);
    i2cStop();
    DEBUG("reg=0x%"PRIx8" 0x%"PRIx8, reg, reg & 0xc5);

    // no or wrong sensor, see datasheet register description
    if (reg != 0x3a)
    {
        ERROR("si7021: reg=0x"PRIx8, reg);
        return;
    }
    else
    {
        sSi7021Ready = true;
    }

    sSi7021Delay(25);

    // read serial number (for fun)
    uint8_t raw[8];

    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_READ_ID1_0);
    i2cWrite(SI7021_CMD_READ_ID1_1);
    i2cStart(SI7021_I2C_ADDR, I2C_READ, 1);
    i2cRead(8, raw);
    i2cStop();
    //DEBUG("raw %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8,
    //    raw[0], raw[1], raw[2], raw[3], raw[4], raw[5], raw[6], raw[7]);
    const uint32_t sn1 = ((uint32_t)raw[0] << 24) | ((uint32_t)raw[2] << 16) |
        ((uint32_t)raw[4] << 8) | (uint32_t)raw[6];

    sSi7021Delay(25);

    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_READ_ID2_0);
    i2cWrite(SI7021_CMD_READ_ID2_1);
    i2cStart(SI7021_I2C_ADDR, I2C_READ, 1);
    i2cRead(6, raw);
    i2cStop();
    //DEBUG("raw %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8,
    //    raw[0], raw[1], raw[2], raw[3], raw[4], raw[5]);
    // raw[0] should be 0x15 (=21) for the Si7021
    const uint32_t sn2 = ((uint32_t)raw[0] << 24) | ((uint32_t)raw[1] << 16) |
        ((uint32_t)raw[3] << 8) | (uint32_t)raw[4];

    // read firmware version (for fun)
    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_READ_FWVER_0);
    i2cWrite(SI7021_CMD_READ_FWVER_1);
    i2cStart(SI7021_I2C_ADDR, I2C_READ, 1);
    i2cRead(2, raw);
    i2cStop();
    //DEBUG("raw %02"PRIx8" %02"PRIx8, raw[0], raw[1]);
    const uint8_t fw = raw[0] == 0xff ? 0x10 : raw[0];

    DEBUG("si7021: init (%08"PRIx32", %08"PRIx32", v%"PRIu8".%"PRIu8")",
        sn1, sn2, fw >> 4, fw & 0x0f);
}

uint16_t si7021MeasHumidity(void)
{
    if (!sSi7021Ready)
    {
        return SI7021_NODATA;
    }
    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_RHUM_NOHOLD);
    osTaskDelay(25); // seems to be necessary
    i2cStart(SI7021_I2C_ADDR, I2C_READ, 1);
    uint8_t resp[3] = { 0 };
    const bool res = i2cRead(3, resp);
    i2cStop();
    //DEBUG("resp=0x%"PRIx8" 0x%"PRIx8" 0x%"PRIx8, resp[0], resp[1], resp[2]);

    if (!res)
    {
        return SI7021_NODATA;
    }

    const uint16_t raw = ((uint16_t)resp[0] << 8) | resp[1];
    const uint16_t hum = (((uint32_t)raw * (125 * 100)) / 65536) - 600;

    return CLIP(hum, 0, 10000);
}

uint16_t si7021MeasTemperature(void)
{
    if (!sSi7021Ready)
    {
        return SI7021_NODATA;
    }
    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_TEMP_NOHOLD);
    osTaskDelay(25); // seems to be necessary
    i2cStart(SI7021_I2C_ADDR, I2C_READ, 1);
    uint8_t resp[3] = { 0 };
    const bool res = i2cRead(3, resp);
    i2cStop();
    //DEBUG("resp=0x%"PRIx8" 0x%"PRIx8" 0x%"PRIx8, resp[0], resp[1], resp[2]);

    if (!res)
    {
        return SI7021_NODATA;
    }

    const uint16_t raw = ((uint16_t)resp[0] << 8) | resp[1];
    const uint16_t temp = (((uint32_t)raw * (uint32_t)(175.72 * 100)) / 65536) - (uint32_t)(46.85 * 100);

    return temp;
}



bool si7021MeasHumidityAndTemperature(uint16_t *pHum, uint16_t *pTemp)
{
    if (!sSi7021Ready)
    {
        *pHum  = SI7021_NODATA;
        *pTemp = SI7021_NODATA;
        return false;
    }

    *pHum = si7021MeasHumidity();
    if (*pHum == SI7021_NODATA)
    {
        *pTemp = SI7021_NODATA;
        return false;
    }

    i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
    i2cWrite(SI7021_CMD_TEMP_PREV);
    i2cStart(SI7021_I2C_ADDR, I2C_READ, 1);
    uint8_t resp[3] = { 0 };
    const bool res = i2cRead(3, resp);
    i2cStop();
    //DEBUG("resp=0x%"PRIx8" 0x%"PRIx8" 0x%"PRIx8, resp[0], resp[1], resp[2]);

    if (!res)
    {
        *pTemp = SI7021_NODATA;
        return false;
    }

    const uint16_t raw = ((uint16_t)resp[0] << 8) | resp[1];
    *pTemp = (((uint32_t)raw * (uint32_t)(175.72 * 100)) / 65536) - (uint32_t)(46.85 * 100);

    return true;
}


/* ************************************************************************** */

//@}
// eof
