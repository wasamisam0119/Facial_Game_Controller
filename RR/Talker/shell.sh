#!/bin/bash

input="testpipe"
echo "Im in"
while [ "$REPLY" != "000" ]
do
read -n 3
	if test -n "$REPLY"; then
		echo $REPLY
	fi
done < "$input"
echo "Fin"
