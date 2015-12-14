#!/bin/bash

# Compiles a MATC file in $2, with MATCC provided in $1,
# then run spim and check the output against the expected output

matcc=$1
source=$2
spim=$(which spim)
# File containing the spim pollution it outputs everytime
spim_spam=$(dirname $0)/spim_intro.txt

if [ ! -f ${spim_spam} ]
then echo Cannot open spim intro ${spim_spam}; exit 1
fi

# Compile
echo ${matcc} -o test.s ${source}
if ! ${matcc} -o test.s ${source}
then exit 1
fi

# Run the SPIM emulator
${spim} -f test.s > out.txt 2> out2.txt

# If there is a syntax error in assembler code
if grep "syntax error" out2.txt
then exit 1
fi

# Check if we need to compare output to the expected output
expected=${source%.*}.expected
if [ -f ${expected} ]
then
cat ${spim_spam} ${expected} > expected_with_spam.txt
diff out.txt expected_with_spam.txt
fi
