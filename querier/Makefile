# Makefile for 'querier'
#
# Amethyst McKenzie, CS50, Spring 2022

C = ../common
L = ../libcs50

PROGS = querier fuzzquery
OBJS = querier.o fuzzquery.o
LLIBS = $C/common.a $L/libcs50.a
TEST_DATA = valgrind.out

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
VFLAGS = --track-origins=yes --leak-check=full --show-leak-kinds=all
CC = gcc
MAKE = make

.PHONY: all querier fuzzquery test valgrind clean

###### default ######
all: $(PROGS)

###### querier ######
querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

###### fuzzquery ######
fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# dependencies: object files depend on header files
querier.o: $C/index.h $L/webpage.h $L/memory.h $L/file.h $C/pagedir.h
fuzzquery.o: $L/memory.h $L/file.h

###### test ######
test: $(PROGS) testing.sh
	chmod +x testing.sh
	./testing.sh &> testing.out

###### valgrind ######
valgrind: $(PROGS)
	valgrind $(VFLAGS) ./fuzzquery /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-2 30 1 | ./querier /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-depth-2 /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-2 &> valgrind.out

###### clean ######
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROGS)
	rm -f $(TEST_DATA)
