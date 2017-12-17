/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 8x8 (see \ref PROJECTS_FFMATRIX8X8)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX8X8
    @{
*/


#include "ffmatrix.h"
#include "ffmatrix8x8.h"

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
