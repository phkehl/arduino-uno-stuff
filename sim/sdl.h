/*!
    \file
    \brief flipflip's Arduino Uno stuff: SDL helpers

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

*/

#ifndef __SLD_H__
#define __SLD_H__

#include "stdstuff.h"      // ff: useful macros and types

/* *************************************************************************** */

#ifndef SDL_FONT
#  define SDL_FONT "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
#endif

typedef void (*SDL_CANVAS_CB_t)(SDL_Renderer *);

bool sdlInit(const char *title, SDL_CANVAS_CB_t canvasCb);

bool sdlHandle(void);

bool sdlUpdate(void);

bool sdlShutdown(void);

void sdlSetFramerate(const int fps);


/* *************************************************************************** */

#endif // __SLD_H__

//@}
// eof
