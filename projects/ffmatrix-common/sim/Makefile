###############################################################################
#
# flipflip's Arduino Uno stuff: LED Matrix Simulator
#
# Copyright (c) 2014-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>
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

FFDIR      := ../../..

# linker flags
LDLIBS      += -lm

# files to compile and link
SRCFILES    += $(wildcard *.c) $(wildcard $(FFDIR)/src/*_sim.c)
SRCFILES    += $(FFDIR)/src/ledfx.c
SRCFILES    += $(FFDIR)/src/hsv2rgb.c

# include directories and flags
SRCDIRS     += $(sort $(dir $(SRCFILES))) $(OBJDIR)
INCFLAGS    += $(strip $(foreach dir, $(SRCDIRS), -I$(dir))) -I$(FFDIR)/3rdparty/atomthreads

# object directory
OBJDIR     := obj

# object files
OFILES     :=

# target executable
EXECUTABLE := $(OBJDIR)/ffmatrix-sim

# default target
.PHONY: all
all: $(EXECUTABLE)

# makes compile rule for .c files
define simMakeCompileRuleC
#$ (info simMakeCompileRuleC $(1) --> $(OBJDIR)/$(subst /,__,$(subst $(FFDIR)/,,$(patsubst %.c,%.o,$(1)))))
OFILES += $(OBJDIR)/$(subst /,__,$(subst ..,,$(subst $(FFDIR)/,,$(patsubst %.c,%.o,$(1)))))
$(OBJDIR)/$(subst /,__,$(subst ../,__,$(subst $(FFDIR)/,,$(patsubst %.c,%.o,$(1))))): $(1) $(MAKEFILE_LIST) | $(OBJDIR)
	@echo "$(HLY)C $$< $(HLR)$$@$(HLO)"
	$(V)$(CC) -c -o $$@ $$(CFLAGS) $(DEFS) $(INCFLAGS) $$< -MD -MF $$(@:%.o=%.d) -MT $$@
endef

# create compile rules and populate $(OFILES) list
$(foreach cfile, $(filter %.c,$(SRCFILES)), $(eval $(call simMakeCompileRuleC,$(cfile)))) # watch the spaces!

# dependency files
DFILES := $(patsubst %.o,$(OBJDIR)/%.d,$(notdir $(OFILES)))

###############################################################################

# include dependency rules
-include $(DFILES)

$(OBJDIR):
	$(V)$(MKDIR) -p $@

$(EXECUTABLE): $(OFILES)
	@echo "$(HLM)L $@$(HLO)"
	$(V)$(CC) -o $@ $(LDFLAGS) $(OFILES) $(LDLIBS)

# cleanup
.PHONY: clean
clean:
	@if [ -d "$(OBJDIR)" ]; then echo "$(HLM)* removing $(OBJDIR) dir$(HLO)"; fi
ifneq ($(OBJDIR),)
	$(V)if [ -d "$(OBJDIR)" ]; then $(RM) -rf $(OBJDIR); fi
else
	@echo "$(HLR)ERROR: no OBJDIR!!!$(HLO)"
endif

.PHONY: debugmf
debugmf:
	@echo "SRCFILES=$(SRCFILES)"
	@echo "OFILES=$(OFILES)"

# eof
