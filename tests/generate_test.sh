#!/bin/bash

FILE1=test1.txt
FILE2=test2.txt

echo "TRUNCATE A" > $FILE1
echo "TRUNCATE B" > $FILE2

for (( i = 0; i < 100; i++ )); do
	echo "INSERT A" $i "AAA"$i >> $FILE1
done

for (( i = 80; i < 180; i++ )); do
	echo "INSERT B" $i "BBB"$i >> $FILE2
done

echo "INTERSECTION" >> $FILE1
echo "SYMMETRIC_DIFFERENCE" >> $FILE2

echo "TRUNCATE A" >> $FILE1
echo "TRUNCATE B" >> $FILE2
