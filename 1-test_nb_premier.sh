#!/usr/local/bin/bash

#Ce script est destiné à être utilisé après avoir lancé le programme avec le script 5 : cela permet de tester la primalité avec is_prime_naïve des nombres qui ont été générés par random_prime_number (qui utilise le test de primalité de Miller-Rabin).

>INSTRUCTIONS

while read line
do
        echo -e "1\n$line" >> INSTRUCTIONS
done < <(sed '1d' output.csv | cut -d ";" -f 1)
echo 0 >> INSTRUCTIONS
