/*!
    \file
    \brief flipflip's Arduino Uno stuff: Arduino Uno pins (see \ref ARDUINOPINS)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup ARDUINOPINS Arduino Pins
    \ingroup FF

    See \ref P_PINOUTS.

    @{
*/

#ifndef __ARDUINOPINS_H__
#define __ARDUINOPINS_H__

#include "stdstuff.h"      // ff: useful macros and types

#define PIN_TO_PORT(pin) CONCAT(PIN_PORT_,pin)
#define PIN_TO_DDR(pin)  CONCAT(PIN_DDR_,pin)
#define PIN_TO_PIN(pin)  CONCAT(PIN_PIN_,pin)
#define PIN_TO_BIT(pin)  CONCAT(PIN_BIT_,pin)
#define PIN_TO_MASK(pin) BIT( CONCAT(PIN_BIT_,pin) )

/*!
    \name Pin Manipulation Macros

    The \c pin arguments to these macros are the Arduino pin number or the AVR
    names prefixed with an underscrore:
    - Arduino: _D1, _D2, ..., _A0, ...
    - AVR: _PD0, _PD1, ...

    Examples:
\code{.c}
// Arduino pin 13 (built-in LED)
PIN_OUTPUT(_D13);
PIN_HIGH(_D13);

// Arduino pin A1
PIN_OUTPUT(_A1);
PIN_LOW(_A1);

// AVR pin PC5
PIN_OUTPUT(_PC5);
PIN_LOW(_PC5);

// Arduino pin 13 (built-in LED) = AVR pin PB5
#define LED_PIN _PB5
PIN_OUTPUT(LED_PIN);
PIN_HIGH(LED_PIN);
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
//! set pin state \hideinitializer
#define PIN_SET(pin, state) if (state) { PIN_HIGH(pin); } else { PIN_LOW(pin); }
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

//------------------------------------------------------------------------------


#ifdef __AVR_ATmega328P__

#  define PIN_PORT__D0  PORTD
#  define PIN_DDR__D0   DDRD
#  define PIN_BIT__D0   PD0
#  define PIN_PIN__D0   PIND
#  define PIN_PORT__PD0 PORTD
#  define PIN_DDR__PD0  DDRD
#  define PIN_BIT__PD0  PD0
#  define PIN_PIN__PD0  PIND

#  define PIN_PORT__D1  PORTD
#  define PIN_DDR__D1   DDRD
#  define PIN_BIT__D1   PD1
#  define PIN_PIN__D1   PIND
#  define PIN_PORT__PD1 PORTD
#  define PIN_DDR__PD1  DDRD
#  define PIN_BIT__PD1  PD1
#  define PIN_PIN__PD1  PIND

#  define PIN_PORT__D2  PORTD
#  define PIN_DDR__D2   DDRD
#  define PIN_BIT__D2   PD2
#  define PIN_PIN__D2   PIND
#  define PIN_PORT__PD2 PORTD
#  define PIN_DDR__PD2  DDRD
#  define PIN_BIT__PD2  PD2
#  define PIN_PIN__PD2  PIND

#  define PIN_PORT__D3  PORTD
#  define PIN_DDR__D3   DDRD
#  define PIN_BIT__D3   PD3
#  define PIN_PIN__D3   PIND
#  define PIN_PORT__PD3 PORTD
#  define PIN_DDR__PD3  DDRD
#  define PIN_BIT__PD3  PD3
#  define PIN_PIN__PD3  PIND

#  define PIN_PORT__D4  PORTD
#  define PIN_DDR__D4   DDRD
#  define PIN_BIT__D4   PD4
#  define PIN_PIN__D4   PIND
#  define PIN_PORT__PD4 PORTD
#  define PIN_DDR__PD4  DDRD
#  define PIN_BIT__PD4  PD4
#  define PIN_PIN__PD4  PIND

#  define PIN_PORT__D5  PORTD
#  define PIN_DDR__D5   DDRD
#  define PIN_BIT__D5   PD5
#  define PIN_PIN__D5   PIND
#  define PIN_PORT__PD5 PORTD
#  define PIN_DDR__PD5  DDRD
#  define PIN_BIT__PD5  PD5
#  define PIN_PIN__PD5  PIND

#  define PIN_PORT__D6  PORTD
#  define PIN_DDR__D6   DDRD
#  define PIN_BIT__D6   PD6
#  define PIN_PIN__D6   PIND
#  define PIN_PORT__PD6 PORTD
#  define PIN_DDR__PD6  DDRD
#  define PIN_BIT__PD6  PD6
#  define PIN_PIN__PD6  PIND

#  define PIN_PORT__D7  PORTD
#  define PIN_DDR__D7   DDRD
#  define PIN_BIT__D7   PD7
#  define PIN_PIN__D7   PIND
#  define PIN_PORT__PD7 PORTD
#  define PIN_DDR__PD7  DDRD
#  define PIN_BIT__PD7  PD7
#  define PIN_PIN__PD7  PIND

#  define PIN_PORT__D8  PORTB
#  define PIN_DDR__D8   DDRB
#  define PIN_BIT__D8   PB0
#  define PIN_PIN__D8   PINB
#  define PIN_PORT__PB0 PORTB
#  define PIN_DDR__PB0  DDRB
#  define PIN_BIT__PB0  PB0
#  define PIN_PIN__PB0  PINB

#  define PIN_PORT__D9  PORTB
#  define PIN_DDR__D9   DDRB
#  define PIN_BIT__D9   PB1
#  define PIN_PIN__D9   PINB
#  define PIN_PORT__PB1 PORTB
#  define PIN_DDR__PB1  DDRB
#  define PIN_BIT__PB1  PB1
#  define PIN_PIN__PB1  PINB

#  define PIN_PORT__D10 PORTB
#  define PIN_DDR__D10  DDRB
#  define PIN_BIT__D10  PB2
#  define PIN_PIN__D10  PINB
#  define PIN_PORT__PB2 PORTB
#  define PIN_DDR__PB2  DDRB
#  define PIN_BIT__PB2  PB2
#  define PIN_PIN__PB2  PINB

#  define PIN_PORT__D11  PORTB
#  define PIN_DDR__D11   DDRB
#  define PIN_BIT__D11   PB3
#  define PIN_PIN__D11   PINB
#  define PIN_PORT__PB3 PORTB
#  define PIN_DDR__PB3  DDRB
#  define PIN_BIT__PB3  PB3
#  define PIN_PIN__PB3  PINB

#  define PIN_PORT__D12  PORTB
#  define PIN_DDR__D12   DDRB
#  define PIN_BIT__D12   PB4
#  define PIN_PIN__D12   PINB
#  define PIN_PORT__PB4 PORTB
#  define PIN_DDR__PB4  DDRB
#  define PIN_BIT__PB4  PB4
#  define PIN_PIN__PB4  PINB

#  define PIN_PORT__D13  PORTB
#  define PIN_DDR__D13   DDRB
#  define PIN_BIT__D13   PB5
#  define PIN_PIN__D13   PINB
#  define PIN_PORT__PB5 PORTB
#  define PIN_DDR__PB5  DDRB
#  define PIN_BIT__PB5  PB5
#  define PIN_PIN__PB5  PINB

#  define PIN_PORT__A0  PORTC
#  define PIN_DDR__A0   DDRC
#  define PIN_BIT__A0   PC0
#  define PIN_PIN__A0   PINC
#  define PIN_PORT__PC0 PORTC
#  define PIN_DDR__PC0  DDRC
#  define PIN_BIT__PC0  PC0
#  define PIN_PIN__PC0  PINC

#  define PIN_PORT__A1  PORTC
#  define PIN_DDR__A1   DDRC
#  define PIN_BIT__A1   PC1
#  define PIN_PIN__A1   PINC
#  define PIN_PORT__PC1 PORTC
#  define PIN_DDR__PC1  DDRC
#  define PIN_BIT__PC1  PC1
#  define PIN_PIN__PC1  PINC

#  define PIN_PORT__A2  PORTC
#  define PIN_DDR__A2   DDRC
#  define PIN_BIT__A2   PC2
#  define PIN_PIN__A2   PINC
#  define PIN_PORT__PC2 PORTC
#  define PIN_DDR__PC2  DDRC
#  define PIN_BIT__PC2  PC2
#  define PIN_PIN__PC2  PINC

#  define PIN_PORT__A3  PORTC
#  define PIN_DDR__A3   DDRC
#  define PIN_BIT__A3   PC3
#  define PIN_PIN__A3   PINC
#  define PIN_PORT__PC3 PORTC
#  define PIN_DDR__PC3  DDRC
#  define PIN_BIT__PC3  PC3
#  define PIN_PIN__PC3  PINC

#  define PIN_PORT__A4  PORTC
#  define PIN_DDR__A4   DDRC
#  define PIN_BIT__A4   PC4
#  define PIN_PIN__A4   PINC
#  define PIN_PORT__PC4 PORTC
#  define PIN_DDR__PC4  DDRC
#  define PIN_BIT__PC4  PC4
#  define PIN_PIN__PC4  PINC

#  define PIN_PORT__A5  PORTC
#  define PIN_DDR__A5   DDRC
#  define PIN_BIT__A5   PC5
#  define PIN_PIN__A5   PINC
#  define PIN_PORT__PC5 PORTC
#  define PIN_DDR__PC5  DDRC
#  define PIN_BIT__PC5  PC5
#  define PIN_PIN__PC5  PINC

#endif // __AVR_ATmega328P__

//------------------------------------------------------------------------------


#ifdef __AVR_ATmega2560__

#  define PIN_PORT__D0    PORTE
#  define PIN_DDR__D0     DDRE
#  define PIN_BIT__D0     PE0
#  define PIN_PIN__D0     PINE
#  define PIN_PORT__PE0   PORTE
#  define PIN_DDR__PE0    DDRE
#  define PIN_BIT__PE0    PE0
#  define PIN_PIN__PE0    PINE

#  define PIN_PORT__D1    PORTE
#  define PIN_DDR__D1     DDRE
#  define PIN_BIT__D1     PE1
#  define PIN_PIN__D1     PINE
#  define PIN_PORT__PE1   PORTE
#  define PIN_DDR__PE1    DDRE
#  define PIN_BIT__PE1    PE1
#  define PIN_PIN__PE1    PINE

#  define PIN_PORT__D2    PORTE
#  define PIN_DDR__D2     DDRE
#  define PIN_BIT__D2     PE4
#  define PIN_PIN__D2     PINE
#  define PIN_PORT__PE4   PORTE
#  define PIN_DDR__PE4    DDRE
#  define PIN_BIT__PE4    PE4
#  define PIN_PIN__PE4    PINE

#  define PIN_PORT__D3    PORTE
#  define PIN_DDR__D3     DDRE
#  define PIN_BIT__D3     PE5
#  define PIN_PIN__D3     PINE
#  define PIN_PORT__PE5   PORTE
#  define PIN_DDR__PE5    DDRE
#  define PIN_BIT__PE5    PE5
#  define PIN_PIN__PE5    PINE

#  define PIN_PORT__D4    PORTG
#  define PIN_DDR__D4     DDRG
#  define PIN_BIT__D4     PG5
#  define PIN_PIN__D4     PING
#  define PIN_PORT__PG5   PORTG
#  define PIN_DDR__PG5    DDRG
#  define PIN_BIT__PG5    PG5
#  define PIN_PIN__PG5    PING

#  define PIN_PORT__D5    PORTE
#  define PIN_DDR__D5     DDRE
#  define PIN_BIT__D5     PE3
#  define PIN_PIN__D5     PINE
#  define PIN_PORT__PE3   PORTE
#  define PIN_DDR__PE3    DDRE
#  define PIN_BIT__PE3    PE3
#  define PIN_PIN__PE3    PINE

#  define PIN_PORT__D6    PORTH
#  define PIN_DDR__D6     DDRH
#  define PIN_BIT__D6     PH3
#  define PIN_PIN__D6     PINH
#  define PIN_PORT__PH3   PORTH
#  define PIN_DDR__PH3    DDRH
#  define PIN_BIT__PH3    PH3
#  define PIN_PIN__PH3    PINH

#  define PIN_PORT__D7    PORTH
#  define PIN_DDR__D7     DDRH
#  define PIN_BIT__D7     PH4
#  define PIN_PIN__D7     PINH
#  define PIN_PORT__PH4   PORTH
#  define PIN_DDR__PH4    DDRH
#  define PIN_BIT__PH4    PH4
#  define PIN_PIN__PH4    PINH

#  define PIN_PORT__D8    PORTH
#  define PIN_DDR__D8     DDRH
#  define PIN_BIT__D8     PH5
#  define PIN_PIN__D8     PINH
#  define PIN_PORT__PH5   PORTH
#  define PIN_DDR__PH5    DDRH
#  define PIN_BIT__PH5    PH5
#  define PIN_PIN__PH5    PINH

#  define PIN_PORT__D9    PORTH
#  define PIN_DDR__D9     DDRH
#  define PIN_BIT__D9     PH6
#  define PIN_PIN__D9     PINH
#  define PIN_PORT__PH6   PORTH
#  define PIN_DDR__PH6    DDRH
#  define PIN_BIT__PH6    PH6
#  define PIN_PIN__PH6    PINH

#  define PIN_PORT__D10   PORTB
#  define PIN_DDR__D10    DDRB
#  define PIN_BIT__D10    PB4
#  define PIN_PIN__D10    PINB
#  define PIN_PORT__PB4   PORTB
#  define PIN_DDR__PB4    DDRB
#  define PIN_BIT__PB4    PB4
#  define PIN_PIN__PB4    PINB

#  define PIN_PORT__D11   PORTB
#  define PIN_DDR__D11    DDRB
#  define PIN_BIT__D11    PB5
#  define PIN_PIN__D11    PINB
#  define PIN_PORT__PB5   PORTB
#  define PIN_DDR__PB5    DDRB
#  define PIN_BIT__PB5    PB5
#  define PIN_PIN__PB5    PINB

#  define PIN_PORT__D12   PORTB
#  define PIN_DDR__D12    DDRB
#  define PIN_BIT__D12    PB6
#  define PIN_PIN__D12    PINB
#  define PIN_PORT__PB6   PORTB
#  define PIN_DDR__PB6    DDRB
#  define PIN_BIT__PB6    PB6
#  define PIN_PIN__PB6    PINB

#  define PIN_PORT__D13   PORTB
#  define PIN_DDR__D13    DDRB
#  define PIN_BIT__D13    PB7
#  define PIN_PIN__D13    PINB
#  define PIN_PORT__PB7   PORTB
#  define PIN_DDR__PB7    DDRB
#  define PIN_BIT__PB7    PB7
#  define PIN_PIN__PB7    PINB

#  define PIN_PORT__D14   PORTJ
#  define PIN_DDR__D14    DDRJ
#  define PIN_BIT__D14    PJ1
#  define PIN_PIN__D14    PINJ
#  define PIN_PORT__PJ1   PORTJ
#  define PIN_DDR__PJ1    DDRJ
#  define PIN_BIT__PJ1    PJ1
#  define PIN_PIN__PJ1    PINJ

#  define PIN_PORT__D15   PORTJ
#  define PIN_DDR__D15    DDRJ
#  define PIN_BIT__D15    PJ0
#  define PIN_PIN__D15    PINJ
#  define PIN_PORT__PJ0   PORTJ
#  define PIN_DDR__PJ0    DDRJ
#  define PIN_BIT__PJ0    PJ0
#  define PIN_PIN__PJ0    PINJ

#  define PIN_PORT__D16   PORTH
#  define PIN_DDR__D16    DDRH
#  define PIN_BIT__D16    PH1
#  define PIN_PIN__D16    PINH
#  define PIN_PORT__PH1   PORTH
#  define PIN_DDR__PH1    DDRH
#  define PIN_BIT__PH1    PH1
#  define PIN_PIN__PH1    PINH

#  define PIN_PORT__D17   PORTH
#  define PIN_DDR__D17    DDRH
#  define PIN_BIT__D17    PH0
#  define PIN_PIN__D17    PINH
#  define PIN_PORT__PH0   PORTH
#  define PIN_DDR__PH0    DDRH
#  define PIN_BIT__PH0    PH0
#  define PIN_PIN__PH0    PINH

#  define PIN_PORT__D18   PORTD
#  define PIN_DDR__D18    DDRD
#  define PIN_BIT__D18    PD3
#  define PIN_PIN__D18    PIND
#  define PIN_PORT__PD3   PORTD
#  define PIN_DDR__PD3    DDRD
#  define PIN_BIT__PD3    PD3
#  define PIN_PIN__PD3    PIND

#  define PIN_PORT__D19   PORTD
#  define PIN_DDR__D19    DDRD
#  define PIN_BIT__D19    PD2
#  define PIN_PIN__D19    PIND
#  define PIN_PORT__PD2   PORTD
#  define PIN_DDR__PD2    DDRD
#  define PIN_BIT__PD2    PD2
#  define PIN_PIN__PD2    PIND

#  define PIN_PORT__D20   PORTD
#  define PIN_DDR__D20    DDRD
#  define PIN_BIT__D20    PD1
#  define PIN_PIN__D20    PIND
#  define PIN_PORT__PD1   PORTD
#  define PIN_DDR__PD1    DDRD
#  define PIN_BIT__PD1    PD1
#  define PIN_PIN__PD1    PIND

#  define PIN_PORT__D21   PORTD
#  define PIN_DDR__D21    DDRD
#  define PIN_BIT__D21    PD0
#  define PIN_PIN__D21    PIND
#  define PIN_PORT__PD0   PORTD
#  define PIN_DDR__PD0    DDRD
#  define PIN_BIT__PD0    PD0
#  define PIN_PIN__PD0    PIND

#  define PIN_PORT__D22   PORTA
#  define PIN_DDR__D22    DDRA
#  define PIN_BIT__D22    PA0
#  define PIN_PIN__D22    PINA
#  define PIN_PORT__PA0   PORTA
#  define PIN_DDR__PA0    DDRA
#  define PIN_BIT__PA0    PA0
#  define PIN_PIN__PA0    PINA

#  define PIN_PORT__D23   PORTA
#  define PIN_DDR__D23    DDRA
#  define PIN_BIT__D23    PA1
#  define PIN_PIN__D23    PINA
#  define PIN_PORT__PA1   PORTA
#  define PIN_DDR__PA1    DDRA
#  define PIN_BIT__PA1    PA1
#  define PIN_PIN__PA1    PINA

#  define PIN_PORT__D24   PORTA
#  define PIN_DDR__D24    DDRA
#  define PIN_BIT__D24    PA2
#  define PIN_PIN__D24    PINA
#  define PIN_PORT__PA2   PORTA
#  define PIN_DDR__PA2    DDRA
#  define PIN_BIT__PA2    PA2
#  define PIN_PIN__PA2    PINA

#  define PIN_PORT__D25   PORTA
#  define PIN_DDR__D25    DDRA
#  define PIN_BIT__D25    PA3
#  define PIN_PIN__D25    PINA
#  define PIN_PORT__PA3   PORTA
#  define PIN_DDR__PA3    DDRA
#  define PIN_BIT__PA3    PA3
#  define PIN_PIN__PA3    PINA

#  define PIN_PORT__D26   PORTA
#  define PIN_DDR__D26    DDRA
#  define PIN_BIT__D26    PA4
#  define PIN_PIN__D26    PINA
#  define PIN_PORT__PA4   PORTA
#  define PIN_DDR__PA4    DDRA
#  define PIN_BIT__PA4    PA4
#  define PIN_PIN__PA4    PINA

#  define PIN_PORT__D27   PORTA
#  define PIN_DDR__D27    DDRA
#  define PIN_BIT__D27    PA5
#  define PIN_PIN__D27    PINA
#  define PIN_PORT__PA5   PORTA
#  define PIN_DDR__PA5    DDRA
#  define PIN_BIT__PA5    PA5
#  define PIN_PIN__PA5    PINA

#  define PIN_PORT__D28   PORTA
#  define PIN_DDR__D28    DDRA
#  define PIN_BIT__D28    PA6
#  define PIN_PIN__D28    PINA
#  define PIN_PORT__PA6   PORTA
#  define PIN_DDR__PA6    DDRA
#  define PIN_BIT__PA6    PA6
#  define PIN_PIN__PA6    PINA

#  define PIN_PORT__D29   PORTA
#  define PIN_DDR__D29    DDRA
#  define PIN_BIT__D29    PA7
#  define PIN_PIN__D29    PINA
#  define PIN_PORT__PA7   PORTA
#  define PIN_DDR__PA7    DDRA
#  define PIN_BIT__PA7    PA7
#  define PIN_PIN__PA7    PINA

#  define PIN_PORT__D30   PORTC
#  define PIN_DDR__D30    DDRC
#  define PIN_BIT__D30    PC7
#  define PIN_PIN__D30    PINC
#  define PIN_PORT__PC7   PORTC
#  define PIN_DDR__PC7    DDRC
#  define PIN_BIT__PC7    PC7
#  define PIN_PIN__PC7    PINC

#  define PIN_PORT__D31   PORTC
#  define PIN_DDR__D31    DDRC
#  define PIN_BIT__D31    PC6
#  define PIN_PIN__D31    PINC
#  define PIN_PORT__PC6   PORTC
#  define PIN_DDR__PC6    DDRC
#  define PIN_BIT__PC6    PC6
#  define PIN_PIN__PC6    PINC

#  define PIN_PORT__D32   PORTC
#  define PIN_DDR__D32    DDRC
#  define PIN_BIT__D32    PC5
#  define PIN_PIN__D32    PINC
#  define PIN_PORT__PC5   PORTC
#  define PIN_DDR__PC5    DDRC
#  define PIN_BIT__PC5    PC5
#  define PIN_PIN__PC5    PINC

#  define PIN_PORT__D33   PORTC
#  define PIN_DDR__D33    DDRC
#  define PIN_BIT__D33    PC4
#  define PIN_PIN__D33    PINC
#  define PIN_PORT__PC4   PORTC
#  define PIN_DDR__PC4    DDRC
#  define PIN_BIT__PC4    PC4
#  define PIN_PIN__PC4    PINC

#  define PIN_PORT__D34   PORTC
#  define PIN_DDR__D34    DDRC
#  define PIN_BIT__D34    PC3
#  define PIN_PIN__D34    PINC
#  define PIN_PORT__PC3   PORTC
#  define PIN_DDR__PC3    DDRC
#  define PIN_BIT__PC3    PC3
#  define PIN_PIN__PC3    PINC

#  define PIN_PORT__D35   PORTC
#  define PIN_DDR__D35    DDRC
#  define PIN_BIT__D35    PC2
#  define PIN_PIN__D35    PINC
#  define PIN_PORT__PC2   PORTC
#  define PIN_DDR__PC2    DDRC
#  define PIN_BIT__PC2    PC2
#  define PIN_PIN__PC2    PINC

#  define PIN_PORT__D36   PORTC
#  define PIN_DDR__D36    DDRC
#  define PIN_BIT__D36    PC1
#  define PIN_PIN__D36    PINC
#  define PIN_PORT__PC1   PORTC
#  define PIN_DDR__PC1    DDRC
#  define PIN_BIT__PC1    PC1
#  define PIN_PIN__PC1    PINC

#  define PIN_PORT__D37   PORTC
#  define PIN_DDR__D37    DDRC
#  define PIN_BIT__D37    PC0
#  define PIN_PIN__D37    PINC
#  define PIN_PORT__PC0   PORTC
#  define PIN_DDR__PC0    DDRC
#  define PIN_BIT__PC0    PC0
#  define PIN_PIN__PC0    PINC

#  define PIN_PORT__D38   PORTD
#  define PIN_DDR__D38    DDRD
#  define PIN_BIT__D38    PD7
#  define PIN_PIN__D38    PIND
#  define PIN_PORT__PD7   PORTD
#  define PIN_DDR__PD7    DDRD
#  define PIN_BIT__PD7    PD7
#  define PIN_PIN__PD7    PIND

#  define PIN_PORT__D39   PORTG
#  define PIN_DDR__D39    DDRG
#  define PIN_BIT__D39    PG2
#  define PIN_PIN__D39    PING
#  define PIN_PORT__PG2   PORTG
#  define PIN_DDR__PG2    DDRG
#  define PIN_BIT__PG2    PG2
#  define PIN_PIN__PG2    PING

#  define PIN_PORT__D40   PORTG
#  define PIN_DDR__D40    DDRG
#  define PIN_BIT__D40    PG1
#  define PIN_PIN__D40    PING
#  define PIN_PORT__PG1   PORTG
#  define PIN_DDR__PG1    DDRG
#  define PIN_BIT__PG1    PG1
#  define PIN_PIN__PG1    PING

#  define PIN_PORT__D41   PORTG
#  define PIN_DDR__D41    DDRG
#  define PIN_BIT__D41    PG0
#  define PIN_PIN__D41    PING
#  define PIN_PORT__PG0   PORTG
#  define PIN_DDR__PG0    DDRG
#  define PIN_BIT__PG0    PG0
#  define PIN_PIN__PG0    PING

#  define PIN_PORT__D42   PORTL
#  define PIN_DDR__D42    DDRL
#  define PIN_BIT__D42    PL7
#  define PIN_PIN__D42    PINL
#  define PIN_PORT__PL7   PORTL
#  define PIN_DDR__PL7    DDRL
#  define PIN_BIT__PL7    PL7
#  define PIN_PIN__PL7    PINL

#  define PIN_PORT__D43   PORTL
#  define PIN_DDR__D43    DDRL
#  define PIN_BIT__D43    PL6
#  define PIN_PIN__D43    PINL
#  define PIN_PORT__PL6   PORTL
#  define PIN_DDR__PL6    DDRL
#  define PIN_BIT__PL6    PL6
#  define PIN_PIN__PL6    PINL

#  define PIN_PORT__D44   PORTL
#  define PIN_DDR__D44    DDRL
#  define PIN_BIT__D44    PL5
#  define PIN_PIN__D44    PINL
#  define PIN_PORT__PL5   PORTL
#  define PIN_DDR__PL5    DDRL
#  define PIN_BIT__PL5    PL5
#  define PIN_PIN__PL5    PINL

#  define PIN_PORT__D45   PORTL
#  define PIN_DDR__D45    DDRL
#  define PIN_BIT__D45    PL4
#  define PIN_PIN__D45    PINL
#  define PIN_PORT__PL4   PORTL
#  define PIN_DDR__PL4    DDRL
#  define PIN_BIT__PL4    PL4
#  define PIN_PIN__PL4    PINL

#  define PIN_PORT__D46   PORTL
#  define PIN_DDR__D46    DDRL
#  define PIN_BIT__D46    PL3
#  define PIN_PIN__D46    PINL
#  define PIN_PORT__PL3   PORTL
#  define PIN_DDR__PL3    DDRL
#  define PIN_BIT__PL3    PL3
#  define PIN_PIN__PL3    PINL

#  define PIN_PORT__D47   PORTL
#  define PIN_DDR__D47    DDRL
#  define PIN_BIT__D47    PL2
#  define PIN_PIN__D47    PINL
#  define PIN_PORT__PL2   PORTL
#  define PIN_DDR__PL2    DDRL
#  define PIN_BIT__PL2    PL2
#  define PIN_PIN__PL2    PINL

#  define PIN_PORT__D48   PORTL
#  define PIN_DDR__D48    DDRL
#  define PIN_BIT__D48    PL1
#  define PIN_PIN__D48    PINL
#  define PIN_PORT__PL1   PORTL
#  define PIN_DDR__PL1    DDRL
#  define PIN_BIT__PL1    PL1
#  define PIN_PIN__PL1    PINL

#  define PIN_PORT__D49   PORTL
#  define PIN_DDR__D49    DDRL
#  define PIN_BIT__D49    PL0
#  define PIN_PIN__D49    PINL
#  define PIN_PORT__PL0   PORTL
#  define PIN_DDR__PL0    DDRL
#  define PIN_BIT__PL0    PL0
#  define PIN_PIN__PL0    PINL

#  define PIN_PORT__D50   PORTB
#  define PIN_DDR__D50    DDRB
#  define PIN_BIT__D50    PB3
#  define PIN_PIN__D50    PINB
#  define PIN_PORT__PB3   PORTB
#  define PIN_DDR__PB3    DDRB
#  define PIN_BIT__PB3    PB3
#  define PIN_PIN__PB3    PINB

#  define PIN_PORT__D51   PORTB
#  define PIN_DDR__D51    DDRB
#  define PIN_BIT__D51    PB2
#  define PIN_PIN__D51    PINB
#  define PIN_PORT__PB2   PORTB
#  define PIN_DDR__PB2    DDRB
#  define PIN_BIT__PB2    PB2
#  define PIN_PIN__PB2    PINB

#  define PIN_PORT__D52   PORTB
#  define PIN_DDR__D52    DDRB
#  define PIN_BIT__D52    PB1
#  define PIN_PIN__D52    PINB
#  define PIN_PORT__PB1   PORTB
#  define PIN_DDR__PB1    DDRB
#  define PIN_BIT__PB1    PB1
#  define PIN_PIN__PB1    PINB

#  define PIN_PORT__D53   PORTB
#  define PIN_DDR__D53    DDRB
#  define PIN_BIT__D53    PB0
#  define PIN_PIN__D53    PINB
#  define PIN_PORT__PB0   PORTB
#  define PIN_DDR__PB0    DDRB
#  define PIN_BIT__PB0    PB0
#  define PIN_PIN__PB0    PINB

#  define PIN_PORT__A0    PORTE
#  define PIN_DDR__A0     DDRE
#  define PIN_BIT__A0     PE0
#  define PIN_PIN__A0     PINE
#  define PIN_PORT__PE0   PORTE
#  define PIN_DDR__PE0    DDRE
#  define PIN_BIT__PE0    PE0
#  define PIN_PIN__PE0    PINE

#  define PIN_PORT__A1    PORTE
#  define PIN_DDR__A1     DDRE
#  define PIN_BIT__A1     PE1
#  define PIN_PIN__A1     PINE
#  define PIN_PORT__PE1   PORTE
#  define PIN_DDR__PE1    DDRE
#  define PIN_BIT__PE1    PE1
#  define PIN_PIN__PE1    PINE

#  define PIN_PORT__A2    PORTE
#  define PIN_DDR__A2     DDRE
#  define PIN_BIT__A2     PE4
#  define PIN_PIN__A2     PINE
#  define PIN_PORT__PE4   PORTE
#  define PIN_DDR__PE4    DDRE
#  define PIN_BIT__PE4    PE4
#  define PIN_PIN__PE4    PINE

#  define PIN_PORT__A3    PORTE
#  define PIN_DDR__A3     DDRE
#  define PIN_BIT__A3     PE5
#  define PIN_PIN__A3     PINE
#  define PIN_PORT__PE5   PORTE
#  define PIN_DDR__PE5    DDRE
#  define PIN_BIT__PE5    PE5
#  define PIN_PIN__PE5    PINE

#  define PIN_PORT__A4    PORTG
#  define PIN_DDR__A4     DDRG
#  define PIN_BIT__A4     PG5
#  define PIN_PIN__A4     PING
#  define PIN_PORT__PG5   PORTG
#  define PIN_DDR__PG5    DDRG
#  define PIN_BIT__PG5    PG5
#  define PIN_PIN__PG5    PING

#  define PIN_PORT__A5    PORTE
#  define PIN_DDR__A5     DDRE
#  define PIN_BIT__A5     PE3
#  define PIN_PIN__A5     PINE
#  define PIN_PORT__PE3   PORTE
#  define PIN_DDR__PE3    DDRE
#  define PIN_BIT__PE3    PE3
#  define PIN_PIN__PE3    PINE

#  define PIN_PORT__A6    PORTH
#  define PIN_DDR__A6     DDRH
#  define PIN_BIT__A6     PH3
#  define PIN_PIN__A6     PINH
#  define PIN_PORT__PH3   PORTH
#  define PIN_DDR__PH3    DDRH
#  define PIN_BIT__PH3    PH3
#  define PIN_PIN__PH3    PINH

#  define PIN_PORT__A7    PORTH
#  define PIN_DDR__A7     DDRH
#  define PIN_BIT__A7     PH4
#  define PIN_PIN__A7     PINH
#  define PIN_PORT__PH4   PORTH
#  define PIN_DDR__PH4    DDRH
#  define PIN_BIT__PH4    PH4
#  define PIN_PIN__PH4    PINH

#  define PIN_PORT__A8    PORTH
#  define PIN_DDR__A8     DDRH
#  define PIN_BIT__A8     PH5
#  define PIN_PIN__A8     PINH
#  define PIN_PORT__PH5   PORTH
#  define PIN_DDR__PH5    DDRH
#  define PIN_BIT__PH5    PH5
#  define PIN_PIN__PH5    PINH

#  define PIN_PORT__A9    PORTH
#  define PIN_DDR__A9     DDRH
#  define PIN_BIT__A9     PH6
#  define PIN_PIN__A9     PINH
#  define PIN_PORT__PH6   PORTH
#  define PIN_DDR__PH6    DDRH
#  define PIN_BIT__PH6    PH6
#  define PIN_PIN__PH6    PINH

#  define PIN_PORT__A10   PORTB
#  define PIN_DDR__A10    DDRB
#  define PIN_BIT__A10    PB4
#  define PIN_PIN__A10    PINB
#  define PIN_PORT__PB4   PORTB
#  define PIN_DDR__PB4    DDRB
#  define PIN_BIT__PB4    PB4
#  define PIN_PIN__PB4    PINB

#  define PIN_PORT__A11   PORTB
#  define PIN_DDR__A11    DDRB
#  define PIN_BIT__A11    PB5
#  define PIN_PIN__A11    PINB
#  define PIN_PORT__PB5   PORTB
#  define PIN_DDR__PB5    DDRB
#  define PIN_BIT__PB5    PB5
#  define PIN_PIN__PB5    PINB

#  define PIN_PORT__A12   PORTB
#  define PIN_DDR__A12    DDRB
#  define PIN_BIT__A12    PB6
#  define PIN_PIN__A12    PINB
#  define PIN_PORT__PB6   PORTB
#  define PIN_DDR__PB6    DDRB
#  define PIN_BIT__PB6    PB6
#  define PIN_PIN__PB6    PINB

#  define PIN_PORT__A13   PORTB
#  define PIN_DDR__A13    DDRB
#  define PIN_BIT__A13    PB7
#  define PIN_PIN__A13    PINB
#  define PIN_PORT__PB7   PORTB
#  define PIN_DDR__PB7    DDRB
#  define PIN_BIT__PB7    PB7
#  define PIN_PIN__PB7    PINB

#  define PIN_PORT__A14   PORTJ
#  define PIN_DDR__A14    DDRJ
#  define PIN_BIT__A14    PJ1
#  define PIN_PIN__A14    PINJ
#  define PIN_PORT__PJ1   PORTJ
#  define PIN_DDR__PJ1    DDRJ
#  define PIN_BIT__PJ1    PJ1
#  define PIN_PIN__PJ1    PINJ

#  define PIN_PORT__A15   PORTJ
#  define PIN_DDR__A15    DDRJ
#  define PIN_BIT__A15    PJ0
#  define PIN_PIN__A15    PINJ
#  define PIN_PORT__PJ0   PORTJ
#  define PIN_DDR__PJ0    DDRJ
#  define PIN_BIT__PJ0    PJ0
#  define PIN_PIN__PJ0    PINJ



#endif // __AVR_ATmega2560__

#endif // __ARDUINOPINS_H__

//@}
// eof
