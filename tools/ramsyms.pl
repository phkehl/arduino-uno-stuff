#!/usr/bin/perl
################################################################################
#
# show ram symbols
#
# Usage: objdump -t img.elf | ramsyms
#
# Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>
#
################################################################################

use strict;
use warnings;

my @symbols = ();
my $have20000000 = 0;
my $have00800000 = 0;
while (<STDIN>)
{
    if (m/^(([02][0-9a-f]+).*(\.bss|\.data|\.noinit|\.stack|\.text|\.relocate)\s+([0-9a-f]+)\s+(.*))\r*\n*$/i)
    {
        my ($line, $addr, $sec, $size, $sym) = ($1, hex($2), $3, hex($4), $5);
        #printf("%x $addr / $sec / $size / $sym\n", $addr);
        $have20000000++ if ($addr >= 0x2000000);
        $have00800000++ if (($addr >= 0x0080000) && ($addr < 0x2000000));
        if ($size)
        {
            push(@symbols, { addr => $addr, sec => $sec, size => $size, sym => $sym });
        }
    }
}

if ($have00800000)
{
    @symbols = grep { ($_->{addr} >= 0x0080000) } @symbols;
}
if ($have20000000)
{
    @symbols = grep { ($_->{addr} >= 0x2000000) } @symbols;
}

my $totSize = 0;
$totSize += $_->{size} for (@symbols);

print("***** symbols by address *****\n");
my $prevS = undef;
foreach my $s (sort { $a->{addr} <=> $b->{addr} } @symbols)
{
    my $comment = '';
    if ($prevS)
    {
        if ($s->{addr} > ($prevS->{addr} + $prevS->{size}))
        {
            my $padding = $s->{addr} - ($prevS->{addr} + $prevS->{size});
            printf("0x%08x  %05u  0x%04x  _padding_\n", $s->{addr} - $padding, $padding, $padding);
            #$comment = "padded " . ();
        }
        elsif ($s->{addr} < ($prevS->{addr} + $prevS->{size}))
        {
            $comment = "overlap";
        }
    }

    printf("0x%08x  %05u  0x%04x  %-12s  %-35s %s\n",
           $s->{addr}, $s->{size}, $s->{size}, $s->{sec}, $s->{sym}, $comment);
    $prevS = $s;
}
printf("           %05u 0x%04x\n", $totSize, $totSize);

print("***** symbols by size *****\n");
foreach my $s (sort { $b->{size} <=> $a->{size} or $a->{sym} cmp $b->{sym} } @symbols)
{
    printf("%05u  0x%04x  %-12s  %s\n", $s->{size}, $s->{size}, $s->{sec}, $s->{sym});
}
printf("%05u 0x%04x\n", $totSize, $totSize);


print("***** size by section *****\n");
my %sections = ();
foreach my $s (@symbols)
{
    $sections{$s->{sec}} += $s->{size};
}
foreach my $s (sort { $sections{$a} <=> $sections{$b} } keys %sections)
{
    printf("%05u 0x%05x %s\n", $sections{$s}, $sections{$s}, $s);
}

################################################################################
1;
__END__
