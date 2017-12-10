###############################################################################
#
# flipflip's Arduino Uno stuff: LED FX Simulator
#
# Copyright (c) 2014-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>
#
###############################################################################

include ../sim.mk

.PHONY: main
main: main.c ../sim.c ../sim.h ../../src/ledfx.c ../../src/hsv2rgb.c
	$(CC) -o $@ -I. -I.. -I../../src $(CFLAGS) $(DEFS) $(INCFLAGS) $^ $(LDLIBS)
	./$@


# eof
