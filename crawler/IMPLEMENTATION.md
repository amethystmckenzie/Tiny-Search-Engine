# TSE Crawler Implementation Spec
## Amethyst McKenzie CS50 Spring 2022

### General Implemenation
I implemented crawler by following the pseudocode provided in the DESIGN spec by cs50.

The *crawler* crawls a website and retrieves webpages starting from a seed URL (`seedURL`).
It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages until it reaches the max depth (`maxDepth`). It writes the page contents to a file with unique document ID in an already existing and writable directory (`pageDir`).

The user inputs the `seedURL`, `pageDir` and `maxDepth` via command-line arguments. 

### Functions and Modules
1. *main*, which parses arguments and initializes other modules
2. *crawler*, which loops over pages to explore, until the list is exhausted
3. *pagefetcher*, which fetches a page from a URL
4. *pagescanner*, which extracts URLs from a page and processes each one
4. *pagesaver*, which outputs a page to the the appropriate file
5. *valid_directory*, which checks if the directory being written to exists and is writable
6. *logr*, which logs messages of the program to the user's terminal

```c
void crawler(char *url, char *pageDir, int maxDepth);
void page_fetcher(webpage_t *webpage);
void page_scanner(webpage_t *webpage, bag_t *bag, hashtable_t *hash);
inline static void logr(const char *word, const int depth, const char *url);
```

### Data Structures and Interfaces
*bag* of pages we have yet to explore, implemented through bag.h

*hashtable* of URLs we've seen so far, implemented through hashtable.h

*set* which is implemented indirectly through hashtable

bag.h
```c
bag_t *bag_new(void);
void bag_insert(bag_t *bag, void *item);
void *bag_extract(bag_t *bag);
void bag_delete(bag_t *bag, void (*itemdelete)(void *item) );
```

hashtable.h
```c
hashtable_t *hashtable_new(const int num_slots);
bool hashtable_insert(hashtable_t *ht, const char *key, void *item);
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) );
```

`crawler.c` implements the following functions through webpage.h, pagedir.h and memory.h

webpage.h
```c
int webpage_getDepth(const webpage_t *page);
char *webpage_getURL(const webpage_t *page);
char *webpage_getHTML(const webpage_t *page);
webpage_t *webpage_new(char *url, const int depth, char *html);
bool webpage_fetch(webpage_t *page);
char *webpage_getNextURL(webpage_t *page, int *pos);
bool NormalizeURL(char *url);
bool IsInternalURL(char *url);
void webpage_delete(void *data);
```

pagedir.h
```c
bool valid_directory(char *directoryName);
void page_saver(webpage_t* page, char* directoryname, int id);
```

memory.h
```c
void *assertp(void *p, const char *message);
```

### Pseduocode

1. main
- Parse through command-line arguments
- Check for invalid parameters and arguments

2. crawler
- Initialise data structures and document ID number
- make a *webpage* for `seedURL` with depth of 0
- add the webpage to the bag of webpages to crawl
- insert the URL of that webpage into the hashtable
- while there are webpages to crawl (the bag is empty)
    - extract a webpage
    - use *page_fetcher* to fetch the HTML of the webpage with a 1 second delay
    - if the HTML was retrieved successfully
        - use *page_saver* to write the webpage to `pageDir` with a unique document ID number
        - incrememnt the ID number
        - if the depth of the webpage is less than the `maxDepth`
            - use *page_scanner* to extract the webpage's embedded URLs
    - delete the extracted webpage

- delete bag and hashtable

3. page_fetcher
- if webpage can be fetched
    - log that the webpage has been fetched
- else
    - log that the webpage could not be fetched, allowing the program to continue to the next webpage

4. page_scanner
- while there are embedded URLs to be extracted
    - normalise the URL
        - check if URL is internal (if not, ignore)
            - try to insert it into the hashtable (if it's already been inserted, do nothing)
                - if it was added to the hashtable successfully, create a new webpage for that URL
                - add it to the bag of webpages to crawl

5. page_saver (which is in the common directory)
- allocate memory for filename
- create filename with proper `pageDir/id` format
- write the contents of the webpage into a new file in `pageDir`, if possible

6. isValidDirectory (which is in the common directory)
- create a file in the directory
- if the file is NULL, that means directory is invalid, so return false
- else, the directory is valid, so return true

### Error Handling and Recovery
For any invalid command-line parameters, including:
1. Wrong number of arguments
2. Invalid maxDepth
3. Out of range maxDepth
4. Invalid directory
5. Inability to Normalise URL 
6. Non-Internal URL

the program prints an error message and returns with a non-zero integer

For any issues with allocating memory throughout the program, I use `assertp()` to check if the return value is NULL. If it is NULL,
the program is terminated. 

For any non-existent, valid websites the program logs these as a "failed fetch" and continues to the next URL
