# Notes

## 1 - FIFOs
I've been investigating the best ways to take the output of one program and feed it in as the input of another.
This, it seems, is best done by something called a FIFO.
This is something in C that approximates a file without actually creating one.
You create one using the `mkfifo` command, and you can then interact with it exactly as you would a file.
There's some race condition stuff that you can get around using a `usleep` (in `unistd.h`), we'd probably use that.

## 2 - DOSBox
DOSBox is a free, open-source piece of software that allows you to emulate old DOS games.
It's what I propose we use as the basis of the initial version of our software, as it can be run on Linux, which is a fairly easy programming environment.
Being open-source, and written largely in C++, it should be simple to find the location of the I/O handler and extend it to take input from an external program.
We'll test this by having a program write to a FIFO a set of pre-programmed instructions, and if we've done it right, the DOSBox environment should do what we expect.
This is likely just making a directory, although we could have it play pac-man. Long-term, the work done on the facial recognition will get to a point where they can pipe the output to a FIFO, which will then be read into the DOSBox, and used to control a game in real-time.

## 3 - Piping `/proc`
Edgar has found a tool in Linux that allows us to pipe output without buffering
    and without modifying the, frankly, labyrinthine source code of DOSBox's input handling file
    and using processes that are already running
The idea is an extension of our reasoning for using Linux in the first place, which is that in UNIX, all things are treated as files
Hence, using Linux/bash tools, we can redirect the output of one file to the input of another, via the `/proc` folder
Unfortunately, on the University provided Linux servers (`avon`, `bann`, `clyde`, etc.), we can't access `/proc`
    This is likely for a good reason, but annoying nonetheless, as it would make our lives a whole lot easier

## 4 - We Control The Controller
I found during my research into piping `/proc` that redirecting the output of a running process is difficult, assuming the process has been started 'normally'.
I also found that redirecting something to a process' input is almost straightforward.
This is because `/proc/<process id>/fd/{0,1,2}` all symlink to the same place on a Linux system, namely `/dev/pts/n`, where n is just an integer representing which terminal window is running the process.
Therefore, taking from one is difficult, as then you get a jumbled mess of `stdin`, `stdout`, and `stderr`.
Putting into one, as in redirecting the output of a file to `/proc/<process id>/fd/0`, is easy by comparison, especially in Linux.
It still requires admin privileges, but it's at least a start.

## 4.5 - Control Testing
In testing, this approach almost works. The output of the `create` program does appear in the terminal window of the `run` program, however `run` does not respond.
This is likely to do again with the fact that in a standard bash process, `stdin`, `stdout`, and `stderr` all symlink to the same location.
Eventually, the thought occurred that using C for this testing was unnecessary, as the testing is to ascertain how well I/O redirection can be used.
Testing `.sh` files reveals that the use of `echo` statements allows for really simple redirection, so why it doesn't work as well in C is beyond me; the putchar() function's supposed to do the same thing as echo, isn't it?

## 5 - Integrating into DOSbox
There is, it turns out, a Linux utility called `xdotool`. This is a tool that lets you emulate keyboard and mouse input in X11, and is essentially a wrapper for built-in X11 tools.
`xdotool` has an extensive man page, which will require a lot of reading.

## 6 - xdotool to the rescue
xdotool works well for pacman, via a while loop that substitutes `w` for `up`, `s` for `down`, etc.
The performance seems snappy, but it is only pacman, more intensive games may require optimisation somehow.

## 7 - `keyboard.cpp`
Ed's found in the DOSbox folder a file called `keyboard.cpp`.
This file, we believe, emulates a keyboard (hence the name).
There's a function called `KEYBOARD_AddKey()` that apparently takes a keyboard key and returns an `int`.
We believe we can take this and extend it to return the same `int`s as certain keys based on face controller input.
The tricky bit is finding where this function is called and modifying that section of the DOSbox source code to take input from somewhere else.

