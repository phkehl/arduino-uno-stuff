/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFCLOCK)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFCLOCK flifplip clock
    \ingroup PROJECTS

    <img src="../../projects/ffclock/ffclock.jpg"/>
    <img src="../../projects/ffclock/ffclock-annot.jpg" width="50%"/>

    - [28BYJ-48.pdf](../../projects/ffclock/docs/28BYJ-48.pdf) (stepper motor)
    - [dialight_547-2xxx.pdf](../../projects/ffclock/docs/dialight_547-2xxx.pdf) (LED modules)
    - [helical_antenna.pdf](../../projects/ffclock/docs/helical_antenna.pdf) (from http://home.iae.nl/users/plundahl/antenne/helical.htm, https://web.archive.org/web/20091210100330/http://home.iae.nl/users/plundahl/antenne/helical.htm)
    - [LN2003A.pdf](../../projects/ffclock/docs/LN2003A.pdf) (darlington transistor array)
    - [L293.pdf](../../projects/ffclock/docs/L293.pdf) (driver)
    - [panasonic_av4444.pdf](../../projects/ffclock/docs/panasonic_av4444.pdf) (mini switches)
    - [NEO-M8_DataSheet_(UBX-13003366).pdf](../../projects/ffclock/docs/NEO-M8_DataSheet_(UBX-13003366).pdf) (GNSS receiver)
    - [NEO-M8_HardwareIntegrationManual_(UBX-13003557).pdf](../../projects/ffclock/docs/NEO-M8_HardwareIntegrationManual_(UBX-13003557).pdf) (GNSS receiver)
    - [NEO-M8_ProductSummary_(UBX-13003449).pdf](../../projects/ffclock/docs/NEO-M8_ProductSummary_(UBX-13003449).pdf) (GNSS receiver)

    See ffclock.c source code and https://oinkzwurgl.org/projaeggd/flipflipclock for details.

    Wiring (\ref P_PINOUTS):
\verbatim
-    1  PC6  ~RESET  PCINT14           <- reset button
D0   2  PD0  RXD     PCINT16           <- GNSS receiver input <- u-blox NEO-M8T TXD pin
D1   3  PD1  TXD     PCINT17           -> debug output -> FTDI cable RXD
D2   4  PD2  INT0    PCINT18           <- button A (left)
D3   5  PD3  INT1    PCINT19  OC2B     <- button B (right)
D4   6  PD4  T0      PCINT20  XCK      -> system load LED (built into reset button)
-    7  -            -        VCC      <- 5V0 power
-    8  -            -        GND      <- ground
-    9  PB6  OSC1    PCINT16  XTAL1    <- 16 MHz crystal oscillator
-   10  PB7  OSC2    PCINT17  XTAL2    <- 16 MHz crystal oscillator
D5  11  PD5  T1      PCINT21  OC0B     -> system tick signal (unused)
D6  12  PD6  AIN0    PCINT22  OC0A     <- minutes wheel "zero" position sensor (switch)
D7  13  PD7  AIN1    PCINT23           <- hours wheel "zero" position sensor (switch)
D8  14  PB0  CLK0    PCINT0   ICP1     -> stepper motor I4
D9  15  PB1  OC1A    PCINT1            -> stepper motor I1
D10 16  PB2  OC1B    PCINT2   ~SS      -> stepper motor I3
D11 17  PB3  OC2A    PCINT3   MOSI     -> stepper motor I2
D12 18  PB4          PCINT4   MISO     .
D13 19  PB5          PCINT5   SCK      .
-   20  -            -        VCC      <- 5V0 power
-   21  -            -        AREF     .
-   22  -            -        GND      <- ground
A0  23  PC0  ADC0    PCINT8            -> seconds display LED 0
A1  24  PC1  ADC1    PCINT9            -> seconds display LED 1
A2  25  PC2  ADC2    PCINT10           -> seconds display LED 2
A3  26  PC3  ADC3    PCINT11           -> seconds display LED 3
A4  27  PC4  ADC4    PCINT12  SDA      -> seconds display LED 4
A5  28  PC5  ADC5    PCINT13  SCL      -> seconds display LED 5
\endverbatim

    @{
*/

#ifndef __FFCLOCK_H__
#define __FFCLOCK_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __FFCLOCK_H__
// @}
// eof
