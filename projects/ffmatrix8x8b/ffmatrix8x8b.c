/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 8x8 (version b) (see \ref PROJECTS_FFMATRIX8X8B)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX8X8B
    @{
*/

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
