#!/bin/bash
L=$(find CODE -type f)
for l in $L
do
	echo -n "-"
done
echo
for l in $L
do
	echo -n "#"
	#echo "$l"
	cp "$l" temp
	inR="Release$(echo $l | grep -o "/.*")"
	#echo $inR
	(echo $l | grep "\.c$" > /dev/null) && (mv temp temp.c && ./PCfi.c.sh && rm temp.c)
	(echo $l | grep "\.h$" > /dev/null) && (mv temp temp.h && ./PCfi.h.sh && rm temp.h)
	mkdir $(echo $inR | grep -o ".*/.*/") 2>/dev/null
	#mkdir -p $inR && 
	cp temp $inR
done
echo
