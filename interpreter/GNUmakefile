# public domain

include opinionated.mk

CC ?= gcc
CFLAGS ?= -O0 -g3 -Wall -Wextra
OBJ = lexer.o tokens.o

tests: lexer.test
> ./lexer.test
.PHONY: tests

%.test: $(OBJ) %.test.c
> $(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h util.h
> $(CC) $(CFLAGS) -c $<

%.o: %.c util.h
> $(CC) $(CFLAGS) -c $<

clean:
> rm -f *.o *.test
.PHONY: clean

