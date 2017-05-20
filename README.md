# flipflip's Arduino Uno Stuff

https://oinkzwurgl.org/projaeggd/arduino-uno-stuff/

## Introduction

This is my Arduino Uno™ (and others) stuff. It's a build system, an "operating
system" and a collection of utility routines for easy programming of an Arduino
Uno without using the Arduino Software. Instead it bases directly on the AVR
Libc and other tools to enable "low-level" c programming of the Arduino. While
this makes it impossible to use any of the nice Arduino libraries it provides
more control and a, perhaps a more familiar, development setup for advanced
users.

Most importantly it comes with a real-time scheduler (currently based on a
modified Atomthreads) that allows to run multiple tasks. The modifications
include runtime per-thread and total system load indicators (on serial port
debug output and via a LED). A system task is compiled-in and started
automatically. It asserts the watchdog and outputs system monitoring information
(task load, stack high water marks, and more).

## Usage

Setting up the environment:

```
$ sudo apt install avr-libc avrdude gcc-avr gdb-avr binutils-avr libdevice-serialport-perl doxygen texi2html
```

Building everything:

```
$ make all
```

or one or more of:

```
$ make doc
$ make examples
$ make projects
```

Then browse to doc/html/index.html.

Build single example or project:

```
$ cd examples/ex3
$ make help
```
