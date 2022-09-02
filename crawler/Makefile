# Makefile for 'crawler'
#
# Amethyst McKenzie, CS50, Spring 2022

C = ../common
L = ../libcs50

PROG = crawler
OBJS = crawler.o
LLIBS = $C/common.a $L/libcs50.a
TEST = testing.sh

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
VFLAGS = --track-origins=yes --leak-check=full --show-leak-kinds=all
CC = gcc
MAKE = make

.PHONY: all test valgrind clean

###### default ######
all: $(PROG)

###### crawler ######
crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# dependencies: object files depend on header files
crawler.o: $L/webpage.h $C/pagedir.h
crawler.o: $L/memory.h $L/bag.h $L/hashtable.h


###### test ######
test: $(PROG) $(TEST)
	chmod +x testing.sh
	./$(TEST) &> testing.out


###### valgrind ######
valgrind: $(PROG)
	valgrind $(VFLAGS) ./$(PROG) http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 2


###### clean ######
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

