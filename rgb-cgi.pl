#!/usr/bin/perl -w
use strict;
use CGI;

my $q = CGI->new();
print $q->header();

my $color_string = $q->param('color');
my ($red, $green, $blue);
if ($color_string =~ /^(..)(..)(..)$/) {
  ($red, $green, $blue) = map { pack("C",hex($_)) } ($1, $2, $3);
  open(OUT, ">/var/rgbcolor") or die "Unable to open $!";
  binmode(OUT);
  print OUT $red;
  print OUT $green;
  print OUT $blue;
  close(OUT);
  print $q->start_html('OK');
  print $q->h1('OK');
} else {
  print $q->start_html('ERROR');
  print $q->h1('ERROR');
  print 'Use 6 hex chars';
}

print $q->end_html();

