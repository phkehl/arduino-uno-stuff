/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 7x7 (see \ref PROJECTS_FFMATRIX7X7)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX7X7
    @{
*/

#include "ffmatrix.h"
#include "ffmatrix7x7.h"

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
