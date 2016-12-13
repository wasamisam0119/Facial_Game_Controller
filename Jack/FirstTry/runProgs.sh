#!/bin/bash
gcc writer.c -o writer.run

gcc reader.c -o reader.run

open -a Terminal.app -e ./writer.run & open -a Terminal.app -e ./reader.run
