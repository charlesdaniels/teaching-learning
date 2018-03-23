This program demonstrates the following;

* Implementation of linked lists in C.
* Use of `malloc` and `free` in C.
* Use of `sprintf` in C.
* Use of `void*` in C.

This directory shows an example of an idiomatic doubly linked list in C using
structs. The linked list which is implemented is capable of storing arbitrary
types of data, but integers are used for demonstration purposes.

This list supports append, prepend, and insert operations, as well as element
deletion, and forward and backward iteration. Helper methods are provided to
display a single `node` instance showing it's memory address and the addresses
of the next and previous nodes in the list. A values of `NULL` is used to
signal the beginning or end of the linked list.

Sample output from demo file:

```
created head node: (nil) -> 0x66f030 (0) -> (nil)
inserting 1...
inserting 2...
inserting 3...
inserting 4...
inserting 5...
inserting 6...
inserting 7...
inserting 8...
inserting 9...
List state is:
(nil) -> 0x66f030 (0) -> 0x670890
0x66f030 -> 0x670890 (1) -> 0x6708d0
0x670890 -> 0x6708d0 (2) -> 0x670910
0x6708d0 -> 0x670910 (3) -> 0x670950
0x670910 -> 0x670950 (4) -> 0x670990
0x670950 -> 0x670990 (5) -> 0x6709d0
0x670990 -> 0x6709d0 (6) -> 0x670a10
0x6709d0 -> 0x670a10 (7) -> 0x670a50
0x670a10 -> 0x670a50 (8) -> 0x670a90
0x670a50 -> 0x670a90 (9) -> (nil)
The 5th node is: 0x670950 -> 0x670990 (5) -> 0x6709d0
Deleting the 5th node...
List state is:
(nil) -> 0x66f030 (0) -> 0x670890
0x66f030 -> 0x670890 (1) -> 0x6708d0
0x670890 -> 0x6708d0 (2) -> 0x670910
0x6708d0 -> 0x670910 (3) -> 0x670950
0x670910 -> 0x670950 (4) -> 0x6709d0
0x670950 -> 0x6709d0 (6) -> 0x670a10
0x6709d0 -> 0x670a10 (7) -> 0x670a50
0x670a10 -> 0x670a50 (8) -> 0x670a90
0x670a50 -> 0x670a90 (9) -> (nil)
```
