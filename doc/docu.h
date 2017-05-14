#ifndef __DOCU_H_
#define __DOCU_H__

/*!

\file
\brief documentation (see \ref mainpage)

\mainpage
\anchor mainpage

\tableofcontents

\section P_INTRO Introduction

This is my [Arduino Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno)&trade; (and others) stuff.
It's a build system, an "operating system" and a collection of utility routines for easy programming
of an Arduino Uno _without_ using the [Arduino Software](https://www.arduino.cc/en/Main/Software).
Instead it bases directly on the [AVR Libc](http://www.nongnu.org/avr-libc/) and other tools to
enable "low-level" c programming of the Arduino. While this makes it impossible to use any of the
nice Arduino libraries it provides more control and a, perhaps a more familiar, development setup
for advanced users.

Most importantly it comes with a real-time scheduler (currently based on a modified
[Atomthreads](http://atomthreads.com/)) that allows to run multiple tasks. The modifications include
runtime per-thread and total system load indicators (on serial port debug output and via a LED).  A
system task is compiled-in and started automatically. It asserts the watchdog, runs the buffered
serial transmit and outputs system monitoring information (task load, stack high water marks, and
more).

See [source code](files.html) for copyrights, sources and more.

Credits: the Internet, AVR freaks forum, et al. (see [source code](files.html)).

See https://oinkzwurgl.org/projaeggd/arduino-uno-stuff/ for the latest version.

\section P_API API Reference

- \ref CONFIG
- \ref DEBUG
- \ref HW
- \ref OS
- \ref SYS
- \ref STDSTUFF
- \ref UNOPINS
- \ref HSV2RGB
- \ref WS2801
- \ref WS2812
- \ref LEDFX
- \ref FXLOOP
- \ref UBX
- \ref GNSS
- \ref ALIMATRIX
- See also the [source code](files.html), the \ref P_REFERENCES below, the \ref EXAMPLES, the \ref PROJECTS
  and the [AVR libc docu](../avr-libc/html/index.html).



\section P_HW Hardware Resources

Some hardware resources are reserved for stuff functions:

- Pin D0 (PD0/RXD) and pin D1 (PD1/TXD) for serial port receive and transmit at baudrate 115200
  - Configuration: #FF_HW_RX_BUFSIZE and #FF_HW_TX_BUFSIZE
- Pin A0 as a system load and status indicator.
  - load: The LED is lit whenever a thread (other than the idle thread) is running.
  - status: The LED blinks on system panic (via hwPanic()) and resets afterwards
  - The pin for this functionality can be configured via #FF_HW_LOAD_PIN.
- Pin A1 has the 1kHz system tick signal
  - The pin for this functionality can be configured via #FF_HW_TICK_PIN.
- Timer/Counter0 is used for the 1kHz system tick, which runs the scheduler
  - This currently cannot be configured.
- Timer/Counter1 (TCCR1) is used for runtime measurements (hwTIC() and hwTOC() functions)
  - Configuration: #FF_HW_NUM_TICTOC
- Some of the SRAM is reserved and required for the system.
  - Configuration: #FF_HW_RX_BUFSIZE, #FF_HW_TX_BUFSIZE, #FF_SYS_TASK_STACK, #FF_OS_IDLE_STACK, #FF_OS_HEAP_SIZE



\section P_EXAMPLES Examples

See \ref EXAMPLES.



\section P_PROJECTS Projects

See \ref PROJECTS.



\section P_REFERENCES References

- [[AVR8271](../avr_doc8271S_atmega_48_88_168_328.pdf)]
  8-bit Microcontroller with 4/8/16/32K Bytes In-System Programmable Flash ATmega48A / ATmega48PA / ATmega88A /
  ATmega88PA / ATmega168A / ATmega168PA / ATmega328 / ATmega328P -- Datasheet / Manual, Rev. 8271C–AVR–08/10

- [[AVR0856](../avr_doc0856_avr_instruction_set_manual.pdf)]
  AVR Instruction Set Manual

- [[avr-libc](../avr-libc/html/index.html)]
  AVR Libc (documentation), http://www.nongnu.org/avr-libc

- [[avrdude](../avrdude.html)]
  AVRDUDE (AVR Downloader/UploaDEr) by Brian S. Dean, http://www.nongnu.org/avrdude/

- [ATMEGA328P] Atmel 8-bit AVR RISC-based microcontroller (MCU)
  http://www.atmel.com/devices/atmega328p.aspx?tab=documents

- [[UnoRev3](../Arduino_Uno_Rev3-schematic.pdf)]
  Arduino Uno Rev3 Schematics

- [[NanoRev3](../ArduinoNano30Schematic.pdf)]
  Arduino Nano Rev3 Schematics

- [\ref ATOM]
  Open Source RTOS by Kelvin Lawson (http://atomthreads.com, https://github.com/kelvinlawson/atomthreads/)

- [avr-gcc]
  ABI and Extensions, https://gcc.gnu.org/wiki/avr-gcc

\section P_PINOUTS Pinouts

\subsection P_PINOUT_UNO Arduino Uno Pinout
(by Alberto (aka PighiXXX), http://www.pighixxx.com/test/pinoutspg/boards/)

\htmlonly
<a href="../unov3.png"><img src="../unov3.png" width="100%"/></a>
\endhtmlonly

\subsection P_PINOUT_NANO Arduino Nano Pinout
(by Alberto (aka PighiXXX), http://www.pighixxx.com/test/pinoutspg/boards/)

\htmlonly
<a href="../nano.png"><img src="../nano.png" width="100%"/></a>
\endhtmlonly

\subsection P_PINOUT_AVR ATmega328 Pinput
(by Alberto (aka PighiXXX), http://www.pighixxx.com/test/pinoutspg/processors/)

\htmlonly
<a href="../atmega328v3_0.png"><img src="../atmega328v3_0.png" width="100%"/></a>
\endhtmlonly

\subsection P_PINOUT_AVRMLF ATmega328 MLF Pinout
(by Alberto (aka PighiXXX), http://www.pighixxx.com/test/pinoutspg/processors/)

\htmlonly
<a href="../atmega382mlf.png"><img src="../atmega382mlf.png" width="100%"/></a>
\endhtmlonly



\section P_DEBUGGING Debugging

\subsection P_DEBUGGING_SERIAL Serial Port

Pretty-print serial port debugging output (see \ref DEBUG):

\verbatim
./tools/debug.pl /dev/ttyACM0 115200
\endverbatim

or

\verbatim
make debug
\endverbatim


\subsection P_DEBUGGING_SIMAVR Simavr Debugging

Compile [simavr](https://github.com/buserror/simavr):

\verbatim
cd 3rdparty
tar -xzvf simavr-1.4.tar.gz
cd simavr
make
\endverbatim

In the project directory:

\verbatim
make simavr
\endverbatim

or

\verbatim
make simavrgdb
\endverbatim

\todo use gtkwave to look at signals

In the Holy GNU Emacs, the One True Editor:
- M-x gdb avr-gdb -i=mi
- M-x gdb-many-windows
- commands
  - x $sp   examine stack pointer
  - break (b) [function][line]   -- or C-x space
  - delete (d) n       -- delete breakpoint
  - disable (dis) n    -- disable breakpoint
  - enable (ena) n     -- enable breakpoint
  - step (s)           -- next
  - next (n)           -- next over
  - continue (c)       -- resume
  - finish (fin)       -- step up
  - until (u) [function][line] -- step up & next line
  - bt backtrace

Or with nemiver: nemiver --remote=localhost:1234 --gdb-binary=/usr/bin/avr-gdb obj_.../img_...elf


\section P_NOTES Various Notes

\subsection P_NOTES_MEM328 Atmega 328p Memory Layout

From the datasheet:
- 0x0000-0x001f (32 bytes) CPU registers
- 0x0020-0x004f (64 bytes) I/O registers (SREG)
- 0x0060-0x00ff (160 bytes) ext. I/O registers
- 0x0100-0x08ff (2048 bytes) SRAM

In GCC: offset 0x0080000


\subsection P_NOTES_SP Stack Pointer (SP)

- \c __stack symbol is the initial SP (0x08ff, RAMEND)
- can just say SP=something in the program
- lives at address 0x5e (SPH, high byte) and 0x5d (SPL, low byte)
- can tell GCC to use a different __stack initialisation (e.g. `-Wl,--defsym=__stack=sMainInitStack`)


\subsection P_FUSES MCU Fuses

The fuse settings are (0 = programmed, 1 = unprogrammed):

- lfuse = 0xd7 = 01101 0111 (CKSEL = 0010, SUT = 10, CKDIV8 = 0)
  - bit7: 0 CKDIV8   -- divide clock by 8           -> clock is not divided by 8
  - bit6: 1 CKOUT    -- clock output enable         -> clock output is not enabled
  - bit5: 1 SUT1     -- select start-up time
  - bit4: 0 SUT0     -- select start-up time
  - bit3: 0 CKSEL3   -- select clock source
  - bit2: 0 CKSEL2   -- select clock source
  - bit1: 1 CKSEL1   -- select clock source
  - bit0: 0 CKSEL0   -- select clock source
- hfuse = 0xd9 = 1101 1001
  - bit7: 1 RSTDISBL -- external reset disable      -> external reset is not disabled
  - bit6: 1 DWEN     -- debug wire enable           -> debug wire is not enabled
  - bit5: 0 SPIEN    -- SPI programming             -> SPI programming is enabled
  - bit4: 1 WDTON    -- watchdog timer always on    -> wd timer is not always on
  - bit3: 1 EESAVE   -- EEPROM memory preserved.    -> not enabled
  - bit2: 0 BOOTSZ1  -- boot size select
  - bit1: 0 BOOTSZ0  -- boot size select
  - bit0: 1 BOOTRST  -- select reset vector
- efuse = 0xff = 0000 0111
  - bit7: 0 -
  - bit6: 0 -
  - bit5: 0 -
  - bit4: 0 -
  - bit3: 0 -
  - bit2: 1 BODLEVEL2 -- Brown-out Detector trigger level
  - bit1: 1 BODLEVEL1 -- Brown-out Detector trigger level
  - bit0: 1 BODLEVEL0 -- Brown-out Detector trigger level

Good fuse calculator at http://www.engbedded.com/fusecalc (-U lfuse:w:0xd7:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m)

Reanimate when lfuse wrongly 0: apply external clock on XTAL1 (e.g. 1MHz square from scope).

Get chip info: avrdude -p m328p -v -P /dev/ttyUSB0 -b 19200 -c avrisp

\subsection P_FTDI FTDI Cable

  - green: RTS (out)
  - yellow: RXD (in)
  - orange: TXD (out)
  - red: VCC (out)
  - brown: CTS (in)
  - black: GND (out)
  - [ftdi_cable.pdf](../ftdi_cable.pdf)


\page P_DOXYGEN Doxygen

\section P_DOXYGEN_WARNINGS Warnings

\verbatim
%DOXYGEN_WARNINGS%
\endverbatim

\section P_DOXYGEN_LOG Log

\verbatim
%DOXYGEN_LOG%
\endverbatim


\defgroup FF flifplip's Arduino Uno Stuff

\defgroup FF_APPS Applications
\ingroup FF

\defgroup EXAMPLES Examples
\ingroup FF_APPS

\defgroup PROJECTS Projects
\ingroup FF_APPS

*/

