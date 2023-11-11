#!/bin/sh

export LD_LIBRARY_PATH=~/Desktop/scripts/C/C_Arg_Parse/Tests/Dependency_files/Dynamic_libraries

echo
echo "*******************************"
echo "Testing 'main' executable file."
echo "*******************************"
# ./Tests/Executable_files/main -a -b 2 -c e -d 1.3
./Tests/Executable_files/main --verbose -r 2.5 -s -c abc