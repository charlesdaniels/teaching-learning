This is a very simple example of a comandline program in C. This program reads
integers from standard input, adds or subtracts a specified quantity to each,
and prints the result to standard out.

This program demonstrates the following concepts:

* Parsing `argv` (`getopt()`)
* Reading arbitrary amounts of information from standard in safely
  (`getline()`).

**NOTE**: This requires your C compiler to support at least `POSIX.1-2008`.
