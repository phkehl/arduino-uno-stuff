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
typedef void (*SDL_EVENT_CB_t)(const SDL_Event *);

bool sdlInit(const char *title, SDL_CANVAS_CB_t canvasCb, SDL_EVENT_CB_t eventCb);

bool sdlHandle(void);

bool sdlUpdate(void);

bool sdlShutdown(void);

void sdlSetFramerate(const int fps);
int sdlGetFramerate(void);

void sdlSetInfoNw(const char *fmt, ...) __attribute__((format(printf,1,2)));
void sdlSetInfoNe(const char *fmt, ...) __attribute__((format(printf,1,2)));


/* *************************************************************************** */

#endif // __SLD_H__

//@}
// eof
