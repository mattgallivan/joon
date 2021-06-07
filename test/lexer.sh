#!/bin/bash

# Compare lexer output on all *.jn files in the examples/ directory with respect
# to their corresponding *.lex files.
#
# (Assumes a build directory sits at the root and contains a joon executable.)

for filepath in ../examples/*.jn; do
    lex_file="${filepath%.*}.lex"
    diff <(../build/joon $filepath) <(cat $lex_file)
done
