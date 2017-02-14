#!/usr/bin/env python

pipe = open('/dev/input/js0', 'r')

msg = []

while 1:
  for char in pipe.read(1):
    msg += [ord(char)]
    if len(msg) == 8:
      print msg
      msg = []
