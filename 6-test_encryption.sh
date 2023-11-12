#!/usr/local/bin/bash

>INSTRUCTIONS

for i in {1..1000}
do
        echo "6" >> INSTRUCTIONS
	for ((j=0;j<$i;j++))
	do
		echo -n "a" >> INSTRUCTIONS
	done
	echo >> INSTRUCTIONS
done
echo 0 >> INSTRUCTIONS

