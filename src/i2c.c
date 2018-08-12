/*!
    \file
    \brief flipflip's Arduino Uno stuff: 2-wire serial interface (I2C) driver (see \ref I2C)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup I2C
    @{

    \todo use interrupts? http://www.chrisherring.net/all/tutorial-interrupt-driven-twi-interface-for-avr-part1/
*/

#include <string.h>        // libc: string operations

#include <util/twi.h>      // avr-libc: TWI bit mask definitions

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "i2c.h"           // ff: i2c driver

// check configuration
#if ( (FF_I2C_MASTER_SPEED < 100) || (FF_I2C_MASTER_SPEED > 400) )
#  error Illegal value for FF_I2C_MASTER_SPEED!
#endif

/* ************************************************************************** */

void i2cInit(void)
{
    static bool sInitDone = false;

    if (!sInitDone)
    {
        DEBUG("i2c: init (%"PRIu16"kHz)", (uint16_t)FF_I2C_MASTER_SPEED);

        // initialise to 100kHz
        // - F_SCL = F_CPU / (16 + (2 * TWBR) * prescaler);
        TWSR = 0; // prescaler = 1 (TWPS1 = 0, TWPS0 = 0)
        TWBR = (((uint32_t)F_CPU / ((uint32_t)(FF_I2C_MASTER_SPEED) * 1000)) - 16) / (2 * 1);

        sInitDone = true;
    }
}

bool i2cStart(const uint8_t addr, const I2C_DIR_t dir, uint32_t timeout)
{
    bool res = false;

    while (ENDLESS)
    {
        if (res)
        {
            break;
        }

        // send start condition
        TWCR = BIT(TWINT) | BIT(TWSTA) | BIT(TWEN);

        // wait for transmission to complete
        loop_until_bit_is_set(TWCR, TWINT);

        // check status
        switch (TW_STATUS)
        {
            // success: (repeated) start condition transmitted
            case TW_REP_START:
            case TW_START:
                break;

                // lost arbitration, cease talking
            case TW_MT_ARB_LOST:
                // not in start condition (must not send stop condition)
            default:
                goto wait; // try again
        }

        // send target device address and transfer direction
        TWDR = (addr << 1) | (dir == I2C_WRITE ? TW_WRITE : TW_READ); // load
        TWCR = BIT(TWINT) | BIT(TWEN);  // initiate

        // wait for transmission to complete
        loop_until_bit_is_set(TWCR, TWINT);

        // check status
        switch (TW_STATUS)
        {
            // SLA+W transmitted, ACK received 
            case TW_MT_SLA_ACK:
                res = true;
                break;

                // SLA+W or SLA+R transmitted, NACK received (device busy)
            case TW_MT_SLA_NACK:
            case TW_MR_SLA_NACK:
                i2cStop();
                // retry
                goto wait;

                // lost arbitration, cease talking
            case TW_MT_ARB_LOST:
                // error (but must send stop condition)
            default:
                goto wait;
        }

    wait:

        // success?
        if (res)
        {
            break;
        }

        // timeout is approximate only
        if (timeout > 0)
        {
            timeout--;
            if (timeout == 0)
            {
                break;
            }
            if (osTaskIsSchedulerRunning())
            {
                osTaskDelay(1);
            }
            else
            {
                hwDelay(1000);
            }
        }
    }

    return res;
}

void i2cStop(void)
{
    // send stop condition
    TWCR = BIT(TWINT) | BIT(TWSTO) | BIT(TWEN);
    // wait until bus is released
    loop_until_bit_is_set(TWCR, TWSTO);
}

bool i2cWrite(const uint8_t data)
{
    bool res = false;
    TWDR = data;                    // load
    TWCR = BIT(TWINT) | BIT(TWEN);  // initiate

    // wait for transmission to complete
    loop_until_bit_is_set(TWCR, TWINT);

    // check status
    const uint8_t twst = TW_STATUS;
    switch (twst)
    {
        // data transmitted, ACK received
        case TW_MT_DATA_ACK:
            res = true;
            break;

        // data transmitted, NACK received 
        //case TW_MT_DATA_NACK:
        // lost arbitration, cease talking
        //case TW_MT_ARB_LOST:
        default:
            break;
    }
    return res;
}

bool i2cRead(const uint8_t num, uint8_t *pData)
{
    if (num < 1)
    {
        return false;
    }
    memset(pData, 0, num);

    const uint8_t maxIx = num - 1;
    bool res = true;
    for (uint8_t ix = 0; res && (ix <= maxIx); ix++)
    {
        //DEBUG("read %"PRIu8"/%"PRIu8, ix, maxIx);
        TWCR = BIT(TWINT) | BIT(TWEN) | ( ix < maxIx ? BIT(TWEA) : 0);
        // wait for transmission to complete
        loop_until_bit_is_set(TWCR, TWINT); // FIXME: can loop forever if no response?
        switch (TW_STATUS)
        {
            // data received, ACK returned
            case TW_MR_DATA_ACK:
                break;
            // data received, NACK returned
            case TW_MR_DATA_NACK:
                if (ix != maxIx)
                {
                    res = false;
                }
                break;
            default: // FIXME: good idea?
                res = false;
                break;
        }

        pData[ix] = TWDR;
    }

    return res;
}


/* ************************************************************************** */

//@}
// eof
