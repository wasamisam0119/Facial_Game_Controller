#!/bin/bash

xdotool key --window "$(xdotool search --name 'DOSBox')" Down

while [ "$REPLY" != "q" ]
do
    read -n 1
    if [ "$REPLY" != \n ]; then
        if [ "$REPLY" == "s" ]; then
            echo "St Helens"
        elif [ "$REPLY" == "h" ] ; then
            echo "Hull KR"
        elif [ "$REPLY" == "c" ]; then
            echo "Castleford"
        else
            echo "$REPLY"
        fi
    fi
done
