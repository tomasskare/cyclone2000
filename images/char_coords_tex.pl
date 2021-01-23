#!/usr/bin/perl -w

my $w = 1024;
my $h = 1024;

while(<>) {
    chomp;

    if(/^(.*): (\d+), (\d+), (\d+), (\d+)$/) {
	my $x1 = $2 / $w;
	my $y1 = ($3 + $5) / $h;
	my $x2 = ($2 + $4) / $w;
	my $y2 = $3 / $h;
	my $tw = $4 / 100.0;
	my $th = $5 / 100.0;
	printf("  { %.3f, %.3f, %.3f, %.3f, %.3f, %.3f },     // %s\n",
	       $x1, $y1, $x2, $y2, $tw, $th, $1);
    }
}
