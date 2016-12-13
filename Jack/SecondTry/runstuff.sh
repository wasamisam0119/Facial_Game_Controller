#!/bin/bash

gcc create.c -o create.run

gcc run.c -o run.run

open -a Terminal.app -e ./create.run & open -a Terminal.app -e ./run.run
