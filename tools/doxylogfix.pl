#!/usr/bin/perl

use strict;
use warnings;

my @lines = grep { $_ } map { $_ =~ s/\r?\n//; $_ } <STDIN>;

for (my $ix =  $#lines; $ix > 0; $ix--)
{
    if ($lines[$ix] =~ m/^\s+/)
    {
        $lines[$ix-1] .= " $lines[$ix]";
        $lines[$ix] = undef;
    }
}


print(map { "$_\n" } grep
{
    my $l = $_;
    #print(STDERR "$l\n");
    defined $l &&
    ($l !~  m{^.+[ch]:[3456]: warning: Unsupported xml/html}) &&
    # fix (ignore warnings due to) Atmel's broken ASF documentation concept
    ($l !~ m{^3rdparty/}) &&
    ($l !~ m{^examples/}) &&
    ($l !~ m{documented symbol.*skHwPanicStr.* was not declared or defined})
} @lines);

1;
__END__
