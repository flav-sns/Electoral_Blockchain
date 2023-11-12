#!/usr/local/bin/bash

>INSTRUCTIONS

for i in {100..10000..25}
do
        echo -e "4\n97 $i 117" >> INSTRUCTIONS
done
echo 0 >> INSTRUCTIONS
