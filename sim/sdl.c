/*!
    \file
    \brief flipflip's Arduino Uno stuff: SDL helpers

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

*/

#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions
#include <stdlib.h>        // libc: general utilities
#include <stdarg.h>        // libc: variable argument lists
#include <stdio.h>         // libc: standard input/output library functions

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "stdstuff.h"      // ff: useful macros and types
#include "sim.h"           // ff: simulator mocks
#include "sdl.h"           // ff: sdl stuff

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
#  define SDL_RMASK 0xff000000
#  define SDL_GMASK 0x00ff0000
#  define SDL_BMASK 0x0000ff00
#  define SDL_AMASK 0x000000ff
#else
#  define SDL_RMASK 0x000000ff
#  define SDL_GMASK 0x0000ff00
#  define SDL_BMASK 0x00ff0000
#  define SDL_AMASK 0xff000000
#endif

typedef struct SDL_STATE_s
{
    char            title[100];
    char            infoNw[100];
    char            infoNe[100];
    SDL_Window     *pWindow;
    SDL_Renderer   *pRenderer;
    TTF_Font       *pFont;
    int             fontSize;
    int             fontHeight;
    int             padding;
    int             rWidth;
    int             rHeight;
    SDL_Rect        cRect;
    SDL_CANVAS_CB_t canvasCb;
    SDL_EVENT_CB_t  eventCb;
    uint32_t        frameCount;
} SDL_STATE_t;

static SDL_STATE_t sSdlState;

static const char *sSdlGetVersionStr(void)
{
    static char str[250];
    SDL_version sdlCompiled;
    SDL_VERSION(&sdlCompiled);
    SDL_version sdlLinked;
    SDL_GetVersion(&sdlLinked);

    SDL_version ttfCompiled;
    SDL_TTF_VERSION(&ttfCompiled);
    const SDL_version *ttfLinked = TTF_Linked_Version();

    snprintf(str, sizeof(str), "SDL platform=%s version=%d.%d.%d(%d.%d.%d) GFX=%d.%d.%d TTF=%d.%d.%d(%d.%d.%d)",
        SDL_GetPlatform(),
        sdlCompiled.major, sdlCompiled.minor, sdlCompiled.patch,
        sdlLinked.major, sdlLinked.minor, sdlLinked.patch,
        SDL2_GFXPRIMITIVES_MAJOR, SDL2_GFXPRIMITIVES_MINOR, SDL2_GFXPRIMITIVES_MICRO,
        ttfCompiled.major, ttfCompiled.minor, ttfCompiled.patch,
        ttfLinked->major, ttfLinked->minor, ttfLinked->patch);
    return str;
}

static void sSdlUpdateWindowSize(void);

bool sdlInit(const char *title, SDL_CANVAS_CB_t canvasCb, SDL_EVENT_CB_t  eventCb)
{
    DEBUG("SDL init: %s", sSdlGetVersionStr());

    // initialise state
    memset(&sSdlState, 0, sizeof(sSdlState));

    snprintf(sSdlState.title, sizeof(sSdlState.title), "%s", title);
    sSdlState.canvasCb = canvasCb;
    sSdlState.eventCb = eventCb;

    // font
    if (TTF_Init() == -1)
    {
        WARNING("SDL TTF init: %s", TTF_GetError());
        sdlShutdown();
        return false;
    }
    DEBUG("SDL TTF font: "SDL_FONT);
    sSdlState.fontSize = 20;
    sSdlState.pFont = TTF_OpenFont(SDL_FONT, sSdlState.fontSize);
    if (sSdlState.pFont == NULL)
    {
        WARNING("SDL TTF font: %s", TTF_GetError());
        sdlShutdown();
        return false;
    }

    // SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
		WARNING("SDL init: %s", SDL_GetError());
        sdlShutdown();
		return false;
	}

    // window
    sSdlState.pWindow = SDL_CreateWindow(sSdlState.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (sSdlState.pWindow == NULL)
    {
        ERROR("SDL window: %s", SDL_GetError());
        sdlShutdown();
        return false;
    }

    // renderer
    sSdlState.pRenderer = SDL_CreateRenderer(sSdlState.pWindow, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sSdlState.pRenderer == NULL)
    {
        ERROR("SDL renderer: %s", SDL_GetError());
        sdlShutdown();
        return false;
    }

    // re-calculate padding and such based on window size
    sSdlUpdateWindowSize();

    return true;
}

