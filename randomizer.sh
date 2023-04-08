#!/bin/bash

# ARG COUNT CHECK - NUID AND EMAIL
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <nuid> <@northeastern email id>"
    exit 1
fi
# CHECK NUID PATTERN
if [[ ! $1 =~ ^00[0-9]{7}$ ]]; then
    echo "Invalid NUID. Expected 9 digits starting with two zeros e.g. 002979108."
    exit 1
fi
# CHECK NORTHEASTERN EMAIL PATTERN - assuming all email are lowercase otherwise  $2 =~ ^[a-zA-Z]+(\.[a-zA-Z]+[0-9]*)\@(northeastern|neu)\.edu$
if [[ ! $2 =~ ^[a-z]+(\.[a-z]+[0-9]*)\@(northeastern|neu)\.edu$ ]]; then
    echo "Invalid northeastern email."
    exit 1
fi


# GIVEN CODE HERE
nuid=$((10#$1))
sc1=$(( $nuid % 19))
echo -e "Thanks, your email is: $2, \nand your 9 programs to work with are:"
for (( c=1; c<10; c++ ))
do
        increment=$(( $c))
        code=$(($(($sc1 + $increment)) % 19))
        code=$(printf %02d $code)
        echo "$c. codeN11$code.c"
done

sc2=$(( $nuid % 3))
echo -e "Your Part 4 Program is:"
if [[ $sc2 -eq 0 ]]
then
echo "Buffer Overflow"
fi
if [[ $sc2 -eq 1 ]]
then
echo "Undefined Behavior"
fi
if [[ $sc2 -eq 2 ]]
then
echo "Memory Leak"
fi
