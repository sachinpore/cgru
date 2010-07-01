#!/bin/bash

prefix=$PWD

CPPFLAGS=" -I$prefix/include"

LDFLAGS="-L/lib64 -L/usr/lib64 -L$prefix/lib -lpthread"

export CPPFLAGS
export LDFLAGS

cd openexr

if [ ! -z $1 ] ; then
   ./configure -h; exit
fi

./configure --disable-ilmbasetest --prefix=$prefix --exec-prefix=$prefix --enable-shared=

make

make install