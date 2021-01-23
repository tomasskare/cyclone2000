#!/usr/bin/perl -w

use Math::Trig;

for(my $t = 0 ; $t < 2 * pi ; $t += 2 * pi / $ARGV[0]) {

    my $x = cos($t);
    my $y = sin($t);

    printf("  %.4ff, %.4ff, %.4ff,\n", $x, $y, 0.0);
}
