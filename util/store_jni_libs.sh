#!/bin/sh

srcdir=$1
dstdir=$2

for l in $srcdir/*
do
    p=`basename $l`
    if [ -f $srcdir/$p/libcyclone2000.so ]
    then
	mkdir -p $dstdir/$p
	echo mv $srcdir/$p/libcyclone2000.so $dstdir/$p/
	mv $srcdir/$p/libcyclone2000.so $dstdir/$p/
	if [ -f $srcdir/$p/gdbserver ]
	then
	    echo mv $srcdir/$p/gdbserver $dstdir/$p/
	    mv $srcdir/$p/gdbserver $dstdir/$p/
	fi
	if [ -f $srcdir/$p/gdb.setup ]
	then
	    echo mv $srcdir/$p/gdb.setup $dstdir/$p/
	    mv $srcdir/$p/gdb.setup $dstdir/$p/
	fi
    fi
done
