#!/bin/sh
# pass in the programs as parameters
fail_count=0
total_test_program=0

for prog in $1
do
	echo "execute $prog"
	total_test_program=`expr $total_test_program + 1`
	eval "./$prog"
	if [ $? -ne 0 ]; then
		fail_count=`expr $fail_count + 1`
	fi
done

echo "Note: 1 test program may contains more than 1 test case."
echo "$total_test_program program(s) run:"
echo "$fail_count program(s) fail. "