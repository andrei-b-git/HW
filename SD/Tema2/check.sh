#!/bin/bash

EXE=skiplist
TOTAL=0

for i in $(seq -w 1 9); do
	./$EXE "tests/test0$i.in"
	diff -wiB "test0$i.out" "tests/test0$i.out_ref" > __stdout
	if [ $? != 0 ]; then
		echo "Test $i "
		cat __stdout | head -n 20
		echo -e "File truncated!               ... test failed!\n"
	else
		echo "Test $i                       ... OK[10/90]"
		TOTAL=$(expr $TOTAL + 10)
	fi
done

echo "----------------------------------------------"
echo "TOTAL: $TOTAL/90 puncte"

rm -f test0*.out
