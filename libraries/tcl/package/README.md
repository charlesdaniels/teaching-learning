# Minimal TCL Example

This demonstration shows a C-based extension to TCL. In general, this is the
preferred way of interfacing C and TCL code.

An example is also shown of calling our C code from Python, by using
Python's `tkinter` module as an intermediary.

If you wish to turn your C extension into a proper TCL package, see the
*Creating a Package* section on [this
page](https://wiki.tcl-lang.org/page/Hello+World+as+a+C+extension).

## Example

```
$ make
gcc -shared -o samplepackage.so -DUSE_TCL_STUBS -I/usr/include/tcl8.6 samplepackage.c -L/usr/include/tcl8.6 -ltclstub8.6 -ltcl -lreadline
$ tclsh
% load ./samplepackage.so
% samplepackage::fibo
wrong # args: should be "usage: fibo N"
% samplepackage::fibo 3
2
% exit
$ tclsh ./example.tcl
sleeping for 2 seconds...
the 14th Fibonacci number is: 377
system time in seconds is: 1574184136
the 17th Fibonacci number is: 1597
$ python3 ./interop.py
the 9th Fibonacci number is: 34
```

## Dependencies

On Ubuntu 18.04

`sudo apt install tcl-dev libreadline-dev`
