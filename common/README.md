# CS50 TSE - Common
## Amethyst McKenzie CS50 Spring 2022

### Common

The common directory assembles code that will be shared between crawler, indexer, and querier.

### Usage
The common directory has the following functions implemented by pagedir.h, word.h and index.h:

#### pagedir.h
```c
bool valid_directory(char *directoryName);
void page_saver(webpage_t* page, char* directoryname, int id);
bool crawler_directory(char *dir);
webpage_t *load_page(char *pageDirectory, int id);
```
pagedir contains functions that check if a directory is valid, save a page to a directory, check if a directory was produced by crawler and converts a file into a webpage.

#### word.h
```c
char *NormalizeWord(char *word);
```
**normalize the word** means to convert it into lowercase
A simple example:
```
pLayGROUND becomes playground
```

#### index.h
```c
index_t *index_new(const int num_slots);
bool index_insert(index_t *index, const char *key, counters_t *counter);
void *index_find(index_t *index, const char *key);
index_t *index_load(char* index_file);
bool index_save(index_t* index, char* index_file);
void index_delete(index_t* index, void (*itemdelete)(void *item));
```
index is a module that acts similar to a hashtable, but pairs strings as keys and counter data types as values. Within each counter data strucutre, the file ID (int) is paired to the number of appearances a the key appears in the file (int).

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `pagedir.h` - the pagedir interface
* `pagedir.c` - the pagedir implementation
* `word.h ` - the word interface
* `word.c` - the word implementation
* `index.h` - the index interface
* `index.c` - the index implementation

### Compilation

To compile, simply `make`.

This will make a `common.a` library that is later included in the `crawler`, `indexer` and `querier` Makefile.




