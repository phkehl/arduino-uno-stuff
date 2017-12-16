#!/usr/bin/perl
################################################################################
#
# flipflip's Arduino Uno stuff: RTTTL to melody converter
# (for tone.c's toneMelody() function)
#
# See https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language.
#
# Copyright (c) 2017 Philippe Kehl <flipflip at oinkzwurgl dot org>
#
################################################################################

use strict;
use warnings;

help () if ($#ARGV != 0);
sub help
{
    print(STDERR "Usage: $0 <rtttl string>\n",
          "E.g.: $0 \"IndianaShort:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.\"\n",
          "See https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language for details.\n");
    exit(1);
}

my $debug = 0;

my $rtttl = $ARGV[0];

printf(STDERR "rttl=[%s]\n", $rtttl) if ($debug);

my $name;
my $defaultOctave = 6;
my $defaultDuration = 4;
my $bpm = 63;
my @notes = ();

if ($rtttl =~ m{^\s*([^:]+)\s*:\s*([^:]+)\s*:\s*(.+)$})
{
    $name = $1;
    my $defaults = $2;
    my $notesStr = $3;
    printf(STDERR "name=[%s] defaults=[%s] notes=[%s]\n", $name, $defaults, $notesStr) if ($debug);

    if ($defaults =~ m{o=([4567])})
    {
        $defaultOctave = 1 * $1;
    }
    if ($defaults =~ m{d=(1|2|4|8|16|32)})
    {
        $defaultDuration = 1 * $1;
    }
    if ($defaults =~ m{b=(\d+)})
    {
        $bpm = 1 * $1;
    }

    @notes = grep { $_ } split(/\s*,\s*/, $notesStr);

    #print(STDERR "[$_]\n") for (@notes);
}

help() unless (defined $name && ($#notes > 0));

# period for one beat
my $period = 60 / $bpm * 1000; # ms

# four beats are one note
my $notetime = $period * 4;

printf(STDERR "defaultOctave=%s defaultDurartion=%s bpm=%s period=%.1f notetime=%.1f\n",
       $defaultOctave, $defaultDuration, $bpm, $period, $notetime);

my @melody = ();

foreach my $noteSpec (@notes)
{
    if ($noteSpec =~ m{^(1|2|4|8|16|32|)(P|C|C#|D|D#|E|F|F#|G|G#|A|A#|B)(4|5|6|7|)(\.|)$}i)
    {
        my $duration = $1 || $defaultDuration;
        my $note     = uc($2); $note =~ s{#}{S};
        my $octave   = $3 || $defaultOctave;
        my $dotted   = $4 ? 1 : 0;

        my $time = $notetime / $duration;
        $time *= 1.5 if ($dotted);

        printf(STDERR "%-5s --> duration=%-2s note=%-2s octave=%s dotted=%s --> time=%.0f\n",
               $noteSpec, $duration, $note, $octave, $dotted, $time) if ($debug);


        push(@melody,
             "TONE_" . ($note eq 'P' ? "PAUSE" : "NOTE_$note$octave"),
             sprintf('%.0f', $time),
             #"TONE_PAUSE", sprintf('%.0f', $notetime / 64)
            );
    }
}
push(@melody, "TONE_END");
my $mName = ucfirst(lc($name));
$mName =~ s{ +}{}g;
print("//! $name melody\n");
print("static const uint16_t skMelody${mName}[] PROGMEM = \n");
print("{\n");
my $nPerLine = 10;
for (my $ix = 0; $ix <= $#melody; $ix += $nPerLine)
{
    my @n = @melody[$ix .. ($ix + $nPerLine - 1)];
    print("    " . join(' ', map { "$_," } grep { $_ } @n )   .   "\n");
    #print("    " . join(" ", map { "$_," } @melody[$ix..($ix+8)]) . "\n");
}
print("};\n");
