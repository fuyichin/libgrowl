#!/bin/sh
tmp_libgrowl=`mktemp`
if [ -f $tmp_libgrowl ]; then
  rm -f $tmp_libgrowl
fi
cd .. # to <project>
make; make prefix=$tmp_libgrowl/usr install
cd -
result=`find $tmp_libgrowl -name GrowlServerHack`
if [ -z $result ]; then
  exit 0
else
  exit 1
fi