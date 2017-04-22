/*!
    \file
    \brief flipflip's Arduino Uno stuff: Arduino Uno pins (see \ref UNOPINS)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

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
    names (PD0, PD1, ...).

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
#define PIN_PORT_PD0  PORTD
#define PIN_DDR_PD0   DDRD
#define PIN_BIT_PD0   PD0
#define PIN_PIN_PD0   PIND

#define PIN_PORT_1    PORTD
#define PIN_DDR_1     DDRD
#define PIN_BIT_1     PD1
#define PIN_PIN_1     PIND
#define PIN_PORT_PD1  PORTD
#define PIN_DDR_PD1   DDRD
#define PIN_BIT_PD1   PD1
#define PIN_PIN_PD1   PIND

#define PIN_PORT_2    PORTD
#define PIN_DDR_2     DDRD
#define PIN_BIT_2     PD2
#define PIN_PIN_2     PIND
#define PIN_PORT_PD2  PORTD
#define PIN_DDR_PD2   DDRD
#define PIN_BIT_PD2   PD2
#define PIN_PIN_PD2   PIND

#define PIN_PORT_3    PORTD
#define PIN_DDR_3     DDRD
#define PIN_BIT_3     PD3
#define PIN_PIN_3     PIND
#define PIN_PORT_PD3  PORTD
#define PIN_DDR_PD3   DDRD
#define PIN_BIT_PD3   PD3
#define PIN_PIN_PD3   PIND

#define PIN_PORT_4    PORTD
#define PIN_DDR_4     DDRD
#define PIN_BIT_4     PD4
#define PIN_PIN_4     PIND
#define PIN_PORT_PD4  PORTD
#define PIN_DDR_PD4   DDRD
#define PIN_BIT_PD4   PD4
#define PIN_PIN_PD4   PIND

#define PIN_PORT_5    PORTD
#define PIN_DDR_5     DDRD
#define PIN_BIT_5     PD5
#define PIN_PIN_5     PIND
#define PIN_PORT_PD5  PORTD
#define PIN_DDR_PD5   DDRD
#define PIN_BIT_PD5   PD5
#define PIN_PIN_PD5   PIND

#define PIN_PORT_6    PORTD
#define PIN_DDR_6     DDRD
#define PIN_BIT_6     PD6
#define PIN_PIN_6     PIND
#define PIN_PORT_PD6  PORTD
#define PIN_DDR_PD6   DDRD
#define PIN_BIT_PD6   PD6
#define PIN_PIN_PD6   PIND

#define PIN_PORT_7    PORTD
#define PIN_DDR_7     DDRD
#define PIN_BIT_7     PD7
#define PIN_PIN_7     PIND
#define PIN_PORT_PD7  PORTD
#define PIN_DDR_PD7   DDRD
#define PIN_BIT_PD7   PD7
#define PIN_PIN_PD7   PIND

#define PIN_PORT_8    PORTB
#define PIN_DDR_8     DDRB
#define PIN_BIT_8     PB0
#define PIN_PIN_8     PINB
#define PIN_PORT_PB0  PORTB
#define PIN_DDR_PB0   DDRB
#define PIN_BIT_PB0   PB0
#define PIN_PIN_PB0   PINB

#define PIN_PORT_9    PORTB
#define PIN_DDR_9     DDRB
#define PIN_BIT_9     PB1
#define PIN_PIN_9     PINB
#define PIN_PORT_PB1  PORTB
#define PIN_DDR_PB1   DDRB
#define PIN_BIT_PB1   PB1
#define PIN_PIN_PB1   PINB

#define PIN_PORT_10   PORTB
#define PIN_DDR_10    DDRB
#define PIN_BIT_10    PB2
#define PIN_PIN_10    PINB
#define PIN_PORT_PB2  PORTB
#define PIN_DDR_PB2   DDRB
#define PIN_BIT_PB2   PB2
#define PIN_PIN_PB2   PINB

#define PIN_PORT_11   PORTB
#define PIN_DDR_11    DDRB
#define PIN_BIT_11    PB3
#define PIN_PIN_11    PINB
#define PIN_PORT_PB3  PORTB
#define PIN_DDR_PB3   DDRB
#define PIN_BIT_PB3   PB3
#define PIN_PIN_PB3   PINB

#define PIN_PORT_12   PORTB
#define PIN_DDR_12    DDRB
#define PIN_BIT_12    PB4
#define PIN_PIN_12    PINB
#define PIN_PORT_PB4  PORTB
#define PIN_DDR_PB4   DDRB
#define PIN_BIT_PB4   PB4
#define PIN_PIN_PB4   PINB

#define PIN_PORT_13   PORTB
#define PIN_DDR_13    DDRB
#define PIN_BIT_13    PB5
#define PIN_PIN_13    PINB
#define PIN_PORT_PB5  PORTB
#define PIN_DDR_PB5   DDRB
#define PIN_BIT_PB5   PB5
#define PIN_PIN_PB5   PINB

#define PIN_PORT_A0   PORTC
#define PIN_DDR_A0    DDRC
#define PIN_BIT_A0    PC0
#define PIN_PIN_A0    PINC
#define PIN_PORT_PC0  PORTC
#define PIN_DDR_PC0   DDRC
#define PIN_BIT_PC0   PC0
#define PIN_PIN_PC0   PINC

#define PIN_PORT_A1   PORTC
#define PIN_DDR_A1    DDRC
#define PIN_BIT_A1    PC1
#define PIN_PIN_A1    PINC
#define PIN_PORT_PC1  PORTC
#define PIN_DDR_PC1   DDRC
#define PIN_BIT_PC1   PC1
#define PIN_PIN_PC1   PINC

#define PIN_PORT_A2   PORTC
#define PIN_DDR_A2    DDRC
#define PIN_BIT_A2    PC2
#define PIN_PIN_A2    PINC
#define PIN_PORT_PC2  PORTC
#define PIN_DDR_PC2   DDRC
#define PIN_BIT_PC2   PC2
#define PIN_PIN_PC2   PINC

#define PIN_PORT_A3   PORTC
#define PIN_DDR_A3    DDRC
#define PIN_BIT_A3    PC3
#define PIN_PIN_A3    PINC
#define PIN_PORT_PC3  PORTC
#define PIN_DDR_PC3   DDRC
#define PIN_BIT_PC3   PC3
#define PIN_PIN_PC3   PINC

#define PIN_PORT_A4   PORTC
#define PIN_DDR_A4    DDRC
#define PIN_BIT_A4    PC4
#define PIN_PIN_A4    PINC
#define PIN_PORT_PC4  PORTC
#define PIN_DDR_PC4   DDRC
#define PIN_BIT_PC4   PC4
#define PIN_PIN_PC4   PINC

#define PIN_PORT_A5   PORTC
#define PIN_DDR_A5    DDRC
#define PIN_BIT_A5    PC5
#define PIN_PIN_A5    PINC
#define PIN_PORT_PC5  PORTC
#define PIN_DDR_PC5   DDRC
#define PIN_BIT_PC5   PC5
#define PIN_PIN_PC5   PINC

#endif // __UNOPINS_H__

//@}
// eof
