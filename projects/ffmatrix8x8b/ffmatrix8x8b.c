/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 8x8 (version b) (see \ref PROJECTS_FFMATRIX8X8B)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX8X8B
    @{
*/

//const char skRtttlMelody24[] PROGMEM = "HappyBday:d=4,o=5,b=125:8d.,16d,e,d,g,2f#,8d.,16d,e,d,a,2g,8d.,16d,d6,b,g,f#,2e,8c.6,16c6,b,g,a,2g";

#include "ffmatrix.h"
#include "ffmatrix8x8b.h"

void appInit(void)
{
    matrixInit();
}

void appCreateTask(void)
{
    matrixCreateTask();
}


//@}
// eof
