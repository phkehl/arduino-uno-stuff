/*!
    \file
    \brief flipflip's Arduino Uno stuff: simulator mocks

    - Copyright (c) 2017-2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

*/

#include <stdio.h>         // libc: standard buffered input/output
#include <stdarg.h>        // libc: variable argument lists
#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions
#include <stdlib.h>        // libc: general utilities
#include <time.h>          // libc: time types
#include <sys/time.h>      // libc: time types
#include <signal.h>        // libc: signals
#include <unistd.h>        // libc: standard symbolic constants and types
#include <fcntl.h>         // libc: file control options

#include "stdstuff.h"      // ff: useful macros and types
#include "sim.h"           // ff: simulator mocks

uint32_t hwMathGetRandom(void)
{
    return random();
}

float hwMathFastCosf(const float phi)
{
    return cosf(phi);
}

float hwMathFastSinf(const float phi)
{
    return sinf(phi);
}

float hwMathFastSqrtf(const float x)
{
    return sqrtf(x);
}

void hwMathSeedRandom(const uint32_t seed)
{
    srandom(seed);
}

void hwTic(const uint8_t reg)
{
    UNUSED(reg);
}

uint16_t hwToc(const uint8_t reg)
{
    UNUSED(reg);
    return 0;
}

static uint32_t sSimGetRandomSeed(void)
{
    int f = open("/dev/random", O_RDONLY);
    size_t readLen = 0;
    uint32_t seed;
    while (readLen < sizeof(seed))
    {
        ssize_t res = read(f, ((char *)&seed) + readLen, sizeof(seed) - readLen);
        if (res < 0)
        {
            WARNING("sim: failed reading /dev/random!");
            break;
        }
        readLen += res;
    }
    close(f);
    return seed;
}

static uint32_t sSimTicks;

// system ticks "interrupt" handler
static void sSimTickHandler(int sig)
{
    UNUSED(sig);
    sSimTicks++;
}

// system "idle" task
static void sSimIdle(void)
{
    struct timespec sleep;
    sleep.tv_sec  = 0;
    sleep.tv_nsec = 100;
    nanosleep(&sleep, NULL);
}

void simInit(void)
{
    const uint32_t seed = sSimGetRandomSeed();
    DEBUG("sim: init (0x%08x)", seed);
    hwMathSeedRandom(seed);

    // setup system ticks "interrupt" using interval timer and a signal handler
    // (not sure how optimal this actually is.. there's probably better ways of doing this)
    struct itimerval itimer;
    itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = 0;
    itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 1000;
    struct sigaction action;
    action.sa_flags   = 0;
    action.sa_handler = sSimTickHandler;
    sigfillset(&action.sa_mask);
    if ( (setitimer(ITIMER_REAL, &itimer, NULL) != 0) ||
         (sigaction(SIGALRM, &action, NULL) != 0) )
    {
        perror("Ouch!");
        exit(EXIT_FAILURE);
    }
}


void osTaskDelay(const uint32_t timeout)
{
    const uint32_t targetTick = osTaskGetTicks() + timeout;
    while (osTaskGetTicks() < targetTick)
    {
        pause();
        //sSimIdle();
    }
}

bool osTaskDelayUntil(uint32_t *pPrevTick, const uint32_t incrTicks)
{
    const uint32_t targetTick = *pPrevTick + incrTicks;
    while (osTaskGetTicks() < targetTick)
    {
        pause();
        sSimIdle();
    }
    *pPrevTick = targetTick;
    return true;
}

uint32_t osTaskGetTicks(void)
{
    return sSimTicks;
}

uint32_t osTaskGetMsss(void)
{
    return sSimTicks;
}

// eof
