# Notes

## 1 - FIFOs
I've been investigating the best ways to take the output of one program and feed it in as the input of another.
This, it seems, is best done by something called a FIFO.
This is something in C that approximates a file without actually creating one.
You create one using the `mkfifo` command, and you can then interact with it exactly as you would a file.
There's some race condition stuff that you can get around using a `usleep` (in `unistd.h`), we'd probably use that.

