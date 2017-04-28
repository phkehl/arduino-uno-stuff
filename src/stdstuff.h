/*!
    \file
    \brief flipflip's Arduino Uno stuff: types and stuff (see \ref STDSTUFF)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup STDSTUFF Standard Types and Handy Macros
    \ingroup FF

    @{
*/

#ifndef __STDSTUFF_H__
#define __STDSTUFF_H__

#include <stdint.h>        // libc: integer types
#include <inttypes.h>      // libc: fixed size integer types
#include <stddef.h>        // libc: standard type definitions
#include <stdbool.h>       // libc: boolean types and values

#ifdef __DOXYGEN__
#define NULL 0 //!< the null pointer (from stddef.h) \hideinitializer
#endif

//! \name Handy Macros
//@{
#define BIT(bit) (1<<(bit))   //!< bit \hideinitializer
#ifndef UNUSED
#  define UNUSED(foo) (void)foo //!< mark variable as unused to silence compiler warnings\hideinitializer
#endif
#define SETBITS(port, bits)    port |= (bits)   //!< sets the bits \hideinitializer
#define CLRBITS(port, bits)    port &= ~(bits)  //!< clears the bits \hideinitializer
#define TOGBITS(port, bits)    port ^= (bits)   //!< toggles the bits \hideinitializer
#define NUMOF(x) (sizeof(x)/sizeof(*(x)))       //!< number of elements in vector \hideinitializer
#define ENDLESS true          //!< for endless while loops \hideinitializer
#define FALLTHROUGH           //!< switch fall-through marker \hideinitializer
#define __PAD(n) uint8_t __PADNAME(__LINE__)[n]  //!< struct padding macro \hideinitializer
#define __PADFILL { 0 }           //!< to fill structure padding in initialisers \hideinitializer
#define MIN(a, b)  ((b) < (a) ? (b) : (a)) //!< smaller value of a and b \hideinitializer
#define MAX(a, b)  ((b) > (a) ? (b) : (a)) //!< bigger value of a and b \hideinitializer
#define ABS(a) ((a) > 0 ? (a) : -(a)) //!< absolute value \hideinitializer
#define CLIP(x, a, b) ((x) <= (a) ? (a) : ((x) >= (b) ? (b) : (x))) //!< clip value in range [a:b] \hideinitializer
#define STRINGIFY(x) _STRINGIFY(x) //!< preprocessor stringification  \hideinitializer
#define CONCAT(a, b)   _CONCAT(a, b) //!< preprocessor concatenation  \hideinitializer

#define SWAP2(x) ( (( (x) >>  8)                                                             | ( (x) <<  8)) )
#define SWAP4(x) ( (( (x) >> 24) | (( (x) & 0x00FF0000) >>  8) | (( (x) & 0x0000FF00) <<  8) | ( (x) << 24)) )
//@}

#ifndef __DOXYGEN__
#define _STRINGIFY(x) #x
#define _CONCAT(a, b)  a ## b
#define ___PADNAME(x) __pad##x
#define __PADNAME(x) ___PADNAME(x)
#endif

//! \name Compiler Hints etc.
//@{
#define __PURE(func)          func __attribute__ ((pure))          //!< pure \hideinitializer
#define __IRQ(func)           func __attribute__ ((interrupt))     //!< irq \hideinitializer
#define __WEAK(arg)           arg  __attribute__ ((weak))          //!< weak \hideinitializer
#define __PACKED(arg)         arg  __attribute__ ((packed))        //!< packed \hideinitializer
#define __ALIGN(n, arg)       arg  __attribute__ ((aligned (n)))   //!< align \hideinitializer
#ifdef __INLINE
#  undef __INLINE
#endif
#define __INLINE              inline                                 //!< inline \hideinitializer
#define __NOINLINE            __attribute__((noinline))              //!< no inline \hideinitializer
#define __FORCEINLINE         __attribute__((always_inline)) inline  //!< force inline (also with -Os) \hideinitializer
#define __USED                __attribute__((used))                  //!< used \hideinitializer
#define __NORETURN            __attribute__((noreturn))              //!< no return \hideinitializer
#define __PRINTF(six, aix)    __attribute__((format(printf, six, aix))) //!< printf() style func \hideinitializer
#define __SECTION(sec)        __attribute__((section (STRINGIFY(sec)))); //!< place symbol in section \hideinitializer
#define __NAKED               __attribute__((naked))                 //!< naked function \hideinitializer
//@}

//! \name Flags
//@{
#define INI_FLAGS(name, num) uint8_t name[((num) + 7) / 8]
#define SET_FLAG(name, ix) (name[(ix) / 8] |= BIT((ix) % 8))
#define CLR_FLAG(name, ix) (name[(ix) / 8] &= ~BIT((ix) % 8))
#define GET_FLAG(name, ix) (name[(ix) / 8] & BIT((ix) % 8))
//@}

#endif // __STDSTUFF_H__

//@}
// eof
