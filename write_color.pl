#!/usr/bin/perl -w
use strict;

my $color_string = shift;
chomp $color_string;
print "Input: $color_string\n";
my ($red, $green, $blue);
if ($color_string =~ /#?(..)(..)(..)/) {
  ($red, $green, $blue) = map { pack("C",hex($_)) } ($1, $2, $3);
} else {
  die "Use 6 hex chars with an optional #";
}
open(OUT, ">/var/rgbcolor") or die "Unable to open $!";
binmode(OUT);
print OUT $red;
print OUT $green;
print OUT $blue;
close(OUT);
