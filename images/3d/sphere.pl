#!/usr/bin/perl -w

use Math::Trig;

for(my $t = -1.1 ; $t < 1.1 ; $t += 0.005) {

    my $at = $t;

    $at = -0.990 if($at < -0.990);
    $at = 0.990 if($at > 0.990);

    my $a = sin(pi * ($at + 1.0) / 2) ** 0.4;

    my $x = $a * cos(14 * pi * $t);
    my $y = $at;
    my $z = $a * sin(14 * pi * $t);

    printf("  %.4ff, %.4ff, %.4ff,\n", $x, $y, $z);
#    printf("v %.4f %.4f %.4f\n", $x, $y, $z);
}
