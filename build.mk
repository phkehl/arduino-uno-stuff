###############################################################################
#
# flipflip's Arduino Uno stuff
#
# Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>
#
###############################################################################

# verbose build?
VERBOSE     ?= 0

# print object sizes after compiling (0 = never, 1 = only for image, 2 = for all files)
SIZES       ?= 1

# choose a project name (single word, no spaces)
PROJECT     ?= dummy

# debugging and programming UART TX0/RX0
PROGPORT    ?= /dev/ttyACM0
DEBUGPORT   ?= $(PROGPORT)
PROGSPEED   ?= 115200
DEBUGSPEED  ?= $(PROGSPEED)

# avrdude settings programming mode (Arduino, AVR ISP, USBtinyISP, etc.)
AVRDUDEPROG ?= arduino
AVRDUDEARGS ?= -P $(DEBUGPORT) -b $(PROGSPEED) -D -V
#AVRDUDEARGS ?= -P /dev/ttyUSB0 -b 19200 -c avrisp -V
#AVRDUDEARGS ?= -c usbtiny -y -V -B 1
AVRDUDEARGS += -c $(AVRDUDEPROG) 

# path to stuff
FFDIR       ?= ../arduino-uno-stuff

# version of the printf() to include (min, norm, float)
PRINTF      ?= norm

# version of the scanf() to include (min, norm, float)
SCANF       ?= norm

# output directory
OBJDIR      ?= obj_$(PROJECT)

# project files
PROJFILES   ?= $(wildcard *.c)


###############################################################################

# default PROJECT to directory name
ifeq ($(PROJECT),dummy)
_PROJECT := $(shell basename $(CURDIR))
ifeq ($(_PROJECT),)
$(warning You should set the 'PROJECT' variable!)
else
PROJECT := $(_PROJECT)
endif
endif

###############################################################################

# default target (FIXME can use .DEFAULT_GOAL?)
ifeq ($(MAKECMDGOALS),)
default: wtf
MAKECMDGOALS = wtf
endif

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
AVRPREFIX   := avr-
STRIP       := $(AVRPREFIX)strip
STRINGS     := $(AVRPREFIX)strings
CC          := $(AVRPREFIX)gcc
AS          := $(AVRPREFIX)as
LD          := $(AVRPREFIX)ld
NM          := $(AVRPREFIX)nm
OBJCOPY     := $(AVRPREFIX)objcopy
OBJDUMP     := $(AVRPREFIX)objdump
SIZE        := $(AVRPREFIX)size
GDB         := $(AVRPREFIX)gdb

AVRDUDE     := avrdude

LC_ALL      := C

###############################################################################

# 3rd party source code
ATOMDIR     := $(FFDIR)/3rdparty/atomthreads
LIGHTDIR    := $(FFDIR)/3rdparty/light_ws2812
AVRHD44780  := $(FFDIR)/3rdparty/avr-hd44780

# target platform
ATMEGANR    ?= 328p
MCU         := atmega$(ATMEGANR)
F_CPU       := 16000000UL

# external stuff versions
FFVERSION   := $(shell cd $(FFDIR)/src && ../tools/version.pl)
GCCVERSION  := $(shell $(CC) --version | $(HEAD) -n1)
ATOMVERSION := $(shell $(CAT) $(ATOMDIR)/_version_)

# build version
BUILDVER    := $(shell $(PERL) $(FFDIR)/tools/version.pl)
BUILDVEROLD := $(shell $(SED) -n '/FF_BUILDVER/s/.*"\(.*\)".*/\1/p' $(OBJDIR)/version_gen.h 2>/dev/null)
BUILDDATE   := $(shell $(DATE) +"%Y-%m-%d %H:%M")

# make as far as possible, build multiple targers in parallel
MAKEFLAGS   := -k $(JFLAG) -j$(shell $(AWK) '/^processor/ { j = j + 1; } END { print j }' /proc/cpuinfo)


###############################################################################

# preprocessor defines
DEFS        += -DF_CPU=$(F_CPU)
DEFS        += -DATOM_FLIPFLIP
DEFS        += -DATOM_USE_SLEEP
DEFS        += -DATOM_STACK_CHECKING

