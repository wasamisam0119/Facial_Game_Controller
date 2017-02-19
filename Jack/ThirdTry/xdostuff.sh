#!/bin/bash

input="testpipe"

while [ "$REPLY" != "000" ]
do
read -n 3
  xdotool key --window "$(xdotool search --name 'DOSBox')" "$REPLY"
  echo $REPLY
done < "$input"
