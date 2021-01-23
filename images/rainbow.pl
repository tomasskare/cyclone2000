#!/usr/bin/perl -w

use Math::Trig;
use GD::Simple;

my $NUMCOORDS = $ARGV[0] || 20;
my $RADSTEP = 2 * pi / $NUMCOORDS;

my $img = GD::Simple->new(800, 100);

printf("  .line_colours = {\n");
for(my $i = 0 ; $i < $NUMCOORDS ; $i++) {
    my $r = sin($i * $RADSTEP + 0) / 2 + 0.5;
    my $g = sin($i * $RADSTEP + 2 * pi / 3) / 2 + 0.5;
    my $b = sin($i * $RADSTEP + 4 * pi / 3) / 2 + 0.5;
    $img->bgcolor($r * 255, $g * 255, $b * 255);
    $img->fgcolor($r * 255, $g * 255, $b * 255);
    $img->rectangle((800 * $i) / $NUMCOORDS, 0,
		    (800 * ($i + 1)) / $NUMCOORDS - 1, 99);
    printf("    %.3ff, %.3ff, %.3ff, %.3ff, %.3ff, %.3ff,\n",
	   $r * 0.9, $g * 0.9, $b * 0.9, $r * 0.4, $g * 0.4, $b * 0.4);
}
printf("  }\n");

#print STDERR $img->png;
