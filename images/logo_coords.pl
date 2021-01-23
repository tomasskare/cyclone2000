#!/usr/bin/perl -w

my @lines;
my @xcoords;
my @ycoords;
my $minx = 1000000.0;
my $maxx = 0.0;
my $miny = 1000000.0;
my $maxy = 0.0;

while(<>) {
    chomp;
    if(/(\d+), +(\d+), +\d+,/) {
	push @xcoords, $1;
	push @ycoords, $2;
	$maxx = $1 if($1 > $maxx);
	$maxy = $2 if($2 > $maxy);
	$minx = $1 if($1 < $minx);
	$miny = $2 if($2 < $miny);
    } else {
	push @xcoords, 0.0;
	push @ycoords, 0.0;
	push @lines, $_;
    }
}

my $centerx = ($minx + $maxx) / 2.0;
my $centery = ($miny + $maxy) / 2.0;
my $lenx = ($maxx - $minx) / 2.0;
my $leny = ($maxy - $miny) / 2.0;

print "$centerx, $centery, $lenx, $leny\n";

my $l = 0;

for(my $i = 0 ; $i < scalar @xcoords ; $i++) {
    if($xcoords[$i] == 0.0) {
	print $lines[$l++] . "\n";
	next;
    }

    my $x = $xcoords[$i];
    my $y = $ycoords[$i];

    $x = ($x - $centerx) / $lenx;
    $y = ($centery - $y) / $leny;

    printf "  %.3f, %.3f, %.3f,\n", $x, $y, 0.0;
}
