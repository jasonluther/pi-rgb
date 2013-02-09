#!/usr/bin/perl -w
use strict;

my $color_string = shift;
chomp $color_string;
print "Input: $color_string\n";
my ($red, $green, $blue);
if ($color_string =~ /#?(..)(..)(..)/) {
  ($red, $green, $blue) = map { hex($_) } ($1, $2, $3);
} else {
  die "You didn't provide a format that satisfied me.";
}

unlink("/var/rgbcolor");
open(OUT, ">/var/rgbcolor") or die "Unable to open $!";
binmode(OUT);
print "Red: $red\n";
print "Green: $green\n";
print "Blue: $blue\n";
print OUT $red;
print OUT $green;
print OUT $blue;
close(OUT);

print `cat /var/rgbcolor`;
