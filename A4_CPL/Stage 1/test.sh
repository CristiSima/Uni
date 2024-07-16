#!/bin/bash

make

for i in $(seq -w 1 20)
do
    if (python -X utf8 cool/Compiler.py ./tests/tema1/$i*.cl | diff tests/tema1/$i*.ref - > /dev/null)
    then
        echo -e "Test $i\t OK"
    else 
        echo -e "Test $i\t FAIL"
    fi 
done

make clean