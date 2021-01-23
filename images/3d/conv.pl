#!/usr/bin/perl -w

my %vertices;
my @indices;
my $maxabsx = 0;
my $maxabsy = 0;
my $maxabsz = 0;
my $maxz = 0;
my $vid = 0;

while(<>) {
    chomp;
    @vs = split(/ /);
    foreach my $v (@vs) {
	my ($x,$y,$z) = split(/,/, $v);
	$maxabsx = abs($x) if(abs($x) > $maxabsx);
	$maxabsy = abs($y) if(abs($y) > $maxabsy);
	$maxabsz = abs($z) if(abs($z) > $maxabsz);
	$maxz = $z if($z > $maxz);

	if(!defined($vertices{$v})) {
	    $vertices{$v} = $vid++;
	}
	push @indices, $vertices{$v};
    }
}

print "static GLfloat vertices[] = {\n";
foreach my $v (sort {$vertices{$a} <=> $vertices{$b}} keys %vertices) {
    my ($x,$y,$z) = split(/,/, $v);

    if($maxabsx == 0) {
	$x = 0;
    } else {
	$x /= $maxabsx;
    }
    if($maxabsy == 0) {
	$y = 0;
    } else {
	$y /= $maxabsy;
    }
    $z -= $maxz;
    if($maxabsz + $maxz == 0) {
	$z = 0;
    } else {
	$z /= ($maxabsz + $maxz);
    }

    printf("    %.3ff * SCALEX, %.3ff * SCALEY, %.3ff * SCALEZ,\n", $x, $y, $z);
}
print "};\n";
print "static const int num_vertices = " . (scalar keys %vertices) . ";\n\n";

print "static GLushort indices[] = {\n";
for($i = 0 ; $i < scalar @indices ; $i += 3) {
    # Clockwise rotation
    print "    $indices[$i+0], $indices[$i+2], $indices[$i+1],\n";
}
print "};\n";
print "static const int num_indices = " . (scalar @indices) / 3 . " * 3;\n";

