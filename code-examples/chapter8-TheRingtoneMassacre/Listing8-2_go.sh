#!/bin/bash

# file size
filesize=415959

# file offset
off=0

# number of files
num=4

# fuzz value
val=255

# name counter
cnt=0

while [ $cnt -lt $num ]
do
	cp ./Alarm.m4r ./file$cnt.m4a
	./fuzz $filesize $off $val ./file$cnt.m4a
	let "off+=1"
	let "cnt+=1"
done