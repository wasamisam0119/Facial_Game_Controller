#!/bin/bash

while [ "$REPLY" != "q" ]
do
    read -n 1 -r
    echo -e "$REPLY"
done
