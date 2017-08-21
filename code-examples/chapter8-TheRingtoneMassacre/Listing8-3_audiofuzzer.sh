#!/bin/bash

fuzzhost=192.168.99.103

echo [+] =================================
echo [+] Start fuzzing
echo [+]
echo -n "[+] Cleanup: "
killall MobileSafari
killall mediaserverd
sleep 5
echo

origpid=`ps -u mobile -o pid,command | grep /usr/sbin/mediaserverd | cut -c 0-5`
echo [+] Original PID of /usr/sbin/mediaserverd: $origpid

currpid=$origpid
let cnt=0
let i=0

while [ $cnt -le 1000 ];
do
	if [ $i -eq 10 ];
	then
		echo -n "[+] Restarting mediaserverd.. "
		killall mediaserverd
		sleep 4
		origpid=`ps -u mobile -o pid,command | grep /usr/sbin/mediaserverd | cut -c 0-5`
		currpid=$origpid
		sleep 10
		echo "done"
		echo [+] New mediaserverd PID: $origpid
		i=0
	fi
	echo
	echo [+] =================================
	echo [+] Current file: http://$fuzzhost/file$cnt.m4a
	openURL http://$fuzzhost/file$cnt.m4a
	sleep 30
	currpid=`ps -u mobile -o pid,command | grep /usr/sbin/mediaserverd | cut -c 0-5`
	echo [+] Current PID of /usr/sbin/mediaserverd: $currpid
	if [ $currpid -ne $origpid ];
	then
		echo [+] POTENTIAL BUG FOUND! File: file$cnt.m4a
		openURL http://$fuzzhost/BUG_FOUND_file$cnt.m4a
		origpid=$currpid
		sleep 5
	fi
	((cnt++))
	((i++))
	killall MobileSafari
 done

killall MobileSafari