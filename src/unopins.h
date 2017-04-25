/*!
    \file
    \brief flipflip's Arduino Uno stuff: Arduino Uno pins (see \ref UNOPINS)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup UNOPINS Arduino Uno Pins
    \ingroup FF

    @{
*/

#ifndef __UNOPINS_H__
#define __UNOPINS_H__

#define PIN_TO_PORT(pin) CONCAT(PIN_PORT_,pin)
#define PIN_TO_DDR(pin)  CONCAT(PIN_DDR_,pin)
#define PIN_TO_PIN(pin)  CONCAT(PIN_PIN_,pin)
#define PIN_TO_BIT(pin)  CONCAT(PIN_BIT_,pin)
#define PIN_TO_MASK(pin) BIT( CONCAT(PIN_BIT_,pin) )

/*!
    \name Pin Manipulation Macros

    The \c pin arguments to these macros are the Arduino pin number (1, 2, ..., A0, ...) or the AVR
    names prefixed with an underscrore (_PD0, _PD1, ...).

    Examples:
\code{.c}
// Arduino pin 13 (built-in LED)
PIN_OUTPUT(13);
PIN_HIGH(13);

// Arduino pin A1
PIN_OUTPUT(A1);
PIN_LOW(A1);

// AVR pin PC5
PIN_OUTPUT(_PC5);
PIN_LOW(_PC5);

// Arduino pin 13 (built-in LED) = AVR pin PB5
#define LED_PIN _PB5
PIN_OUTPUT(LED_PIN);
PIN_HIGH(_LED_PIN);
\endcode

    @{
*/
//! configure pin for output \hideinitializer
#define PIN_OUTPUT(pin)     SETBITS( PIN_TO_DDR(pin), PIN_TO_MASK(pin) )
//! set output pin high \hideinitializer
#define PIN_HIGH(pin)       SETBITS( PIN_TO_PORT(pin), PIN_TO_MASK(pin) )
//! set output pin low \hideinitializer
#define PIN_LOW(pin)        CLRBITS( PIN_TO_PORT(pin), PIN_TO_MASK(pin) )
//! toggle output pin state \hideinitializer
#define PIN_TOGGLE(pin)     TOGBITS( PIN_TO_PORT(pin), PIN_TO_MASK(pin) )
//! configure pin for input \hideinitializer
#define PIN_INPUT(pin)      CLRBITS( PIN_TO_DDR(pin), PIN_TO_MASK(pin) )
//! enable pull-up on input pin \hideinitializer
#define PIN_PULLUP_ON(pin)  SETBITS( PIN_TO_PORT(pin), PIN_TO_MASK(pin) )
//! disable pull-up on input pin \hideinitializer
#define PIN_PULLUP_OFF(pin) CLRBITS( PIN_TO_PORT(pin), PIN_TO_MASK(pin) )
//! read input pin state \hideinitializer
#define PIN_GET(pin)        ( PIN_TO_PIN(pin) & PIN_TO_MASK(pin) ? true : false )
//@}

#define PIN_BIT_NONE  9999

#define PIN_PORT_0    PORTD
#define PIN_DDR_0     DDRD
#define PIN_BIT_0     PD0
#define PIN_PIN_0     PIND
#define PIN_PORT__PD0 PORTD
#define PIN_DDR__PD0  DDRD
#define PIN_BIT__PD0  PD0
#define PIN_PIN__PD0  PIND

#define PIN_PORT_1    PORTD
#define PIN_DDR_1     DDRD
#define PIN_BIT_1     PD1
#define PIN_PIN_1     PIND
#define PIN_PORT__PD1 PORTD
#define PIN_DDR__PD1  DDRD
#define PIN_BIT__PD1  PD1
#define PIN_PIN__PD1  PIND

#define PIN_PORT_2    PORTD
#define PIN_DDR_2     DDRD
#define PIN_BIT_2     PD2
#define PIN_PIN_2     PIND
#define PIN_PORT__PD2 PORTD
#define PIN_DDR__PD2  DDRD
#define PIN_BIT__PD2  PD2
#define PIN_PIN__PD2  PIND

#define PIN_PORT_3    PORTD
#define PIN_DDR_3     DDRD
#define PIN_BIT_3     PD3
#define PIN_PIN_3     PIND
#define PIN_PORT__PD3 PORTD
#define PIN_DDR__PD3  DDRD
#define PIN_BIT__PD3  PD3
#define PIN_PIN__PD3  PIND

#define PIN_PORT_4    PORTD
#define PIN_DDR_4     DDRD
#define PIN_BIT_4     PD4
#define PIN_PIN_4     PIND
#define PIN_PORT__PD4 PORTD
#define PIN_DDR__PD4  DDRD
#define PIN_BIT__PD4  PD4
#define PIN_PIN__PD4  PIND

#define PIN_PORT_5    PORTD
#define PIN_DDR_5     DDRD
#define PIN_BIT_5     PD5
#define PIN_PIN_5     PIND
#define PIN_PORT__PD5 PORTD
#define PIN_DDR__PD5  DDRD
#define PIN_BIT__PD5  PD5
#define PIN_PIN__PD5  PIND