# compiler flags
CFLAGS      += -mmcu=$(MCU) -g3 -O1
CFLAGS      += -pipe -std=gnu99 #--param max-inline-insns-single=500
CFLAGS      += -Wall -Wpadded -Wextra -Wstrict-prototypes # -Werror -Wpedantic
CFLAGS      += -fno-common -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
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
LDFLAGS     += -mmcu=$(MCU) -Wl,-Map=$(OBJDIR)/$(PROJECT).map,--cref -Wl,-gc-sections
#LDFLAGS     += -Wl,--defsym=__stack=sMainInitStack
LDLIBS      += -lm
#LDLIBS      += -nostdlib -lm -lc -lgcc -lc

# librarires linker flags
ifeq ($(PRINTF),min)
#$(info lo-fi printf())
LDLIBS      += -Wl,-u,vfprintf -lprintf_min
else ifeq ($(PRINTF),norm)
#$(info normal printf())
LDLIBS      +=
else ifeq ($(PRINTF),float)
#$(info hi-fi printf() (incl. float conversion))
LDLIBS      += -Wl,-u,vfprintf -lprintf_flt -lm
else
$(error Wrong value for PRINTF variable!)
endif

ifeq ($(SCANF),min)
#$(info lo-fi scanf())
LDLIBS      += -Wl,-u,vfscanf -lscanf_min
else ifeq ($(SCANF),norm)
#$(info  normal scanf())
LDLIBS      +=
else ifeq ($(SCANF),float)
#$(info hi-fi scanf() (incl. float conversion))
LDLIBS      += -Wl,-u,vfscanf -lscanf_flt
else
$(error Wrong value for SCANF variable!)
endif

###############################################################################

