TCL_CFLAGS=-I/usr/include/tcl8.6
TCL_LIBS=-ltcl
CC=cc

CFLAGS+=$(TCL_CFLAGS)
LIBS+=$(TCL_LIBS) -lreadline

minimal: minimal.o
	$(CC) $(CFLAGS) minimal.o -o minimal $(LIBS)

minimal.o: minimal.c
	$(CC) $(CFLAGS) $(LIBS) -c minimal.c

.PHONY: clean

clean:
	rm -f minimal.o minimal
