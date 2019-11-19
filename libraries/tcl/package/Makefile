TCLLIB=/usr/include/tcl8.6
TCL_CFLAGS=-I$(TCLLIB)
TCL_LIBS=-ltcl
CC=cc

CFLAGS+=$(TCL_CFLAGS)
LIBS+=$(TCL_LIBS) -lreadline

samplepackage.so: samplepackage.c
	gcc -shared -o $@ -DUSE_TCL_STUBS $(CFLAGS) $^ -L$(TCLLIB) -ltclstub8.6 $(LIBS)

.PHONY: clean

clean:
	rm -f samplepackage.so
