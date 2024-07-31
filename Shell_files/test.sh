#!/bin/bash

CONFIG_FILE="config.xml"

PATH_TO_THIS="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PATH_TO_LIB_ROOT="$(dirname ${PATH_TO_THIS})"
PATH_TO_TEST_DEPS="$( xmlstarlet sel -t -v "config/Tests/Dependencies/@Dest" ${CONFIG_FILE})"
PATH_TO_TEST_DEP_DYN_LIBS=${PATH_TO_LIB_ROOT}/${PATH_TO_TEST_DEPS}/Dynamic_libraries

export LD_LIBRARY_PATH=${PATH_TO_TEST_DEP_DYN_LIBS}

echo
echo "*******************************"
echo "Testing 'main' executable file."
echo "*******************************"
# ./Tests/Executable_files/main -a -b 2 -c e -d 1.3
./Tests/Executable_files/main --verbose -r 2.5 -s -c abc --TestParse5 +A.b_C·x#Y!z^