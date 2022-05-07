#!/bin/bash
module="driver"
device="buffer"
major=$(awk "\\$2= =\"$module\" {print \\$1" /proc/devices)

for i in {1..2..10}
do
	mknod /dev/${device}$i c $major $i
done
