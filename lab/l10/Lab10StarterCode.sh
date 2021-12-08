#!/bin/sh

DBUSCMD=dbus-monitor
DBUSOPTS=--system

cleanup(){
	#TODO: finish this function
}

welcome(){
	#TODO: finish this function
}

$DBUSCMD $DBUSOPTS | while read line; do
	# connected=
	# TODO: find out who connected
	case "$connected" in
		"mum")
			cleanup;
			;;
		"grandpa")
			welcome;
			;;
	esac
done