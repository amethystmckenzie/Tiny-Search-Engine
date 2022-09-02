# CS50 TSE - Querier
## Amethyst McKenzie CS50 Spring 2022

### Querier

The *querier* reads an index file produced by TSE Indexer, `indexFilename`, and a page directory produced by the TSE Crawler, `pageDirectory`, and answers search queries submitted via stdin.

### Assumptions

Assume that `indexFilename` has been produced by TSE Indexer.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation
* `fuzzquery.c` - helper program for testing
* `testing.sh` - unit test driver
* `testing.out` - output of testing
* `IMPLEMENTATION.md` - description of the implementation of querier
* `DESIGN.md` - description of the design of querier
* `REQUIREMENTS.md` - description of the requirements of querier

### Compilation

To compile, simply `make`.

### Testing

To test, simply `make test`.

To test with valgrind, `make valgrind`.






