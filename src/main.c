/*!
    \file
    \brief flipflip's Arduino Uno stuff: main program (see \ref FF)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup FF
    @{
*/


#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "version_gen.h"   // ff: generated version information
#include "hw.h"            // ff: hardware
#include "os.h"            // ff: operating system
#include "debug.h"         // ff: debugging
#include "sys.h"           // ff: system task

// to be implemented by the projects
extern void appInit(void);
extern void appCreateTask(void);

//! main function
int main(void)
{
    // use some of the later system task stack for the initialisation
    SP = sysGetInitStackPointer();

    // initialise the hardware
    hwInit();

    // initialise the debug output early
    debugInit();

    // initialise the operating system
    osInit();

    // initialise the system task
    sysInit();

    // initialise application module
    appInit();

    // say hello
    NOTICE("***** " FF_PROJECT " (" FF_BUILDVER " " FF_BUILDDATE ") *****");
    PRINT("-- Stuff " FF_FFVERSION " -- Copyright (c) Philippe Kehl. All rights reserved. --");
    PRINT("-- Atomthreads " FF_ATOMVERSION " -- Copyright (c) Kelvin Lawson. All rights reserved. --");
    PRINT("-- GCC " FF_GCCVERSION " -- Copyright (c) Free Software Foundation, Inc. --");
    PRINT("-- Credits to the Internets, the Arduino forum, et al. --");
    PRINT("-- See source code and documentation for details. Don't be a troll. --");

    // create the system task
    sysCreateSystemTask();

    // create the application task
    appCreateTask();

    // start the scheduler (i.e. the tasks)
    osTaskStartScheduler();

    // we shouldn't end up here..
    hwPanic(HW_PANIC_OTHER, 0x01, 0);

    // we'll never end up here..
	return 0;
}



//@}
// eof
