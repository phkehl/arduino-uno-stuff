/*!
    \file
    \brief flipflip's Arduino Uno stuff: 2-wire serial interface (I2C) driver (see \ref I2C)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    Based on [TUTORIAL] Interrupt Driven TWI Interface for AVR (Part 1: MT/MR) by Chris Herring
    (http://www.chrisherring.net/all/tutorial-interrupt-driven-twi-interface-for-avr-part1/)

    \addtogroup I2C
    @{

    \todo use interrupts? http://www.chrisherring.net/all/tutorial-interrupt-driven-twi-interface-for-avr-part1/
*/

#include <string.h>        // libc: string operations

#include <util/twi.h>      // avr-libc: TWI bit mask definitions
#include <avr/interrupt.h> // avr: interrupt things
#include <util/delay.h>    // avr: convenience functions for busy-wait delay loops

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

static volatile uint32_t svI2cIsrCnt;
static volatile uint8_t svI2cIsrDebug[100];

typedef enum I2C_MODE_e
{
	I2C_MODE_READY,     // interface ready
    I2C_MODE_INIT,      // initialising
    I2C_MODE_REP_START, // repeated start sent
    I2C_MODE_MT,        // master transmitter
    I2C_MODE_MR,        // master receiver
} I2C_MODE_t;

static volatile I2C_MODE_t svI2cMode;
static volatile uint8_t    svI2cStatus;
static volatile bool       svI2cRepStart;
static volatile uint8_t    svI2cTxBuf[16];
static volatile uint8_t    svI2cTxBufIx;
static volatile uint8_t    svI2cTxBufLen;
static volatile uint8_t    svI2cRxBuf[16];
static volatile uint8_t    svI2cRxBufIx;
static volatile uint8_t    svI2cRxBufLen;

// status code that will not appear in TW_STATUS
#define I2C_NO_STATUS 0xff

// clear flag to continue, enable interrupt, enable interface, send start signal
#define I2C_SEND_START()    (TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWIE) | BIT(TWSTA))

// clear flag to continue, enable interrupt, enable interface, send stop signal
#define I2C_SEND_STOP()     (TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWIE) | BIT(TWSTO))

// clear flag to continue, enable interrupt, enable interface
#define I2C_SEND_TRANSMIT() (TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWIE))

// clear flag to continue, enable interrupt, enable interface, ack byte received
#define I2C_SEND_ACK()      (TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWIE) | BIT(TWEA))

// clear flag to continue, enable interrupt, enable interface, nack byte received
#define I2C_SEND_NACK()     (TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWIE))


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

        PIN_PULLUP_ON(_A4);
        PIN_PULLUP_ON(_A5);

        // set initial states
        svI2cMode     = I2C_MODE_READY;
        svI2cStatus   = I2C_NO_STATUS;
        svI2cRepStart = false;

        // enable interface and interrupt
        TWCR = BIT(TWIE) | BIT(TWEN);

        sInitDone = true;

        memset(svI2cIsrDebug, 0xff, sizeof(svI2cIsrDebug));
        uint8_t buf[] = { I2C_WRITE_ADDR(0x40), 0xfe };
        i2cTransmit(buf, sizeof(buf), false);

        //i2cStart(SI7021_I2C_ADDR, I2C_WRITE, 50);
        //i2cWrite(SI7021_CMD_RESET);
        //i2cStop();

    }
}


static bool sI2cIsReady(void)
{
    switch (svI2cMode)
    {
        case I2C_MODE_READY:
        case I2C_MODE_REP_START:
            return true;
        case I2C_MODE_INIT:
        case I2C_MODE_MT:
        case I2C_MODE_MR:
            return false;
    }
    return false;
}

bool i2cTransmit(const uint8_t *data, const uint8_t len, const bool cont)
{
    if (len > sizeof(svI2cTxBuf))
    {
        ERROR("i2c: %"PRIu8" > %"PRIu8, len, sizeof(svI2cTxBuf));
        return false;
    }
    DEBUG("i2c: tx %"PRIu8" 0x%02"PRIx8, SREG, len);


    //sei(); // not enabled at init!! (systick should not yet run, FIXME: can we have interrupts w/o systick? nope, we don't want that..)

    // wait until interface is ready
    while (!sI2cIsReady())
    {
        _delay_us(1);
    }

    // setup transmit buffer
    for (uint8_t ix = 0; ix < len; ix++)
    {
        svI2cTxBuf[ix] = data[ix];
    }
    svI2cTxBufIx = 0;
    svI2cTxBufLen = len;

    svI2cRepStart = cont;

    // repeated start was sent
    if (svI2cMode == I2C_MODE_REP_START)
    {
        svI2cMode = I2C_MODE_INIT;
        TWDR = svI2cTxBuf[svI2cTxBufIx];
        svI2cTxBufIx++;
        I2C_SEND_TRANSMIT();
    }
    // normal start of transmission
    else
    {
        svI2cMode = I2C_MODE_INIT;
        I2C_SEND_START();
    }

    // FIXME: wait for completion
    _delay_ms(1500);
    DEBUG("i2c: done 0x%02"PRIx8" %"PRIu8" (%"PRIu32"): %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8" %02"PRIx8,
        svI2cStatus, (uint8_t)svI2cMode, svI2cIsrCnt,
        svI2cIsrDebug[0], svI2cIsrDebug[1], svI2cIsrDebug[2], svI2cIsrDebug[3], svI2cIsrDebug[4]);

    return true;
}


