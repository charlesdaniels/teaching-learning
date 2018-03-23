CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Werror --std=iso9899:2011

simple-cli: simple-cli.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

demo: simple-cli demo_input.txt
	./simple-cli < demo_input.txt

clean:
	rm -f *.o
	rm -f *.gch
	rm -f simple-cli
