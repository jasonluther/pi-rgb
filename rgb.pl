#!/usr/bin/perl -w
use strict;
use warnings;
use Data::Dumper;

my $red_pin = 23;
my $green_pin = 24;
my $blue_pin = 25;
my @pins = ($red_pin, $green_pin, $blue_pin);

my %fh = ();
foreach my $pin (@pins) {
  `echo $pin > /sys/class/gpio/export`;
  `echo "out" > /sys/class/gpio/gpio$pin/direction`;
  `echo "0" > /sys/class/gpio/gpio$pin/value`;
  open($fh{$pin}, ">/sys/class/gpio/gpio$pin/value") or die;
}

my %write_pin_cache;
sub write_pin_value {
  my($pin, $value) = @_;
  #`echo "$value" > /sys/class/gpio/gpio$pin/value`;
  my $prev_value = $write_pin_cache{$pin};
  $write_pin_cache{$pin} = $value;
  return if $value == $prev_value;
  my $fh;
  open($fh, ">/sys/class/gpio/gpio$pin/value") or die;
  print { $fh } "$value\n";
  close($fh);
}

my %pin_value;

sub get_color {
  open(COLOR, "< /var/rgbcolor") or die;
  my $color = <COLOR>; chomp $color;
  print "Color is $color\n";
  close(COLOR);
  my ($red, $green, $blue);
  if ($color =~ /#?(..)(..)(..)/) {
    ($red, $green, $blue) = map { int(hex($_)/2) } ($1, $2, $3);
    $pin_value{$red_pin} = $red;
    $pin_value{$green_pin} = $green;
    $pin_value{$blue_pin} = $blue;
    #print "$red $green $blue\n";
  }
}

get_color();
my $check_for_color = 0;
while (1) {
  if (!$check_for_color) {
    get_color();
    $check_for_color = 100;
  }
  $check_for_color--;

  my $tick = 0;
  while ($tick < 127) {
    foreach my $pin (@pins) {
      my $value = $pin_value{$pin};
unless (defined $value) {
  print "Pin: $pin\n";
  print Data::Dumper::Dumper \%pin_value;
}
      if ($tick < $value) {
        write_pin_value($pin, 1);
      } else {
#print "Turning $pin off\n" if $pin == 23;
        write_pin_value($pin, 0);
      }
    }
    $tick++;
  }
}