# source code files to compile
SRCFILES    += $(wildcard $(ATOMDIR)/*.c) $(wildcard $(ATOMDIR)/*.s)
SRCFILES    += $(wildcard $(LIGHTDIR)/*.c)
SRCFILES    += $(wildcard $(AVRHD44780)/*.c)
SRCFILES    += $(wildcard $(FFDIR)/src/*.c)
SRCFILES    += $(PROJFILES)

# include directories and flags
SRCDIRS     += $(sort $(dir $(SRCFILES))) $(OBJDIR)
INCFLAGS    += $(strip $(foreach dir, $(SRCDIRS), -I$(dir)))

# object files to generate (compile, assemble)
OFILES      +=

# makes compile rule for .c files
define avrMakeCompileRuleC
#$ (info avrMakeCompileRuleC $(1) --> $(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.c,%.o,$(1)))))
OFILES += $(OBJDIR)/$(subst /,__,$(subst ..,,$(subst $(FFDIR)/,,$(patsubst %.c,%.o,$(1)))))
$(OBJDIR)/$(subst /,__,$(subst ../,__,$(subst $(FFDIR)/,,$(patsubst %.c,%.o,$(1))))): $(1) $(MAKEFILE_LIST)
	@echo "$(HLY)C $$< $(HLR)$$@$(HLO)"
	$(V)$(CC) -c -o $$@ $$(CFLAGS) $(DEFS) $(INCFLAGS) $$< -MD -MF $$(@:%.o=%.d) -MT $$@ -Wa,-adhlns=$$(@:.o=.lst)
	$(V)if [ "$(SIZES)" -ge 2 ]; then $(SIZE) -B $$@; fi
endef

# makes compile rule for .S files
define avrMakeCompileRuleS
#$ (info avrMakeCompileRuleC $(1) --> $(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.S,%.o,$(1)))))
OFILES += $(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.S,%.o,$(1))))
$(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.S,%.o,$(1)))): $(1) $(MAKEFILE_LIST)
	@echo "$(HLY)A $$< $(HLR)$$@$(HLO)"
	$(V)$(CC) -c -o $$@ $$(CFLAGS) $(DEFS) $(INCFLAGS) $$< -MD -MF $$(@:%.o=%.d) -MT $$@ -Wa,-adhlns=$$(@:.o=.lst)
	$(V)if [ "$(SIZES)" -ge 2 ]; then $(SIZE) -B $$@; fi
endef

# makes compile rule for .s files
define avrMakeCompileRules
#$ (info avrMakeCompileRuleC $(1) --> $(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.s,%.o,$(1)))))
OFILES += $(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.s,%.o,$(1))))
$(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.s,%.o,$(1)))): $(1) $(MAKEFILE_LIST)
	@echo "$(HLY)A $$< $(HLR)$$@$(HLO)"
	$(V)$(CC) -c -o $$@ -x assembler-with-cpp $$(CFLAGS) $(DEFS) $(INCFLAGS) $$< -MD -MF $$(@:%.o=%.d) -MT $$@ -Wa,-adhlns=$$(@:.o=.lst)
	$(V)if [ "$(SIZES)" -ge 2 ]; then $(SIZE) -B $$@; fi
endef

# create compile rules and populate $(OFILES) list
$(foreach sfile, $(filter %.s,$(SRCFILES)), $(eval $(call avrMakeCompileRules,$(sfile)))) # watch the spaces!
$(foreach Sfile, $(filter %.S,$(SRCFILES)), $(eval $(call avrMakeCompileRuleS,$(Sfile)))) # watch the spaces!
$(foreach cfile, $(filter %.c,$(SRCFILES)), $(eval $(call avrMakeCompileRuleC,$(cfile)))) # watch the spaces!

# dependency files
DFILES := $(patsubst %.o,$(OBJDIR)/%.d,$(notdir $(OFILES)))




# load available dependency files
ifneq ($(MAKECMDGOALS),debugmf)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(DFILES)
endif
endif
endif

###############################################################################

# make output directory
$(OBJDIR)/.dummy:
	@echo "$(HLG)* $(OBJDIR)/$(HLO)"
	$(V)$(MKDIR) -p $(OBJDIR)
	$(V)$(TOUCH) $@

# trigger generation of version_gen.h if necessary
ifneq ($(BUILDVER),$(BUILDVEROLD))
$(shell $(MKDIR) -p $(OBJDIR); $(TOUCH) $(OBJDIR)/.version_gen.h)
endif

# generate include files
$(OBJDIR)/version_gen.h: $(OBJDIR)/.dummy $(OBJDIR)/.version_gen.h
	@echo "$(HLV)G $@ $(HLO)"
	@echo "#ifndef __VERSION_GEN_H__" > $@
	@echo "#define __VERSION_GEN_H__" >> $@
	@echo "#define FF_GCCVERSION \"$(GCCVERSION)\"" >> $@
	@echo "#define FF_ATOMVERSION \"$(ATOMVERSION)\"" >> $@
	@echo "#define FF_FFVERSION \"$(FFVERSION)\"" >> $@
	@echo "#define FF_BUILDVER \"$(BUILDVER)\"" >> $@
	@echo "#define FF_BUILDDATE \"$(BUILDDATE)\"" >> $@
	@echo "#define FF_PROJECT \"$(PROJECT)\"" >> $@
	@echo "#endif" >> $@

$(OBJDIR)/src__main.o: $(OBJDIR)/version_gen.h


# image file derivatives to generate
IMGFILESEXT += .elf .lss .lst .sym .hex .srec .ramsym .bin .strings
IMGFILESEXT += _eeprom.hex _eeprom.srec _eeprom.bin
IMGFILESEXT += .size
IMGFILESEXT += #.cof

IMGNAME := img_$(PROJECT)

IMGDERIVS   := $(addprefix $(OBJDIR)/$(IMGNAME), $(IMGFILESEXT))

# image
.PHONY: img
img: $(OFILES) $(OBJDIR)/.dummy $(OBJDIR)/macros.txt $(IMGDERIVS)

# always compile main.c on all changes (for the version info)
$(filter %src__main.o, $(OFILES)): $(filter-out %src__main.o, $(OFILES))

# we generate .o files from .c and .S files
$(OFILES): $(OBJDIR)/.dummy Makefile

# generate list of predefined macros
$(OBJDIR)/macros.txt: $(OBJDIR)/$(IMGNAME).elf # $(MAKEFILE_LIST)
	@echo "$(HLV)G $@ $(HLO)"
	$(V)echo | $(CC) $(DEFS) $(CFLAGS) -dM -E - > $@

# link into .elf file
$(OBJDIR)/$(IMGNAME).elf: $(OFILES)
	@echo "$(HLM)L $(@)$(HLO) $(HLR)($(PROJECT), $(BUILDVER), $(BUILDDATE))$(HLO)"
	$(V)$(CC) -o $@ $(LDFLAGS) $(OFILES) $(LDLIBS)

# .size from .elf (section sizes)
SIZE += --mcu=$(MCU)
$(OBJDIR)/$(IMGNAME).size: $(OBJDIR)/$(IMGNAME).elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(SIZE) -Ax $(OBJDIR)/$(IMGNAME).elf > $@
	$(V)$(SIZE) -Bd $(OBJDIR)/$(IMGNAME).elf >> $@
	$(V)$(SIZE) -Bx $(OBJDIR)/$(IMGNAME).elf >> $@
	$(V)$(SIZE) -AC $(OBJDIR)/$(IMGNAME).elf >> $@
	$(V)$(SIZE) -Bd $(OFILES) >> $@
	$(V)if [ "$(SIZES)" -ge 1 ]; then $(SIZE) -AC $(OBJDIR)/$(IMGNAME).elf; fi

# .lss from .elf (extended listing)
%.lss: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJDUMP) -t -h $< > $@

# .lst from .elf (disassembly)
%.lst: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJDUMP) -h -S $< > $@

# .sym from .elf (symbol table)
%.sym: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(NM) -n $< > $@

# .hex/.srec/.bin from .elf
OBJCOPYIMG := $(OBJCOPY) # -j .text -j .data
%.hex: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJCOPYIMG) -O ihex $< $@
%.srec: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJCOPYIMG) -O srec $< $@
	$(V)$(CHMOD) -x $@
%.bin: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJCOPYIMG) -O binary $< $@
	$(V)$(CHMOD) -x $@

# .ramsym ramsybols
%.ramsym: %.elf $(FFDIR)/tools/ramsyms.pl
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJDUMP) -t $< | $(PERL) $(FFDIR)/tools/ramsyms.pl > $@

# .strings list of strings in binary
%.strings: %.bin
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(STRINGS) $< > $@

OBJCOPYEEP := $(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0

# _eeprom.hex from .elf (eeprom contents)
%_eeprom.hex: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJCOPYEEP) -O ihex $< $@ $(V2)

# _eeprom.srec from .elf (eeprom contents)
%_eeprom.srec: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJCOPYEEP) -O srec $< $@ $(V2)
	$(V)$(CHMOD) -x $@

# _eeprom.bin from .elf (eeprom contents)
%_eeprom.bin: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJCOPYEEP) -O binary $< $@ $(V2)
	$(V)$(CHMOD) -x $@

# .cof from .elf
%.cof: %.elf
	@echo "$(HLV)G $@$(HLO)"
	$(V)$(OBJCOPY) --debugging --change-section-address .data-0x800000 --change-section-address .bss-0x800000 --change-section-address .noinit-0x800000 --change-section-address .eeprom-0x810000 -O coff-avr $< $@

###############################################################################

# program/flash
.PHONY: prog
prog: img
	@echo "$(HLY)P $(OBJDIR)/$(IMGNAME).hex$(HLO)"
	$(V)$(AVRDUDE) $(AVRDUDEARGS) -p m$(ATMEGANR) -U flash:w:$(OBJDIR)/$(IMGNAME).hex
#-U eeprom:w:$(OBJDIR)/$(IMGNAME).eep

# run serial debug tool
.PHONY: debug
debug: $(FFDIR)/tools/debug.pl $(DEBUGPORT)
	@echo "$(HLG)R $^ $(DEBUGSPEED)$(HLO)"
	$(V)$(PERL) $^ $(DEBUGSPEED)

# show AVR chip fuses and stuff
.PHONY: chipinfo
chipinfo:
	$(V)$(AVRDUDE) $(AVRDUDEARGS) -p m$(ATMEGANR) -v

# run simavr
.PHONY: simavr
simavr: img $(FFDIR)/3rdparty/simavr/simavr/run_avr
	@echo "$(HLG)* simavr$(OBJDIR)/$(HLO)"
	$(V)$(FFDIR)/3rdparty/simavr/simavr/run_avr -f $(F_CPU) -m $(MCU) -v -v -v -v $(OBJDIR)/$(IMGNAME).elf

.PHONY: simavrgdb
simavrgdb: img $(FFDIR)/3rdparty/simavr/simavr/run_avr
	@echo "$(HLG)* simavr$(OBJDIR)/$(HLO)"
	@echo -e "\nRun in another terminal:\n\n  $(GDB) --tui --eval-command \"file $(OBJDIR)/$(IMGNAME).elf\" --eval-command \"target remote localhost:1234\" --eval-command \"load\" --eval-command \"break main\" --eval-command \"continue\"\n\n"
	$(V)$(FFDIR)/3rdparty/simavr/simavr/run_avr -f $(F_CPU) -m $(MCU) -v -v -v -v -g $(OBJDIR)/$(IMGNAME).elf

# cleanup
.PHONY: clean
clean:
	@if [ -d "$(OBJDIR)" ]; then echo "$(HLM)* removing $(OBJDIR) dir$(HLO)"; fi
ifneq ($(OBJDIR),)
	$(V)if [ -d "$(OBJDIR)" ]; then $(RM) -rf $(OBJDIR); fi
else
	@echo "$(HLR)ERROR: no OBJDIR!!!$(HLO)"
endif

.PHONY: distclean
distclean:
	@echo "$(HLM)* removing obj_* *~$(HLO)"
	$(V)$(RM) -rf obj_*
	$(V)$(RM) -f *~

###############################################################################

# wtf
.PHONY: wtf
wtf:
	@echo "Make what?! (Try 'make help'.)"

# help
.PHONY: help
help:
	@echo
	@echo "Usage: make <target> [VERBOSE=0|1] [...=...]"
	@echo
	@echo "Where <target> is an object file (e.g. $(OBJDIR)/foo.o) or one the following:"
	@echo
	@echo "  img -- builds the image (and more), i.e. compiles and links all sources"
	@echo ""
	@echo "  prog -- programs the image to the Arduino"
	@echo "          (configuration PROGPORT and AVRDUDEARGS)"
	@echo ""
	@echo "  debug -- connect to debug port and display the debug output"
	@echo "           (configuration DEBUGPORT)"
	@echo ""
	@echo "  clean -- remove the output directory"
	@echo ""
	@echo "  distclean -- remove the output directory and backup files"
	@echo ""
	@echo "  debugmf -- display makefile debugging info"
	@echo ""
	@echo "  simavr / simavrgdb -- run in AVR simulator / ... for GDB debugging"
	@echo ""
	@echo "Typical usage while developing:"
	@echo ""
	@echo "  make prog && make debug"
	@echo ""
	@echo "Specify configuration on the command line (e.g. make debug DEBUGPORT=/dev/ttyUSB0)"
	@echo "or in the Makefile (e.g. DEBUGPORT := /dev/ttyUSB0)."
	@echo ""
	@echo "Happy hacking!"
	@echo ""

###############################################################################

# makefile debugging
.PHONY: debugmf
debugmf:
	@echo "----------------------------------------------------------------------"
	@echo "PROJECT='$(PROJECT)'"
	@echo "BUILDVER='$(BUILDVER)'"
	@echo "BUILDVEROLD='$(BUILDVEROLD)'"
	@echo "BUILDDATE='$(BUILDDATE)'"
	@echo "----------------------------------------------------------------------"
	@echo "DEBUGPORT='$(DEBUGPORT)'"
	@echo "PROGPORT='$(PROGPORT)'"
	@echo "AVRDUDEARGS='$(AVRDUDEARGS)'"
	@echo "----------------------------------------------------------------------"
	@echo "FFDIR='$(FFDIR)'"
	@echo "ATOMDIR='$(ATOMDIR)'"
	@echo "LIGHTDIR='$(LIGHTDIR)'"
	@echo "OBJDIR=$(OBJDIR)"
	@echo "----------------------------------------------------------------------"
	@echo "GCCVERSION=$(GCCVERSION)"
	@echo "ATOMVERSION=$(ATOMVERSION)"
	@echo "FFVERSION=$(FFVERSION)"
	@echo "----------------------------------------------------------------------"
	@echo "SRCFILES=$(SRCFILES)" | $(FMT) -t -w 120
	@echo "SRCDIRS=$(SRCDIRS)" | $(FMT) -t -w 120
	@echo "----------------------------------------------------------------------"
	@echo "PWD=$(PWD)"
	@echo "CURDIR=$(CURDIR)"
	@echo "TERM=$(TERM)"
	@echo "----------------------------------------------------------------------"
	@echo -E 'DEFS=$(DEFS)' | $(FMT) -t -w 120
	@echo "CFLAGS=$(CFLAGS)" | $(FMT) -t -w 120
	@echo "LDFLAGS=$(LDFLAGS)" | $(FMT) -t -w 120
	@echo "LDLIBS=$(LDLIBS)" | $(FMT) -t -w 120
	@echo "INCFLAGS=$(INCFLAGS)" | $(FMT) -t -w 120
	@echo "----------------------------------------------------------------------"
	@echo "OFILES=$(OFILES)" | $(FMT) -t -w 120
	@echo "DFILES=$(DFILES)" | $(FMT) -t -w 120
	@echo "----------------------------------------------------------------------"

# eof
