# CS50 TSE - Crawler
## Amethyst McKenzie CS50 Spring 2022

### Crawler

*crawler* crawls a website and retrieves webpages starting from a seed URL (`seedURL`).
It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages until it reaches the max depth (`maxDepth`). It writes the page contents to a file with unique document ID in an already existing and writable directory (`pageDir`).

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the implementation
* `testing.sh` - unit test driver
* `testing.out` - output of testing
* `IMPLEMENTATION.md` - description of the implementation of crawler
* `DESIGN.md` - description of the design of crawler
* `REQUIREMENTS.md` - description of the requirements of crawler


### Compilation

To compile, simply `make`.

### Testing

The `crawler.c` program reads hardcoded data and follows the testing procedure outlined in the DESIGN spec

- Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

- Test the crawler with a `seedURL` that points to a non-existent server.

- Test the crawler with a `seedURL` that points to a non-internal server.

- Test the crawler with a `seedURL` that points to a valid server but non-existent page.

- Crawl a simple, closed set of cross-linked web pages.

- Point the crawler at a page in that site, and explore at depths 0, 1, 2, 3, 4, 5.

- Repeat with a different seed page in that same site.

- Point the crawler at the Wikipedia playground and explore at depths 0, 1, 2.

To test, simply `make test`.

To test with valgrind, `make valgrind`.




