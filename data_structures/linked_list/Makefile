CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Werror --std=iso9899:2011

demo.bin: linked_list.o demo.o
	$(CC) -o $@ $(CFLAGS) $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

demo: demo.bin
	./demo.bin

clean:
	rm -f *.o
	rm -f *.gch
	rm -f demo.bin
