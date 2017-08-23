#!/bin/bash

echo "==========$(date)==========="

set $(date) #以最后一次执行为准，添加参数
for i in $@; do
	echo -e "$i\n"
done

set $(ls /)
while true; do
	echo "$1" && shift && [ -s $1 ] && exit 0
done
