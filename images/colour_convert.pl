#!/usr/bin/perl -w

while(<>) {
    if(/^([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})  .*$/) {
	$r=hex($1) / 255.0;
	$g=hex($2) / 255.0;
	$b=hex($3) / 255.0;

	printf "  %.3f, %.3f, %.3f, %.3f,\n",
	  $r, $g, $b, 1.0;
    } else {
	print;
    }
}
