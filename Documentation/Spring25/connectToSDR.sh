#!/bin/bash
SDR=192.168.10.3

ping -c1 $SDR
SUCCESS=$?

if [ $SUCCESS -eq 0 ]
then
	echo "SDR has replied, no need for configuration."
else
	echo "SDR has not replied, configuring..."
	ifconfig enp0s31f6 192.168.10.1 netmask 255.255.255.0
	
	ping -c1 $SDR
	SUCCESS=$?

	if [ $SUCCESS -eq 0 ]
	then
		echo "Ping successful; SDR connected."
	else
		echo "SDR has not replied; troubleshoot and try again."
	fi
fi
#EOF
