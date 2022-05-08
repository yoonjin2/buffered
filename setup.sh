#!/bin/bash
module="driver"
major="14132"
device="buffer"

for i in {0..1..10}
do
	mknod ${device}$i c $major $i
done
