include ./opinionated.mk

# Copyright 2020 Charles Daniels
#
# See README.md for license information.
#
# Based on https://raytracing.github.io/

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g3 -O0
LDFLAGS = -lm
HEADERS=util.h vec.h ray.h hit.h camera.h
OBJ=util.o vec.o ray.o hit.o camera.o

%: %.c $(OBJ) $(HEADERS)
> $(CC) $(CFLAGS) -DOUTFILE=\"$@.hif24\" $^ $(LDFLAGS) -o $@

%.o: %.c %.h $(HEADERS)
> $(CC) $(CFLAGS) -c $<

%.o: %.c $(HEADERS)
> $(CC) $(CFLAGS) -c $<

clean:
> rm -f *.o *.hif24
> for f in *.c ; do rm -f "$$(basename "$$f" .c)" ; done
.PHONY: clean