// \defgroup AVRLIBC AVR Libc
//
// \defgroup alloca
// \ingroup AVRLIBC
//
// \defgroup avr_assert
// \ingroup AVRLIBC
//
// \defgroup avr_errno
// \ingroup AVRLIBC
//
// \defgroup avr_inttypes
// \ingroup AVRLIBC
//
// \defgroup avr_math
// \ingroup AVRLIBC
//
// \defgroup setjmp
// \ingroup AVRLIBC
//
// \defgroup avr_stdint
// \ingroup AVRLIBC
//
// \defgroup avr_stdio
// \ingroup AVRLIBC
//
// \defgroup avr_stdlib
// \ingroup AVRLIBC
//
// \defgroup avr_string
// \ingroup AVRLIBC
//
// \defgroup avr_time
// \ingroup AVRLIBC
//
// \defgroup avr_boot
// \ingroup AVRLIBC
//
// \defgroup avr_builtins
// \ingroup AVRLIBC
//
// \defgroup avr_cpufunc
// \ingroup AVRLIBC
//
// \defgroup avr_eeprom
// \ingroup AVRLIBC
//
// \defgroup avr_fuse
// \ingroup AVRLIBC
//
// \defgroup avr_interrupts
// \ingroup AVRLIBC
//
// \defgroup avr_io
// \ingroup AVRLIBC
//
// \defgroup avr_lock
// \ingroup AVRLIBC
//
// \defgroup avr_pgmspace
// \ingroup AVRLIBC
//
// \defgroup avr_power
// \ingroup AVRLIBC
//
// \defgroup avr_sfr
// \ingroup AVRLIBC
//
// \defgroup avr_sfr_notes
// \ingroup AVRLIBC
//
// \defgroup avr_signature
// \ingroup AVRLIBC
//
// \defgroup avr_sleep
// \ingroup AVRLIBC
//
// \defgroup avr_version
// \ingroup AVRLIBC
//
// \defgroup avr_version
// \ingroup AVRLIBC
//
// \defgroup avr_watchdog
// \ingroup AVRLIBC
//
// \defgroup deprecated_items
// \ingroup AVRLIBC
//
// \defgroup compat_ina90
// \ingroup AVRLIBC
//
// \defgroup util_atomic
// \ingroup AVRLIBC
//
// \defgroup util_crc
// \ingroup AVRLIBC
//
// \defgroup util_delay_basic
// \ingroup AVRLIBC
//
// \defgroup util_delay
// \ingroup AVRLIBC
//
// \defgroup util_parity
// \ingroup AVRLIBC
//
// \defgroup util_setbaud
// \ingroup AVRLIBC
//
// \defgroup util_twi
// \ingroup AVRLIBC
//
// \defgroup ctype
// \ingroup AVRLIBC
//
// \defgroup demos
// \ingroup AVRLIBC
//
// \defgroup asmdemo
// \ingroup demos
//
// \defgroup twi_demo
// \ingroup demos
//
// \defgroup largedemo
// \ingroup demos
//
// \defgroup demo_project
// \ingroup demos
//
// \defgroup stdiodemo
// \ingroup demos
//



#endif // __DOCU_H__
// eof
