#!/bin/bash

currdir=$(pwd -P)

xterm -e ./create.sh > pipe &
xterm -e ./input.sh < pipe
