###############################################################################
#
# flipflip's Arduino Uno stuff: offline (Linux) simulator
#
# Copyright (c) 2017-2018 Philippe Kehl <flipflip at oinkzwurgl dot org>
#
###############################################################################

# verbose build?
VERBOSE     ?= 0

# verbosity helpers
ifeq ($(VERBOSE),1)
V =
V1 =
V2 =
V12 =
OBJCOPY += -v
RM += -v
else
V = @
V1 = > /dev/null
V2 = 2> /dev/null
V12 = 2>&1 > /dev/null
endif

# disable fancy stuff for dumb terminals (e.g. Emacs compilation window)
fancyterm := true
ifeq ($(TERM),dumb)
fancyterm := false
endif
ifeq ($(TERM),)
fancyterm := false
endif
ifeq ($(fancyterm),true)
HLR="[31m"
HLG="[32m"
HLY="[33m"
HLV="[35m"
HLM="[1\;36m"
HLO="[m"
else
HLR=
HLG=
HLY=
HLV=
HLM=
HLO=
endif

# tools
PERL        := perl
SED         := sed
SHELL       := bash
TOUCH       := touch
RM          := rm
MKDIR       := mkdir
HEAD        := head
AWK         := gawk
TEE         := tee
DOXYGEN     := doxygen
CAT         := cat
TAIL        := tail
TAR         := tar
LS          := ls
DATE        := date
FMT         := fmt
SVN         := svn
CHMOD       := chmod
STRINGS     := strings

# toolchain
GCCPREFIX   :=
STRIP       := $(GCCPREFIX)strip
STRINGS     := $(GCCPREFIX)strings
CC          := $(GCCPREFIX)gcc
AS          := $(GCCPREFIX)as
LD          := $(GCCPREFIX)ld
NM          := $(GCCPREFIX)nm
OBJCOPY     := $(GCCPREFIX)objcopy
OBJDUMP     := $(GCCPREFIX)objdump
SIZE        := $(GCCPREFIX)size
GDB         := $(GCCPREFIX)gdb

LC_ALL      := C

###############################################################################

# preprocessor defines
DEFS        +=

# compiler flags
CFLAGS      += -g3 -O1
CFLAGS      += -pipe -std=gnu99 #--param max-inline-insns-single=500
CFLAGS      += -Wall -Wpadded -Wextra -Wstrict-prototypes # -Werror -Wpedantic
CFLAGS      += -fno-common -funsigned-char -funsigned-bitfields -fpack-struct #-fshort-enums
CFLAGS      += -Wunused -Wunused-parameter -Wwrite-strings # -Wold-style-definitions
CFLAGS      += -Wnested-externs -Wformat=2 -Wunused-result # -Wredundant-decls
CFLAGS      += -ffunction-sections -Wjump-misses-init -Wlogical-op
CFLAGS      += -fno-strict-aliasing -fdata-sections -Wunused-variable
CFLAGS      += -Wmissing-prototypes -Werror-implicit-function-declaration
CFLAGS      += -Wpointer-arith -Wchar-subscripts -Wcomment -Wimplicit-int -Wmain -Wparentheses
CFLAGS      += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas
CFLAGS      += -Wfloat-equal -Wundef -Wshadow -Wsign-compare -Waggregate-return
CFLAGS      += -Wmissing-declarations -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CFLAGS      += -Wnested-externs -Winline -Wlong-long -Wcast-align
#CFLAGS      += -Wunreachable-code -Wbad-function-cast -Wpacked
#CFLAGS      += -Wa,--gstabs
#CFLAGS      += -ffunction-sections -fdata-sections

# linker flags
LDLIBS      += -lm

INCFLAGS    := -I. -I../src

###############################################################################

SIMULATORS := ffmatrix8x8

.PHONY: all
all: $(SIMULATORS)

.PHONY: clean
clean:
	$(V)$(RM) -f $(SIMULATORS)


ffmatrix8x8: ffmatrix.c Makefile $(wildcard *.c) $(wildcard *.h) $(wildcard ../src/*.c) $(wildcard ../src/*.h)
	$(CC) -o $@ $(CFLAGS) $(DEFS) $(INCFLAGS) \
		-DFF_LEDFX_NUM_X=8 -DFF_LEDFX_NUM_Y=8 -DFF_LEDFX_NUM_LED=64 -DFF_LEDFX_XY_ARR=4 \
		$<  sim.c sdl.c ../src/ledfx.c ../src/hsv2rgb.c $(LDLIBS) -lSDL2 -lSDL2_ttf


# eof