ISR(TWI_vect)
{
    osIsrEnter();

    const uint8_t status = TW_STATUS;

    svI2cIsrDebug[svI2cIsrCnt++] = status;
    svI2cIsrCnt %= NUMOF(svI2cIsrDebug);

	switch (status)
	{
        // ***** master transmitter or writing address *****

        // SLA+W transmitted, ACK received
        case TW_MT_SLA_ACK:
            svI2cMode = I2C_MODE_MT;
            // ...

        // start condition transmitted
		case TW_START:
            // ...

        // data transmitted, ACK received
		case TW_MT_DATA_ACK:
            // more data to send
			if (svI2cRxBufIx < svI2cRxBufLen)
			{
				TWDR = svI2cTxBuf[svI2cTxBufIx];
                svI2cTxBufIx++;
                svI2cStatus = TW_NO_INFO;
                I2C_SEND_TRANSMIT();
			}
            // transmission complete, but more to go
			else if (svI2cRepStart)
			{
                svI2cStatus = TW_NO_INFO;
                I2C_SEND_START();
			}
			// transmission complete, nothing more to go, exit
			else
			{
                svI2cMode = I2C_MODE_READY;
                svI2cStatus = I2C_NO_STATUS;
                I2C_SEND_STOP();
			}
			break;

        // ***** master receiver *****

        // SLA+R transmitted, ACK received
        case TW_MR_SLA_ACK:
            svI2cMode = I2C_MODE_MR;
            // ...

			// ack, more data to receive
			if (svI2cRxBufIx < (svI2cRxBufLen - 1))
			{
                svI2cStatus = TW_NO_INFO;
                I2C_SEND_ACK();
			}
            // nack, last byte
			else
			{
                svI2cStatus = TW_NO_INFO;
                I2C_SEND_NACK();
			}
			break;

        // data received, ACK returned
		case TW_MR_DATA_ACK:

            svI2cRxBuf[svI2cRxBufIx] = TWDR;
            svI2cRxBufIx++;

            // ack, more data to receive
			if (svI2cRxBufIx < (svI2cRxBufLen - 1))
			{
                svI2cStatus = TW_NO_INFO;
                I2C_SEND_ACK();
			}
            // nack, last byte
			else
			{
                svI2cStatus = TW_NO_INFO;
                I2C_SEND_NACK();
			}
			break;

        // data received, NACK returned
        case TW_MR_DATA_NACK:

            svI2cRxBuf[svI2cRxBufIx] = TWDR;
            svI2cRxBufIx++;

            // transmission complete, but do not release bus yet
            if (svI2cRepStart)
            {
                svI2cStatus = I2C_NO_STATUS;
                I2C_SEND_START();
            }
            // transmission completed, release bus
            else
            {
                svI2cMode = I2C_MODE_READY;
                svI2cStatus = I2C_NO_STATUS;
                I2C_SEND_STOP();
            }
            break;

        // ***** master transmit and receive common *****

        // SLA+R transmitted, NACK received
        case TW_MR_SLA_NACK:
        // SLA+W transmitted, NACK received
        case TW_MT_SLA_NACK:
        // data transmitted, NACK received 
        case TW_MT_DATA_NACK:
        // arbitration lost in SLA+W or data
        case TW_MT_ARB_LOST:
            // error (trigger bus error??)
            if (svI2cRepStart)
            {
                svI2cStatus = status;
                I2C_SEND_START();
            }
            // transmission complete
            else
            {
                svI2cMode = I2C_MODE_READY;
                svI2cStatus = I2C_NO_STATUS;
                I2C_SEND_STOP();
            }
			break;

        // repeated start condition transmitted
        case TW_REP_START:
            svI2cMode = I2C_MODE_REP_START;
            break;

        // illegal start or stop condition
        case TW_BUS_ERROR:
            svI2cStatus = status;
            svI2cMode = I2C_MODE_READY;
            I2C_SEND_STOP();
            break;

        // ***** slave receiver *****

        // not implemented

        // ***** slave transmitter *****

        // not implemented

        // ***** misc *****

        case I2C_NO_STATUS:
            break;
	}

    //osSemaphoreGive(&sI2cComplete, true);

    osIsrLeave();
}

uint32_t i2cCnt(void)
{
    return svI2cIsrCnt;
}





/* ************************************************************************** */

//@}
// eof
