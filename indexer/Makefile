# Makefile for 'indexer'
#
# Amethyst McKenzie, CS50, Spring 2022

C = ../common
L = ../libcs50

PROGS = indexer indextest
OBJS = indexer.o indextest.o
LLIBS = $C/common.a $L/libcs50.a
TEST_DATA = index-letters-3 indexer-l3 indexer-ts1 indexer-w1 indextest-l3 indextest-ts1 indextest-w1 ORIGINAL-index-l3 ORIGINAL-index-ts1 ORIGINAL-index-w1 sorted-indexer-l3 sorted-indexer-ts1 sorted-indexer-w1 sorted-indextest-l3 sorted-indextest-ts1 sorted-indextest-w1 indextest-valgrind-l3 invalid

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
VFLAGS = --track-origins=yes --leak-check=full --show-leak-kinds=all
CC = gcc
MAKE = make

.PHONY: all indexer indextest test valgrind clean

###### default ######
all: $(PROGS)

###### indexer ######
indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# dependencies: object files depend on header files
indexer.o: $C/index.h $L/webpage.h $L/memory.h $C/word.h $L/counters.h $L/file.h $C/pagedir.h
indextest.o: $C/index.h $L/memory.h $L/file.h 

###### test ######
test: $(PROGS) testing.sh
	chmod +x testing.sh
	./testing.sh &> testing.out

###### valgrind ######
valgrind: $(PROGS)
	valgrind $(VFLAGS) ./indexer ../crawler/letters-3 index-letters-3
	valgrind $(VFLAGS) ./indextest index-letters-3 indextest-valgrind-l3

###### clean ######
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROGS)
	rm -f $(TEST_DATA)
