/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's LED tester, menu functions (see \ref PROJECTS_FFLEDTESTER_MENU)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFLEDTESTER_MENU flipflip's LED tester, menu functions
    \ingroup PROJECTS

    @{
*/

#ifndef __FFLEDTESTER_MENU_H__
#define __FFLEDTESTER_MENU_H__


#include "stdstuff.h"      // ff: useful macros and types
#include "os.h"            // ff: operating system


/* *************************************************************************** */

// type of menu entry
typedef enum MENU_TYPE_e
{
    MENU_TYPE_STR,  // entry is list of strings to choose from
    MENU_TYPE_VAL,  // entry is value
    MENU_TYPE_HEX,  // entry is value, display hex
    MENU_TYPE_MENU, // entry is a sub-menu
    MENU_TYPE_JUMP, // entry is jump (link) elsewhere
    MENU_TYPE_FUNC, // entry calls function

} MENU_TYPE_t;

// menu entries
typedef struct MENU_s
{
    uint8_t  mid;  // unique menu ID
    uint8_t  pid;  // parent menu ID (or 0 for main menu entries)

    MENU_TYPE_t type;         // the entry type
    const char  name[32];     // the name
    bool        wrap;         // wrap at beginning/end of list/value (only for MENU_TYPE_STR/MENU_TYPE_NUM)
    union
    {
        struct
        {                         // type = MENU_TYPE_STR:
            const __flash char * const __flash *strs;    // - strings to choose from
            uint8_t nStrs;        // - the number of strings
        };
        struct
        {                         // type = MENU_TYPE_NUM
            char    ind;          // - indicator
            int16_t min;          // - minimum value
            int16_t max;          // - maximum value
            int16_t def;          // - default value
        };
        struct
        {                         // type = MENU_TYPE_JUMP
            uint8_t jump;         // - mid to jump to
        };
        struct                    // type = MENU_TYPE_FUNC
        {
            void (*func)(int16_t *);  // function to call

        };
    };
} MENU_t;

// pointer to MENU_t in flash
typedef const __flash MENU_t MENU_P_t;

#define MENU_MID(pkMenu)         ((uint8_t)pgm_read_byte(&((pkMenu)->mid)))
#define MENU_PID(pkMenu)         ((uint8_t)pgm_read_byte(&((pkMenu)->pid)))
#define MENU_TYPE(pkMenu)        ((MENU_TYPE_t)pgm_read_byte(&((pkMenu)->type)))
#define MENU_NAME(pkMenu)        ((pkMenu)->name)
#define MENU_IX(pkMenu, pkCurr)  ((uint8_t)((pkCurr) - (pkMenu)))
#define MENU_WRAP(pkMenu)        ((bool)pgm_read_byte(&((pkMenu)->wrap)))
#define MENU_NSTRS(pkMenu)       ((uint8_t)pgm_read_byte(&((pkMenu)->nStrs)))
#define MENU_STRS(pkMenu)        ((pkMenu)->strs))
#define MENU_STR_IX(pkMenu, ix)  ((pkMenu)->strs[ix])
#define MENU_MIN(pkMenu)         ((int16_t)pgm_read_word(&((pkMenu)->min)))
#define MENU_MAX(pkMenu)         ((int16_t)pgm_read_word(&((pkMenu)->max)))
#define MENU_IND(pkMenu)         ((char)pgm_read_byte(&((pkMenu)->ind)))
#define MENU_DEF(pkMenu)         ((int16_t)pgm_read_byte(&((pkMenu)->def)))
#define MENU_JUMP(pkMenu)        ((uint8_t)pgm_read_byte(&((pkMenu)->jump)))
#define MENU_FUNC(pkMenu)        ((void (*)(int16_t *))pgm_read_word(&((pkMenu)->func)))

// helper macros to generate consistent MENU_t array and value struct
#define M_STR(_mid, _pid, _name, _var, _wrap, _def, _strs) \
    { .type = MENU_TYPE_STR, .mid = (_mid), .pid = (_pid), .name = (_name), .wrap = (_wrap), .def = (_def), .strs = (_strs), .nStrs = NUMOF(_strs) },
#define M_VAL(_mid, _pid, _name, _var, _wrap, _def, _ind, _min, _max) \
    { .type = MENU_TYPE_VAL, .mid = (_mid), .pid = (_pid), .name = (_name), .wrap = (_wrap), .def = (_def), .ind = (_ind), .min = (_min), .max = (_max) },
#define M_HEX(_mid, _pid, _name, _var, _wrap, _def, _ind, _min, _max) \
    { .type = MENU_TYPE_HEX, .mid = (_mid), .pid = (_pid), .name = (_name), .wrap = (_wrap), .def = (_def), .ind = (_ind), .min = (_min), .max = (_max) },
#define M_MEN(_mid, _pid, _name, _var) \
    { .type = MENU_TYPE_MENU, .mid = (_mid), .pid = (_pid), .name = (_name) },
#define M_JMP(_mid, _pid, _name, _var, _jump) \
    { .type = MENU_TYPE_JUMP, .mid = (_mid), .pid = (_pid), .name = (_name), .jump = (_jump) },
#define M_FUN(_mid, _pid, _name, _var, _func) \
    { .type = MENU_TYPE_FUNC, .mid = (_mid), .pid = (_pid), .name = (_name), .func = (_func) },
#define M_VAR(_mod, _pid, _name, _var, ...) int16_t _var;

typedef struct MENU_STATE_s
{
    MENU_P_t *pkMenu; // list of menu entries
    MENU_P_t *pkCurr; // current menu
    uint8_t   nMenu;  // number of menu entries
    bool      active; // true if entry is active
    int16_t  *vals;   // storage for the values associated with each entry (must be big enough for nMenu values)
    OS_TIMER_t scrollTimer; // menu entry scrolling timer
    uint8_t    scrollPhase;
} MENU_STATE_t;


void menuInit(MENU_STATE_t *pState, const MENU_t *pkMenu, const uint8_t nMenu, int16_t *pVals);

void menuDump(const MENU_t *pkMenu, const uint8_t nMenu);

void menuHandle(MENU_STATE_t *pState, const ROTENC_EVENT_t event);

/* *************************************************************************** */

#endif // __FFLEDTESTER_MENU_H__
// @}
// eof
