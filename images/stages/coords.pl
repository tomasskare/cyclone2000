#!/usr/bin/perl -w

my $filename = $ARGV[0];
my $stage = $filename;
$stage =~ s/\..*$//;

my $stagetxt = sprintf("%04d", $stage);

my @xcoords;
my @ycoords;
my @convex;
my $haveconvex = 0;
my $num = 0;
my $xmin = 20000.0;
my $xmax = 0.0;
my $ymin = 20000.0;
my $ymax = 0.0;

open(my $fh, "<$filename");

while(<$fh>) {
    chomp;

    if(/^(\d+), (\d+)(, (\d))?$/) {
	my $x = $1;
	my $y = $2;
	my $c = defined($3) ? $4 : 0;

	$xmin = $x if($x < $xmin);
	$xmax = $x if($x > $xmax);
	$ymin = $y if($y < $ymin);
	$ymax = $y if($y > $ymax);

	push @xcoords, $x;
	push @ycoords, $y;
	push @convex, $c;
	$haveconvex = 1 if($c);

	$num++;
    }
}

print <<__END__;
#ifndef _STAGE_${stagetxt}_H_
#define _STAGE_${stagetxt}_H_

#undef XSCALE
#undef YSCALE
#undef XOFF
#undef YOFF

#define XSCALE 1.000
#define YSCALE 1.000
#define XOFF   0.000
#define YOFF   0.000

static stage_t stage_${stagetxt} __attribute__((unused)) = {
  .closed = 0,
__END__

printf("  .num_coords = %d,\n", $num);
printf("  .coords = {\n");

my $xcenter = ($xmin + $xmax) / 2.0;
my $ycenter = ($ymin + $ymax) / 2.0;
my $xlen = ($xmax - $xmin) / 2.0;
my $ylen = ($ymax - $ymin) / 2.0;

for(my $i = 0 ; $i < $num ; $i++) {
    my $x = $xcoords[$i];
    my $y = $ycoords[$i];

    $x = ($x - $xcenter) / $xlen;
    $y = ($ycenter - $y) / $ylen;

    printf("    %.4f * XSCALE + XOFF, %.4f * YSCALE + YOFF,\n", $x, $y);
}

printf("  },\n");

if($haveconvex) {
    printf("  .convex = {\n");
    for(my $i = 0 ; $i < $num ; $i++) {
	if($convex[$i]) {
	    printf("    [%d] = 1,\n", $i);
	}
    }
    printf("  },\n");
}

print <<__END__;
  .move_colours = 1,
  .colours = {
    0.000, 0.150, 0.550,
    0.350, 0.100, 0.450,
    0.000, 0.150, 0.550,
  },
  .line_colours = {0.15f, 0.15f, 0.65f, 0.05f, 0.05f, 0.40f},
  .start_position = 7,

  .mutant_tanker_rnd = 10,
  .mutant_flipper_rnd = 200,
  .spiker_num = 1,
  .spiker_delay = 2500,

  .stage_length = 35000,
  .foes = {
    {
      .type = STAGE_E_FOE_FLIPPER,
      .initial_entry = 0,
      .jitter = 200,
      .count = 24,
    },
    {
      .type = STAGE_E_FOE_TANKER_FLIPPER,
      .initial_entry = 1500,
      .jitter = 200,
      .count = 10,
    },
    {
      .type = STAGE_E_END,
    }
  }
};

#endif
__END__


close($fh);
