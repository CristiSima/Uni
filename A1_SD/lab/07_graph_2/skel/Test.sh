#!/bin/bash

echo
echo "		Using examples from OCW"
echo
# run_mod="valgrind --leak-check=full"
$run_mod ./Test << EOF
12 10
0 1
0 2
1 2
2 3
4 5
4 6
5 6
4 7
7 8
9 10

7 10
0 1
0 4
1 2
1 3
1 4
2 4
3 5
3 6
4 5
4 6
0 6

9 8
0 1
0 6
1 2
2 7
3 6
4 7
4 8
5 8

5 7
0 1
1 2
0 3
1 3
1 4
2 4
3 4


EOF
