#!/bin/bash

createout="/proc/$(pidof create.run)/fd/0"
createoutlinked=$(readlink $createout)
runin="/proc/$(pidof run.run)/fd/0"
runinlinked=$(readlink $runin)
stdout="/dev/stdout"

while [ 1 ]; do
    tail $createout
    #output=$(cat $createout)
    #echo $output
    #echo $createout >> $runin
done

#while [ 1 ]; do
    #echo $stdout
#done
