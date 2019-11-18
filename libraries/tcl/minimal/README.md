# Minimal TCL Example

This demonstration program shows an implementation of a simple C program which
instantiates an interactive TCL interpreter with a C function exposed into TCL,
as well as a linked variable.

## Example

```
$ make
cc -I/usr/include/tcl8.6 -ltcl -lreadline -c minimal.c
cc -I/usr/include/tcl8.6 minimal.o -o minimal -ltcl -lreadline
$ ./minimal 
minimal> puts "hello from TCL!"
hello from TCL!

minimal> puts "current time is $linked_time"
current time is 1574052611

minimal> puts "current time is $linked_time"
current time is 1574052614

minimal> puts "the 7th fibonacci number is: [fibo 7]"
the 7th fibonacci number is: 13

minimal> fibo 
wrong # args: should be "usage: fibo N"
minimal> fibo 7
13
minimal> fibo 20
6765
minimal> exit
```

## Dependencies

On Ubuntu 18.04

`sudo apt install tcl-dev libreadline-dev`
