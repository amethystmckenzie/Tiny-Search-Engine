# TSE Indexer Implementation Spec
## Amethyst McKenzie CS50 Spring 2022

### General Implemenation
I implemented indexer using an inverted index *index*, a data structure defined in `index.h` in the common directory.

The *index* itself is represented as a `typdef hashtable_t index_t` where most of the functionality stems from the `struct hashtable`. *index* takes an integer of the number of slots the structure will have, which I hard-coded to be 600 for optimal performance. 

The `index_new`, `index_insert`, `index_find` and `index_delete` are implemented almost exactly like their counterparts in `hashtable`, except instead of storing a string and an item, the *index* stores a string and a *counters* data structure. 

The index also has two new functions, `index_load` and `index_save` which load an index from a file and save an index to a file respectively. *indexer* utilises the `index_save` function. 

The *indexer* parses webpages in a Crawler-produced directory (`pageDirectory`) and extracts non-trivial words, keeping track of where the word was found (docID), and how many times it appeared (count) within each docID. It saves the built index to the `indexFilename`.

The user inputs the `pageDirectory` and `indexFilename` via command-line arguments. 

### Functions and Modules
 1. *main* which gets parsed arguments, builds, and saves the index.
 2. *parse_arguments* which parses arguments from the command-line and validates parameters
 3. *index_build* which loops over all webpages in the crawler directory and indexes them, building an index.
 4. *index_page* which maps the words found in each webpage to their docID and count.

```c
static void parse_arguments(const int c, char *argv[], char **pageDirectory, char **indexFilename);
static index_t *index_build(char *pageDirectory);
static void index_page(index_t *index, webpage_t *page, int id);
```

### Data Structures and Interfaces
*index* of words mapped to their docID location and count
*hashtable* which is implemented indirectly through index

`indexer.c` implements the following functions through index.h, webpage.h, pagedir.h, word.h, memory.h and file.h.

index.h
```c
index_t *index_new(const int num_slots);
bool index_insert(index_t *index, const char *key, counters_t *counter);
void *index_find(index_t *index, const char *key);
bool index_save(index_t* index, char* index_file);
void index_delete(index_t *index, void (*itemdelete)(void *item));
```

webpage.h
```c
webpage_t *webpage_new(char *url, const int depth, char *html);
char *webpage_getNextWord(webpage_t *page, int *pos);
void webpage_delete(void *data);
```

pagedir.h
```c
bool crawler_directory(char *dir);
webpage_t *load_page(char *pageDirectory, int id);
```

word.h
```c
void normalize_word(char *word)
```

memory.h
```c
void *assertp(void *p, const char *message);
```

file.h
```c
char *freadlinep(FILE *fp);
char *freadfilep(FILE *fp);
```
*freadlinep* and *freadfilep* implemented indirectly from *load_page*

*load_page* converts files in `pageDirectory` back into webpages
*crawler_directory* checks if `pageDirectory` is produced by Crawler
*normalise_word* normalises a word (sets word to lowercase)
*index_save* outputs the built index to `indexFilename` by iterating through the entire index and printing it to the file. 

### Pseduocode

1. main
- Call *parse_arguments* to parse through command-line arguments and validate parameters.
- Build the index in memory by calling *index_build* 
- Save the index to the external file `indexFilename` provided by command-line argument by calling *index_save*
- Clean up data structures

2. parse_arguments
- Parse command-line arguments
- Check if command-line arguments are valid
- Exit non-zero if any errors

3. index_build
- Initialse modules and docID
- Go through each file in the `pageDirectory` (that has been converted into a webpage via *load_page*)
    - Index the page using *index_page*
    - Delete the webpage using *webage_delete*
    - Increment docID
- Return built index

4. index_page
- While there are words to be extracted from the webpage (*webpage_getNextWord()*)
    - If the word is longer than 2 characters
        - Normalise the word
        - Find the counters related to the word from index using *index_find*
        - If the counters is NULL
            - create a new counters using *counters_new*
            - Increment the count for that new counters using *counters_add*
            - Insert new counters to index using *index_insert*
        - Else, incremenet counters by 1 using *counters_add*
    - Free the word

5. load_page (which is in the common directory)
- Validate parameters
- Initialise modules
- Create filename with proper pageDirectory/id format
- Open FILE for reading (if NULL, return NULL)
- Get URL and depth of the webpage by calling *freadlinep* on file (URL is first line, depth is second line)
- Get HTML by calling *freadfilep* on file
- Convert the depth to an integer
- Free the string storing the char * version of the depth
- Create webpage using *webpage_new*
- Close file
- Return webpage

6. normalise_word (which is in the common directory)
- For every character in the word
    - lowercase the character

7. crawler_directory (which is in the common directory)
- Create filename with pageDirectory/.crawler format (all Crawler directories are programmed to produce a .crawler file)
- Open file for reading
- If file is NULL, return false
- Else, close file and return true

### Error Handling and Recovery
For any invalid command-line parameters, including:
1. Wrong number of arguments
2. Invalid `pageDirectory`
3. Invalid `indexFilename`

the program prints an error message and exits with a non-zero integer

For any issues with allocating memory throughout the program, I use `assertp()` to check if the return value is NULL. If it is NULL, the program is terminated. 

### Testing

The `indexer.c` program reads hardcoded data and follows the testing procedure outlined in the DESIGN spec

Various functionality tests are conducted:

Integration Testing:
* TEST 0-2: Invalid Parameters
* TEST 3-5: Indexer Functionality
* TEST 6-8: Indextest Functionality

- Tests the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

- Tests the `indexer` with letters at depth 3

- Tests the `indexer` with toscrape at depth 1

- Tests the `indexer` with wikipedia at depth 1

Regression and Unit Testing:
- Sorts all files and compares them to their sorted counterparts in the TSE example output provided in `/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/`

- Testing also runs indextest on each of the `indexer` produced files above, which test `index_load` and `index_save` functions from `index.h`. It sorts and compares the files produced to the sorted `indexer` produced files. 

If the files are identical, the program prints out nothing. If they differ, the program will log that the files differ. 