#!/bin/bash

input="dummy.txt"

while [ "$REPLY" != "000" ]
do
  read -n 3
  case "line" in
    '1') thingtodo="O+N+E"
      ;;
    '2') thingtodo="T+W+O"
      ;;
    '3') thingtodo="T+H+R+E+E"
      ;;
    '4') thingtodo="F+O+U+R"
      ;;
    *) thingtodo="$line"
      ;;
  esac
  xdotool key --window "$(xdotool search --name 'DOSBox')" $thingtodo
  echo $thingtodo
done < "$input"

