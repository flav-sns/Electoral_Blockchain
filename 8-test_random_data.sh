#!/usr/local/bin/bash

>INSTRUCTIONS

for i in {50..1000..50}
do
        echo -e "8\n$i 10" >> INSTRUCTIONS
done
echo 0 >> INSTRUCTIONS
