#!/bin/bash


while [ "$REPLY" != "q" ]
do
    read -n 1
    if [ "$REPLY" != \n ]; then
        if [ "$REPLY" == "w" ]; then
            xdotool key --window "$(xdotool search --name 'DOSBox')" Up
        elif [ "$REPLY" == "a" ] ; then
            xdotool key --window "$(xdotool search --name 'DOSBox')" Left
        elif [ "$REPLY" == "s" ]; then
            xdotool key --window "$(xdotool search --name 'DOSBox')" Down
        elif [ "$REPLY" == "d" ]; then
            xdotool key --window "$(xdotool search --name 'DOSBox')" Right
        elif [ "$REPLY" == " " ]; then
            xdotool key --window "$(xdotool search --name 'DOSBox')" space
        else
            echo "$REPLY"
        fi
    fi
done