#define PIN_PORT_6    PORTD
#define PIN_DDR_6     DDRD
#define PIN_BIT_6     PD6
#define PIN_PIN_6     PIND
#define PIN_PORT__PD6 PORTD
#define PIN_DDR__PD6  DDRD
#define PIN_BIT__PD6  PD6
#define PIN_PIN__PD6  PIND

#define PIN_PORT_7    PORTD
#define PIN_DDR_7     DDRD
#define PIN_BIT_7     PD7
#define PIN_PIN_7     PIND
#define PIN_PORT__PD7 PORTD
#define PIN_DDR__PD7  DDRD
#define PIN_BIT__PD7  PD7
#define PIN_PIN__PD7  PIND

#define PIN_PORT_8    PORTB
#define PIN_DDR_8     DDRB
#define PIN_BIT_8     PB0
#define PIN_PIN_8     PINB
#define PIN_PORT__PB0 PORTB
#define PIN_DDR__PB0  DDRB
#define PIN_BIT__PB0  PB0
#define PIN_PIN__PB0  PINB

#define PIN_PORT_9    PORTB
#define PIN_DDR_9     DDRB
#define PIN_BIT_9     PB1
#define PIN_PIN_9     PINB
#define PIN_PORT__PB1 PORTB
#define PIN_DDR__PB1  DDRB
#define PIN_BIT__PB1  PB1
#define PIN_PIN__PB1  PINB

#define PIN_PORT_10   PORTB
#define PIN_DDR_10    DDRB
#define PIN_BIT_10    PB2
#define PIN_PIN_10    PINB
#define PIN_PORT__PB2 PORTB
#define PIN_DDR__PB2  DDRB
#define PIN_BIT__PB2  PB2
#define PIN_PIN__PB2  PINB

#define PIN_PORT_11   PORTB
#define PIN_DDR_11    DDRB
#define PIN_BIT_11    PB3
#define PIN_PIN_11    PINB
#define PIN_PORT__PB3 PORTB
#define PIN_DDR__PB3  DDRB
#define PIN_BIT__PB3  PB3
#define PIN_PIN__PB3  PINB

#define PIN_PORT_12   PORTB
#define PIN_DDR_12    DDRB
#define PIN_BIT_12    PB4
#define PIN_PIN_12    PINB
#define PIN_PORT__PB4 PORTB
#define PIN_DDR__PB4  DDRB
#define PIN_BIT__PB4  PB4
#define PIN_PIN__PB4  PINB

#define PIN_PORT_13   PORTB
#define PIN_DDR_13    DDRB
#define PIN_BIT_13    PB5
#define PIN_PIN_13    PINB
#define PIN_PORT__PB5 PORTB
#define PIN_DDR__PB5  DDRB
#define PIN_BIT__PB5  PB5
#define PIN_PIN__PB5  PINB

#define PIN_PORT_A0   PORTC
#define PIN_DDR_A0    DDRC
#define PIN_BIT_A0    PC0
#define PIN_PIN_A0    PINC
#define PIN_PORT__PC0 PORTC
#define PIN_DDR__PC0  DDRC
#define PIN_BIT__PC0  PC0
#define PIN_PIN__PC0  PINC

#define PIN_PORT_A1   PORTC
#define PIN_DDR_A1    DDRC
#define PIN_BIT_A1    PC1
#define PIN_PIN_A1    PINC
#define PIN_PORT__PC1 PORTC
#define PIN_DDR__PC1  DDRC
#define PIN_BIT__PC1  PC1
#define PIN_PIN__PC1  PINC

#define PIN_PORT_A2   PORTC
#define PIN_DDR_A2    DDRC
#define PIN_BIT_A2    PC2
#define PIN_PIN_A2    PINC
#define PIN_PORT__PC2 PORTC
#define PIN_DDR__PC2  DDRC
#define PIN_BIT__PC2  PC2
#define PIN_PIN__PC2  PINC

#define PIN_PORT_A3   PORTC
#define PIN_DDR_A3    DDRC
#define PIN_BIT_A3    PC3
#define PIN_PIN_A3    PINC
#define PIN_PORT__PC3 PORTC
#define PIN_DDR__PC3  DDRC
#define PIN_BIT__PC3  PC3
#define PIN_PIN__PC3  PINC

#define PIN_PORT_A4   PORTC
#define PIN_DDR_A4    DDRC
#define PIN_BIT_A4    PC4
#define PIN_PIN_A4    PINC
#define PIN_PORT__PC4 PORTC
#define PIN_DDR__PC4  DDRC
#define PIN_BIT__PC4  PC4
#define PIN_PIN__PC4  PINC

#define PIN_PORT_A5   PORTC
#define PIN_DDR_A5    DDRC
#define PIN_BIT_A5    PC5
#define PIN_PIN_A5    PINC
#define PIN_PORT__PC5 PORTC
#define PIN_DDR__PC5  DDRC
#define PIN_BIT__PC5  PC5
#define PIN_PIN__PC5  PINC

#endif // __UNOPINS_H__

//@}
// eof
