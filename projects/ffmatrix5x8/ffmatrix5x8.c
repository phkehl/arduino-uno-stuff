/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 5x8 (see \ref PROJECTS_FFMATRIX5X8)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX5X8
    @{
*/

#include "ffmatrix.h"
#include "ffmatrix5x8.h"

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
