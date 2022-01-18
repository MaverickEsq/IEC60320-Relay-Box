#!/bin/bash

usage() { 
	echo "Usage: $0 [-t10s] <node ID>" 1>&2;
	echo "Controls for IEC60320 relay boxes" 1>&2;
    echo "  -t 	toggle" 1>&2;
    echo "  -1	set to on" 1>&2;
    echo "  -0 	set to off" 1>&2;
    echo "  -s 	return status (0 off, 1 on)" 1>&2;
    echo 1>&2; 
    exit 1;
}

let nodeid="default";

if [ $# -gt 0 ]; then
    nodeid="${@: -1}";
else
	usage;
fi

getStatus() {
	status=`send status`;
	echo "The relay is currently: ${status}";
	case ${status} in
		on) return 1 ;;
		off) return 0 ;;
		*) exit -1 ;;
	esac
}

send() {
	echo $(curl -s --interface wlan0 "$nodeid/$1");
}

while getopts ":t10s" o; do
	case "${o}" in
		t) send toggle ;;
		1) send close ;;
		0) send open ;;
		s) getStatus ;;
		?) usage ;;
	esac
done

shift $((OPTIND-1))

if [ -z $@ ]; then usage; fi;

if [ $OPTIND -eq 1 ]; then
	getStatus; 
fi
