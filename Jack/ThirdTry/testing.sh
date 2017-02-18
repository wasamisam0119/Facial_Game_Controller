#!/bin/bash

threshold=999

char=0

mkfifo testpipe

while [ $char -lt 58 ]
do
  ((char++))
  echo "$char" > testpipe
done

echo "000" > testpipe

rm testpipe
