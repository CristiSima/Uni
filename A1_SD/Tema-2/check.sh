#/bin/bash

cp *.c Checker/
cp *.h Checker/
cp Makefile Checker/
cp README.md Checker/

cd Checker

# clang-format --style=google -i *.c *.h
# clang-format -style="{SpaceBeforeAssignmentOperators: true}" -i *.c *.h

./SimasCFormater.py *.c *.h


if [ "$1" == "pack" ]; then
	make pack
	mv *.zip ../
	exit
fi

if [ "$1" != "" ]; then
	./cpplint.py --root=$(pwd) --filter=-build/include,-runtime/printf,-runtime/threadsafe_fn,-build/include_what_you_use,-runtime/int $1 $2 $3 $4 $5 $6 $7 $8 $9
else
	./check.sh
fi
