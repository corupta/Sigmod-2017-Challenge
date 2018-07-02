#!/bin/bash
FILE="src/superNGramSolverButCurrentlyLinear"

if [ ! -f "$FILE" ]
then
    ./compile.sh
fi
./src/superNGramSolverButCurrentlyLinear
