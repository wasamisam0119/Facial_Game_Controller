#!/usr/bin/env python

from pymouse import PyMouse
from pykeyboard import PyKeyboard
import time

m = PyMouse()
k = PyKeyboard()
i = 0

fifo = open("testpipe")

while 1:
  line=fifo.read()
  if len(line) != 0:
    if line == '000\n':
      break
    else:
      time.sleep(5)
      k.press_keys(['w','a','s','d'])
      print line