## 7.5 - `KEYBOARD_AddBuffer(Bit8u data)`
`KEYBOARD_AddKey` finishes by calling the function `KEYBOARD_AddBuffer`.
Finding this is huge; it means a couple of things.
First, hopefully what it means is that we can just add a `.cpp` file which `#include`s the requisite include files, make a while loop that repeatedly calls `KEYBOARD_AddBuffer` with the correct values, and we can make the input of that whatever we want.
Second, and almost more importantly, it shows that the keyboard in DOSbox isn't just a straight passthrough, it functions as a buffer.
If the buffer's empty, hooray, it acts like a passthrough.
If it isn't the key presses are stored and executed in order.
This means that we don't need to block the keyboard to use another input, or vice-versa.
The next step is adding a function into `keyboard.cpp` that generates random-ish keyboard presses, by calling `KEYBOARD_AddBuffer`, and figuring out how to call it from the main file in DOSBox.

## 8 - Compiling DOSBox
DOSBox has some... interesting dependencies when you're trying to compile it.
Ed's been hard at work trying to use the built-in build files to build version 0.74 of DOSbox from source.
This is a very important step in our process, as the changes we make to DOSBox are useless if we can't then compile them and run them.

## 9 - `/dev/input/js0`
We're not giving up on DOSbox, however we are exploring alternatives.
One such alternative is the path `/dev/input/js0`, which, as the name may suggest, is a joystick input.
I wrote a quick Python script to read its output, which with no joystick connected is of the form:
```
[140, 84, 253, 255, 0, 0, 129, 0]
[140, 84, 253, 255, 0, 0, 129, 1]
[140, 84, 253, 255, 0, 0, 129, 2]
[140, 84, 253, 255, 0, 0, 129, 3]
[140, 84, 253, 255, 0, 0, 129, 4]
[140, 84, 253, 255, 1, 128, 130, 0]
[140, 84, 253, 255, 1, 128, 130, 1]
```
These lists, each 8 characters long, correspond to something. However, with no access to a joystick, I can't say what yet. More investigation needs doing while Ed finds the dependency files to compile DOSbox

## 10 - A Return to `xdotool`
It's been a learning process, trying to emulate keyboard input, and we feel that we gave up on `xdotool` too early, and for the wrong reasons.
With that in mind, we have returned to it, and with some bash scripting magic, we reckon that it's stable and fast enough as a solution to work in our application.
So we come back to it, using what we learned from `dev/input/js0`, and with some knowledge of keycodes, we can have it pass through an input it gets from a pipe as a keystroke.
This means little to no decision logic, which will greatly reduce latency, and result in a more portable bit of code.

## 10.5 Translation Table
| XDotool         | Actual      |
|-----------------|-------------|
|000              |0            |
|001              |1            |
|002              |2            |
|003              |3            |
|004              |4            |
|005              |5            |
|006              |6            |
|007              |7            |
|008              |8            |
|009              |9            |
|010              |1            |
|011              |2            |
|012              |3            |
|013              |4            |
|014              |5            |
|015              |6            |
|016              |7            |
|017              |8            |
|018              |9            |
|019              |0            |
|020              |-            |
|021              |=            |
|022              |DELETE       |
|023              |TAB          |
|024              |q            |
|025              |w            |
|026              |e            |
|027              |r            |
|028              |t            |
|029              |y            |
|030              |u            |
|031              |i            |
|032              |o            |
|033              |p            |
|034              |[            |
|035              |]            |
|036              |RETURN       |
|037              |CAPSLOCK     |
|038              |a            |
|039              |s            |
|040              |d            |
|041              |f            |
|042              |g            |
|043              |h            |
|044              |j            |
|045              |k            |
|046              |l            |
|047              |;            |
|048              |'            |
|049              |\`           |
|050              |SHIFT        |
|051              |\            |
|052              |z            |
|053              |x            |
|054              |c            |
|055              |v            |
|056              |b            |
|057              |n            |
|058              |m            |
|059              |,            |
|060              |.            |
|061              |/            |
|062              |SHIFT        |
|063              |\*           |
|064              |             |
|065              |' '          |
