#!/usr/bin/perl

use strict;
use warnings;

my @digital;
my @analog;


# ATmega328P (Arduino Uno)
if (0)
{
    @digital =
    (
    #     0   1   2   3   4   5   6   7   8   9   10  11  12  13
      qw(PD0 PD1 PD2 PD3 PD4 PD5 PD6 PD7 PB0 PB1 PB2 PB3 PB4 PB5),
    );

    @analog =
    (
     qw(PC0 PC1 PC2 PC3 PC4 PC5),
    );
}


# ATmega2560 (Arduino Mega)
if (0)
{
    @digital =
    (
    #     0   1   2   3   4   5   6   7   8   9   10  11  12  13
      qw(PE0 PE1 PE4 PE5 PG5 PE3 PH3 PH4 PH5 PH6 PB4 PB5 PB6 PB7),
    #     14  15  16  17  18  19  20  21
      qw(PJ1 PJ0 PH1 PH0 PD3 PD2 PD1 PD0),
    #     22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37
      qw(PA0 PA1 PA2 PA3 PA4 PA5 PA6 PA7 PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0),
    #     38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53
      qw(PD7 PG2 PG1 PG0 PL7 PL6 PL5 PL4 PL3 PL2 PL1 PL0 PB3 PB2 PB1 PB0)
    );

    @analog =
    (
     qw(PF0 PF1 PF2 PF3 PF4 PF5 PF6 PF7 PK0 PK1 PK2 PK3 PK4 PK5 PK6 PK7),
    );
}


for (my $n = 0; $n <= $#digital; $n++)
{
    my $p = $digital[$n];

    my $name1 = substr('_' . 'D' . $n . '   ', 0, 4);
    my $name2 = substr('_' . $p . '   ', 0, 4);
    #printf(STDERR "%2i %s %s %s\n", $n, $p, $name1);

    my $uid = ((ord(substr($p, 1, 1)) - ord('A') + 1) << 8) + substr($p, 2) + 1;

    foreach my $name ($name1, $name2)
    {
        printf("#  define PIN_PORT_$name   PORT%s\n", substr($p, 1, 1));
        printf("#  define PIN_DDR_$name    DDR%s\n", substr($p, 1, 1));
        printf("#  define PIN_BIT_$name    %s\n", $p);
        printf("#  define PIN_PIN_$name    PIN%s\n", substr($p, 1, 1));
        printf("#  define PIN_UID_$name    0x%04x\n", $uid);
    }
    printf("\n");
}


for (my $n = 0; $n <= $#analog; $n++)
{
    my $p = $analog[$n];

    my $name1 = substr('_' . 'A' . $n . '   ', 0, 4);
    my $name2 = substr('_' . $p . '   ', 0, 4);
    #printf(STDERR "%2i %s %s %s\n", $n, $p, $name1, $name2);

    my $uid = ((ord(substr($p, 1, 1)) - ord('A') + 1) << 8) + substr($p, 2) + 1;

    foreach my $name ($name1, $name2)
    {
        printf("#  define PIN_PORT_$name   PORT%s\n", substr($p, 1, 1));
        printf("#  define PIN_DDR_$name    DDR%s\n", substr($p, 1, 1));
        printf("#  define PIN_BIT_$name    %s\n", $p);
        printf("#  define PIN_PIN_$name    PIN%s\n", substr($p, 1, 1));
        printf("#  define PIN_UID_$name    0x%04x\n", $uid);
    }
    printf("\n");
}
