# CS50 TSE - Indexer
## Amethyst McKenzie CS50 Spring 2022

### Indexer

*indexer* parses webpages in a Crawler-produced directory (`pageDirectory`) and extracts non-trivial words, keeping track of where the word was found (docID), and how many times it appeared (count) within each docID. It saves the built index to the `indexFilename`.

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation
* `indextest.c` - index.h unit testing
* `testing.sh` - unit test driver
* `testing.out` - output of testing
* `IMPLEMENTATION.md` - description of the implementation of indexer
* `DESIGN.md` - description of the design of indexer
* `REQUIREMENTS.md` - description of the requirements of indexer

### Compilation

To compile, simply `make`.

### Testing

indextest tests the specific functions `index_load` and `index_save` from `index.h` and ensures they are working correctly by comparing them to the files we produce in indexer. If the sorted files produced by indextest are identical to the sorted files produced by indexer, we know that `index_load` and `index_save` work as intended. 

When files are compared: if they are identical, the program prints out nothing. If they differ, the program will log that the files differ. The testing driver additionally logs when files are being sorted and compared. 

To test, simply `make test`.

To test with valgrind, `make valgrind`.