void sdlSetInfoNw(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsnprintf(sSdlState.infoNw, sizeof(sSdlState.infoNw), fmt, args);
    va_end(args);
}

void sdlSetInfoNe(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsnprintf(sSdlState.infoNe, sizeof(sSdlState.infoNe), fmt, args);
    va_end(args);
}

bool sdlShutdown(void)
{
    DEBUG("SDL: shutdown");
    if (sSdlState.pFont != NULL)
    {
        TTF_CloseFont(sSdlState.pFont);
        sSdlState.pFont = NULL;
    }
    if (sSdlState.pWindow != NULL)
    {
        SDL_DestroyWindow(sSdlState.pWindow);
        sSdlState.pWindow = NULL;
    }
    if (sSdlState.pRenderer != NULL)
    {
        SDL_DestroyRenderer(sSdlState.pRenderer);
        sSdlState.pRenderer = NULL;
    }
    SDL_Quit();
    return true;
}

bool sdlHandle(void)
{
    bool abort = false;
    SDL_Event event;

    //DEBUG("%08.3f poll event", (double)SDL_GetTicks() * 1e-3);
    while (SDL_PollEvent(&event) != 0)
    {
        //DEBUG("%08.3f event %d", (double)event.common.timestamp * 1e-3, event.type);
        if (sSdlState.eventCb != NULL)
        {
            sSdlState.eventCb(&event);
        }
        switch (event.type)
        {
            // window close
            case SDL_QUIT:
                abort = true;
                break;
            // window event
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        sSdlUpdateWindowSize();
                        break;
                }
                break;
            // key press
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_q:      // q key
                    case SDLK_ESCAPE: // esc key
                        abort = true;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    return abort ? false : true;
}

typedef enum SDL_INFOSTR_e
{
    SDL_INFOSTR_NW, SDL_INFOSTR_NE, SDL_INFOSTR_SW, SDL_INFOSTR_SE,
    SDL_INFOSTR_N, SDL_INFOSTR_S,
} SDL_INFOSTR_t;

static void sSdlRenderInfoString(SDL_INFOSTR_t where, const char *fmt, ...) __attribute__((format(printf,2,3)));

bool sdlUpdate(void)
{
    SDL_Renderer *pRenderer = sSdlState.pRenderer;
    SDL_RenderClear(pRenderer);

    // fill background
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pRenderer, 80, 60, 60, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(pRenderer, NULL);

    // fill canvas
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(pRenderer, &sSdlState.cRect);

    // call user canvas function
    if (sSdlState.canvasCb)
    {
        SDL_Surface *pSurface = SDL_CreateRGBSurface(0,
            sSdlState.cRect.w, sSdlState.cRect.h, 32, 0, 0, 0, 0 /*SDL_RMASK, SDL_GMASK, SDL_BMASK, SDL_AMASK*/);
        if (pSurface != NULL)
        {
            SDL_Renderer *pCanvasRenderer = SDL_CreateSoftwareRenderer(pSurface);
            if (pCanvasRenderer != NULL)
            {
                SDL_SetRenderDrawBlendMode(pCanvasRenderer, SDL_BLENDMODE_BLEND);
                sSdlState.canvasCb(pCanvasRenderer);
                SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
                if (pTexture != NULL)
                {
                    SDL_RenderCopy(pRenderer, pTexture, NULL, &sSdlState.cRect);
                    SDL_DestroyTexture(pTexture);
                }
                SDL_DestroyRenderer(pCanvasRenderer);
            }
            SDL_FreeSurface(pSurface);
        }
    }

    // render info strings
    if (sSdlState.infoNw[0] != '\0')
    {
        sSdlRenderInfoString(SDL_INFOSTR_NW, sSdlState.infoNw);
    }
    sSdlRenderInfoString(SDL_INFOSTR_N, sSdlState.title);
    if (sSdlState.infoNe[0] != '\0')
    {
        sSdlRenderInfoString(SDL_INFOSTR_NE, sSdlState.infoNe);
    }

    sSdlRenderInfoString(SDL_INFOSTR_SW, "frame %i", sSdlState.frameCount);
    sSdlRenderInfoString(SDL_INFOSTR_S, "??? fps");
    //sSdlRenderInfoString(SDL_INFOSTR_SE, "time %.3f", (double)SDL_GetTicks() * 1e-3);
    sSdlRenderInfoString(SDL_INFOSTR_SE, "time %.3f", (double)osTaskGetMsss() * 1e-3);

    sSdlState.frameCount++;
    SDL_RenderPresent(pRenderer);

    return true;
}

static void sSdlUpdateWindowSize(void)
{
    SDL_Renderer *pRenderer = sSdlState.pRenderer;

    int rWidth, rHeight;
    if (SDL_GetRendererOutputSize(pRenderer, &rWidth, &rHeight) != 0)
    {
        WARNING("SDL renderer: %s", SDL_GetError());
        return;
    }

    sSdlState.padding = MAX(5, rHeight /100);

    // calculate new font size
    const int fontSize = MAX(10, rHeight / 25);
    TTF_Font *pNewFont = TTF_OpenFont(SDL_FONT, fontSize);
    if (pNewFont != NULL)
    {
        TTF_CloseFont(sSdlState.pFont);
        sSdlState.fontSize = fontSize;
        sSdlState.pFont = pNewFont;
    }
    sSdlState.fontHeight = TTF_FontHeight(sSdlState.pFont);

    sSdlState.rWidth  = rWidth;
    sSdlState.rHeight = rHeight;

    // new canvas size
    sSdlState.cRect.x = sSdlState.padding;
    sSdlState.cRect.y = sSdlState.fontHeight + (2 * sSdlState.padding);
    sSdlState.cRect.w = sSdlState.rWidth - (2 * sSdlState.padding);
    sSdlState.cRect.h = sSdlState.rHeight - (2 * sSdlState.cRect.y);

    DEBUG("SDL renderer: %dx%d %d/%d %dx%d@%d/%d",
        sSdlState.rWidth, sSdlState.rHeight, sSdlState.fontSize, sSdlState.fontHeight,
        sSdlState.cRect.w, sSdlState.cRect.h, sSdlState.cRect.x, sSdlState.cRect.y);
}

static void sSdlRenderInfoString(SDL_INFOSTR_t where, const char *fmt, ...)
{
    SDL_Renderer *pRenderer = sSdlState.pRenderer;

    SDL_Texture *pTexture = NULL;
    char str[1000];
    str[0] = '\0';
    {
        va_list args;
        va_start(args, fmt);
        vsnprintf(str, sizeof(str), fmt, args);
        va_end(args);
        SDL_Color color = { 255, 255, 255, 255 };
        SDL_Surface *pSurface = TTF_RenderText_Blended(sSdlState.pFont, str, color);
        if (pSurface == NULL)
        {
            WARNING("SDL TTF surface: %s", TTF_GetError());
            return;
        }
        else
        {
            pTexture = SDL_CreateTextureFromSurface(sSdlState.pRenderer, pSurface);
            SDL_FreeSurface(pSurface);
            if (pTexture == NULL)
            {
                WARNING("SDL texture: %s", SDL_GetError());
                return;
            }
        }
    }

    int rWidth  = sSdlState.rWidth;
    int rHeight = sSdlState.rHeight;
    int padding = sSdlState.padding;
    int tWidth, tHeight;
    if (SDL_QueryTexture(pTexture, NULL, NULL, &tWidth, &tHeight) != 0)
    {
        WARNING("SDL texture: %s", SDL_GetError());
    }
    else
    {
        SDL_Rect dst = { .x = 0, .y = 0, .w = tWidth, .h = tHeight };
        switch (where)
        {
            case SDL_INFOSTR_NW:  dst.x = padding;                      dst.y = padding;                      break;
            case SDL_INFOSTR_NE:  dst.x = rWidth - padding - tWidth;    dst.y = padding;                      break;
            case SDL_INFOSTR_SW:  dst.x = padding;                      dst.y = rHeight - padding - tHeight;  break;
            case SDL_INFOSTR_SE:  dst.x = rWidth - padding - tWidth;    dst.y = rHeight - padding - tHeight;  break;
            case SDL_INFOSTR_N:   dst.x = (rWidth - tWidth) / 2;        dst.y = padding;                      break;
            case SDL_INFOSTR_S:   dst.x = (rWidth - tWidth) / 2;        dst.y = rHeight - padding - tHeight;  break;
        }
        //DEBUG("%dx%d %dx%d @ %d.%d %s", rWidth, rHeight, tWidth, tHeight, dst.x, dst.y, str);
        SDL_RenderCopy(pRenderer, pTexture, NULL, &dst);
    }

    SDL_DestroyTexture(pTexture);
}


// eof
