#!/bin/sh
# pass in the programs as parameters
fail_count=0

echo $1
for prog in $1
do
	eval "./$prog"
	if [ $? -ne 0 ]; then
		fail_count=`expr $fail_count + 1`
	fi
done

echo "$fail_count program(s) fail"