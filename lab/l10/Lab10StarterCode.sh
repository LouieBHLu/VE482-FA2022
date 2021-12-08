#!/bin/bash

DBUSCMD=dbus-monitor
DBUSOPTS=--system
MODULE="dicedevice"
DEVICE="dice"

cleanup(){
	# Remove the module dicedevice
    rmmod $MODULE
    rm -f /dev/${DEVICE} /dev/${DEVICE}[0-2]
}

welcome(){
    insmod /lib/module/$MODULE.ko gen_sides=200
    rm -f /dev/${DEVICE}[0-2]
    major=$(awk "/${DEVICE}/ {print $1}" /proc/devices)

    mknod /dev/${DEVICE}0 c $major 0
    mknod /dev/${DEVICE}1 c $major 1
    mknod /dev/${DEVICE}2 c $major 2

    # change group to staff and privilege to 664: can read, write; cannot execute
    GROUP="staff"
    chgrp $GROUP /dev/${DEVICE}[0-2]
    chmod 664 /dev/${DEVICE}[0-2]
}

$DBUSCMD $DBUSOPTS | while read line; do
	connected=$(echo $line | awk {print $7})
	case "$connected" in
		"mum")
			cleanup;
			;;
		"grandpa")
			welcome;
			;;
	esac
done