#!/usr/bin/perl -w

use Data::Dumper;

my $SCALEX = $ARGV[0] || 1.0;
my $SCALEY = $ARGV[1] || 1.0;
my $SCALEZ = $ARGV[2] || 1.0;

my $numfaces;
my $numverts;
my $numcols;
my $vertices;
my $colours;
my $indices;
my $fnormals;
my $vnormals;

my $what = "";
while(<STDIN>) {
    chomp;

    if($what eq "" && /GLfloat .*vertices/) {
	$what = "vertices";
	$numverts = 0;
	next;
    }

    if($what eq "" && /GLfloat .*colours/) {
	$what = "colours";
	$numcols = 0;
	next;
    }

    if($what eq "" && /GLushort .*indices/) {
	$what = "indices";
	$numfaces = 0;
	next;
    }

    if($what ne "" && /};/) {
	$what = "";
	next;
    }

    next if($what eq "");
    s,^(.*) */[/*].*,$1,;
    next if(/^$/);

    if($what eq "vertices") {
	s/[^-0-9. ]//g;
	s/^ *//g;
	next if(/^$/);

	push @{$vertices->{$numverts}}, split(/ +/);
	$numverts++;
    } elsif($what eq "colours") {
	s/[^0-9. ]//g;
	s/^ *//g;
	next if(/^$/);

	push @{$colours->{$numcols}}, split(/ +/);
	$numcols++;
    } elsif($what eq "indices") {
	s/[^0-9 ]//g;
	s/^ *//g;
	next if(/^$/);

	push @{$indices->{$numfaces}}, split(/ +/);
	$numfaces++;
    }
}

#print "vertices:\n";
#print Dumper($vertices);
#
#print "colours:\n";
#print Dumper($colours);
#
#print "indices:\n";
#print Dumper($indices);

#print "numfaces: $numfaces\n";

for(my $v = 0 ; $v < $numverts ; $v++) {
    $vertices->{$v}->[0] *= $SCALEX;
    $vertices->{$v}->[1] *= $SCALEY;
    $vertices->{$v}->[2] *= $SCALEZ;
}

# Calculate face normals
for(my $f = 0 ; $f < $numfaces ; $f++) {
    my $i1 = $indices->{$f}->[0];
    my $i2 = $indices->{$f}->[2];
    my $i3 = $indices->{$f}->[1];

    my $ux = $vertices->{$i2}->[0] - $vertices->{$i1}->[0];
    my $uy = $vertices->{$i2}->[1] - $vertices->{$i1}->[1];
    my $uz = $vertices->{$i2}->[2] - $vertices->{$i1}->[2];
    my $vx = $vertices->{$i3}->[0] - $vertices->{$i1}->[0];
    my $vy = $vertices->{$i3}->[1] - $vertices->{$i1}->[1];
    my $vz = $vertices->{$i3}->[2] - $vertices->{$i1}->[2];

    my $nx = $uy * $vz - $uz * $vy;
    my $ny = $uz * $vx - $ux * $vz;
    my $nz = $ux * $vy - $uy * $vx;

    my $len = sqrt($nx * $nx + $ny * $ny + $nz * $nz);
    $nx /= $len;
    $ny /= $len;
    $nz /= $len;

    push @{$fnormals->{$f}}, $nx, $ny, $nz;
}

#print "fnormals:\n";
#print Dumper($fnormals);


# Calculate average vertex normals
for(my $v = 0 ; $v < $numverts ; $v++) {
    # Find faces this vertex is in.
    my $nx = 0.0;
    my $ny = 0.0;
    my $nz = 0.0;
    my $num = 0;
    for(my $f = 0 ; $f < $numfaces ; $f++) {
	next if($indices->{$f}->[0] != $v &&
		$indices->{$f}->[1] != $v &&
		$indices->{$f}->[2] != $v);

	$nx += $fnormals->{$f}->[0];
	$ny += $fnormals->{$f}->[1];
	$nz += $fnormals->{$f}->[2];
	$num++;
    }

    next if($num == 0);

    $nx /= $num;
    $ny /= $num;
    $nz /= $num;

    my $len = sqrt($nx * $nx + $ny * $ny + $nz * $nz);
    $nx /= $len;
    $ny /= $len;
    $nz /= $len;

    push @{$vnormals->{$v}}, $nx, $ny, $nz;
}

#print "vnormals:\n";
#print Dumper($vnormals);


printf("static GLfloat normals[] = {\n");
for(my $v = 0 ; $v < $numverts ; $v++) {
    printf("  %0.4ff, %0.4ff, %0.4ff,\n",
	   $vnormals->{$v}->[0],
	   $vnormals->{$v}->[1],
	   $vnormals->{$v}->[2]);
}
printf("};\n");
