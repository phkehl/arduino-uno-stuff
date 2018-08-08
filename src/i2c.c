/*!
    \file
    \brief flipflip's Arduino Uno stuff: 2-wire serial interface (I2C) driver (see \ref I2C)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup I2C
    @{

    \todo make clock speed configurable
*/

#include <util/twi.h>      // avr-libc: TWI bit mask definitions

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "i2c.h"           // ff: i2c driver


/* ************************************************************************** */

void i2cInit(void)
{
    DEBUG("i2c: init");

    // initialise to 100kHz
    // - F_SCL = F_CPU / (16 + (2 * TWBR) * prescaler);
    TWSR = 0; // prescaler = 1 (TWPS1 = 0, TWPS0 = 0)
    TWBR = ((F_CPU / 100000) - 16) / (2 * 1);
}

bool i2cStart(const uint8_t addr, const I2C_DIR_t dir, uint32_t timeout)
{
    bool res = false;

    if (osTaskIsSchedulerRunning())
    {
        timeout += osTaskGetMsss();
    }

    while (ENDLESS)
    {
        // send start condition
        TWCR = BIT(TWINT) | BIT(TWSTA) | BIT(TWEN);

        // wait for transmission to complete
        loop_until_bit_is_set(TWCR, TWINT);

        // check status
        const uint8_t twst1 = TW_STATUS;
        switch (twst1)
        {
            // success: (repeated) start condition transmitted
            case TW_REP_START:
            case TW_START:
                break;

                // lost arbitration, cease talking
            case TW_MT_ARB_LOST:
                // not in start condition (must not send stop condition)
            default:
                continue; // try again
        }

        // send target device address and transfer direction
        const uint8_t sla = (addr << 1) | (dir == I2C_WRITE ? TW_WRITE : TW_READ);
        TWDR = sla;                     // load
        TWCR = BIT(TWINT) | BIT(TWEN);  // initiate

        // wait for transmission to complete
        loop_until_bit_is_set(TWCR, TWINT);

        // check status
        const uint8_t twst2 = TW_STATUS;
        switch (twst2)
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
                continue;

                // lost arbitration, cease talking
            case TW_MT_ARB_LOST:
                // error (but must send stop condition)
            default:
                continue;
        }

        // success?
        if (res)
        {
            break;
        }

        // wait a bit and consider timeout to give up
        if (osTaskIsSchedulerRunning())
        {
            osTaskDelay(1);
            if (timeout != 0)
            {
                if (osTaskGetMsss() >= timeout)
                {
                    break;
                }
            }
        }
        else
        {
            hwDelay(1000);
            if (timeout != 0)
            {
                timeout--;
                if (timeout == 0)
                {
                    break;
                }
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


/* ************************************************************************** */

//@}
// eof
