/*!
    \file
    \brief flipflip's Arduino Uno stuff: Maschki application (see \ref PROJECTS_MASCHKI)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_MASCHKI Maschki
    \ingroup PROJECTS

    - [PIR Sensor #555-28027](../../projects/maschki/555-28027_PIR_Sensor.pdf)
    - [PIR Sensor RE 200B](../../projects/maschki/RE200B.pdf)
    - [Ultrasonic Ranging Module HC-SR04](../../projects/maschki/HCSR04.pdf)

    <img src="../../projects/maschki/maschki.jpg"/>

    See maschki.c source code for details.

    @{
*/

#ifndef __MASCHKI_H__
#define __MASCHKI_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __MASCHKI_H__
// @}
// eof
