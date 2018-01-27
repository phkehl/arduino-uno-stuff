/*!
    \file
    \brief flipflip's Arduino Uno stuff: twelfth example application (see \ref EXAMPLES_EX12)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup EXAMPLES_E211 12th Example Application
    \ingroup EXAMPLES

    This is the twelfth example application. It demonstrates rotary encoder reading. It assumes an
    encoder with detents and hence there is no resolution to loos when it only uses the falling edge
    of one pin and sampling the other pin. The interrupt pin has to be debounced (100nF or so
    between pin and ground).

    See ex12.c source code for details.

    @{
*/

#ifndef __EX12_H__
#define __EX12_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __EX12_H__
// @}
// eof
