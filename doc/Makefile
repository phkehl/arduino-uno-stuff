###############################################################################
#
# flipflip's Arduino Uno Stuff
#
# Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>
#
###############################################################################

# verbose build?
VERBOSE     ?= 0

###############################################################################

# verbosity helpers
ifeq ($(VERBOSE),1)
V =
V1 =
V2 =
V12 =
else
V = @
V1 = > /dev/null
V2 = 2> /dev/null
V12 = 2>&1 > /dev/null
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
SVNVERSION  := svnversion
CHMOD       := chmod

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

ifeq ($(VERBOSE),1)
RM  := $(RM) -v
TAR := $(TAR) -v
endif

###############################################################################

VERSION := $(shell ../tools/version.pl)

doc: avrlibc_html avrlibc_man stuff_doc

FFDIR := $(subst /docXX,,$(CURDIR)XX)

DEPS :=  $(wildcard ../src/*.c)        $(wildcard ../src/*.h) \
	     $(wildcard ../examples/*/*.h) $(wildcard ../examples/*/*.c) \
	     $(wildcard ../projects/*/*.h) $(wildcard ../projects/*/*.c)

# generate stuff documentation
stuff_doc: html/index.html
html/index.html: Makefile Doxyfile docu.h $(DEPS)
# remove previous output and create new output directory
	$(V)$(RM) -rf html
	$(V)$(MKDIR) -p html
# generate documentation (run the doxygen tool)
	@echo "$(HLG)R Doxygen$(HLR) html/index.html$(HLO)"
	$(V)(\
		$(CAT) Doxyfile; \
		echo "PROJECT_NUMBER = $(VERSION)"; \
	) | $(DOXYGEN) - | $(TEE) html/doxygen.log $(V1)
# make paths in the logfiles relative
	$(V)$(SED) -i -r -e 's@$(FFDIR)/@@g' -e '/^$$/d' html/warnings.log html/doxygen.log
# remove some known won't-fix warnings in the logfile
	$(V)$(PERL) ../tools/doxylogfix.pl < html/warnings.log > html/warnings_clean.log
	$(V)if [ -s html/warnings_clean.log ]; then \
		$(CAT) html/warnings_clean.log; \
	else \
		echo "no warnings" > html/warnings_clean.log; \
	fi
# inject the warnings and doxygen logfiles into the generated HTML
	$(V)$(SED) -i \
		-e '/%DOXYGEN_LOG/r html/doxygen.log' \
		-e '/%DOXYGEN_WARNINGS/r html/warnings_clean.log' \
		-e '/%DOXYGEN_WARNINGS/s/%DOXYGEN_WARNINGS%//' \
		-e '/%DOXYGEN_LOG/s/%DOXYGEN_LOG%//' \
		html/P_DOXYGEN.html
# make docu files world-readable
	$(V)$(CHMOD) -R a+rX html


# extract avr-libc html docu
avrlibc_html: avr-libc/html/index.html
avr-libc/html/index.html: avr-libc/html.tar.gz
	@echo "$(HLG)R tar -x $(HLR) $^$(HLO)"
	$(V)cd avr-libc && $(TAR) -xzf html.tar.gz
	$(V)$(TOUCH) $@

# extract avr-libc man pages
avrlibc_man: avr-libc/man/avr_version.3
avr-libc/man/avr_version.3: avr-libc/man.tar.gz
	@echo "$(HLG)R tar -x $(HLR) $^$(HLO)"
	$(V)cd avr-libc && $(TAR) -xzf man.tar.gz
	$(V)$(TOUCH) $@

clean:
	@echo "$(HLM)* removing html dir$(HLO)"
	$(V)$(RM) -rf html

distclean:
	@echo "$(HLM)* removing html avr-libc/html avr-libc/man *~$(HLO)"
	$(V)$(RM) -rf html *~ avr-libc/html avr-libc/man

###############################################################################
# eof
