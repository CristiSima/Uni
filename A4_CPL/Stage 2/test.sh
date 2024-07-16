#!/bin/bash

TESTS_PATH=tests/tema2
EXTRA_FILES=$TESTS_PATH/main.cl

make || exit 1

echo
echo "Running tests in $TESTS_PATH"

for i in $(seq -w 1 20)
do
    if (python -X utf8 cool/Compiler.py  $EXTRA_FILES $TESTS_PATH/$i*.cl| sort | diff <(sort $TESTS_PATH/$i*.ref) - > /dev/null)
    then
        echo -e "Test $i\t OK"
    else 
        echo -e "Test $i\t FAIL"
    fi 
done

echo
make clean