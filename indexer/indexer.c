/* 
 * indexer.c - reads document files produced by TSE crawler, builds an index, and writes that index to a file.
 *
 * usage: ./indexer pageDirectory indexFilename
 * where `pageDirectory` is the pathname of a directory produced by the Crawler
 * where `indexFilename` is the pathname of a file into which the index should be written
 * 
 * Amethyst McKenzie, May 9 2022, cs50
 */

#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "webpage.h"
#include "memory.h"
#include "word.h"
#include "counters.h"
#include "file.h"
#include "pagedir.h"

/* function declarations */
static void parse_arguments(const int c, char *argv[], char **pageDirectory, char **indexFilename);
static index_t *index_build(char *pageDirectory);
static void index_page(index_t *index, webpage_t *page, int id);

/* Purpose: Parse arguments from command-line and build index
 * Parameters: c = number of arguments, argv= argument
 * Return: exit code (0 if ran successfully, non-0 if error)
 */
int main(const int c, char* argv[])
{
    // indexer parameters from command line
    char *pageDirectory = NULL;
    char *indexFilename = NULL;

    // parse arguments, function exits if erros with parsing
    parse_arguments(c, argv, &pageDirectory, &indexFilename);

    // build and save index to indexFilename
    index_t *index = index_build(pageDirectory);
    index_save(index, indexFilename);

    // clean up
    index_delete(index, NULL);

    return 0;

}

/* Purpose: Helper function for main--parses the command-line arguments
 * Parameters: c=number of arguments, argv=argument, pageDirectory=directory we read files from, indexFilename=file we write to
 * Return: Exits non-zero if error
 */
static void parse_arguments(const int c, char *argv[], char **pageDirectory, char **indexFilename)
{   
    // check if valid number of arguments
    if(c != 3){
        fprintf(stderr, "Wrong number of arguments. usage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }

    // check if valid crawler directory
    *pageDirectory = argv[1];
    if(!crawler_directory(*pageDirectory)){
        fprintf(stderr, "Directory not produced by Crawler\n");
        exit(2);
    }

    // check if valid, writable file
    *indexFilename = argv[2];
    FILE *fp;
    if((fp = fopen(*indexFilename, "w")) == NULL){
        fprintf(stderr, "Could not open %s for writing\n", *indexFilename);
        exit(3);
    }

    // clean up
    fclose(fp);
}


/* Purpose: Builds the index by loading data from pageDirectory
 * Parameters: pageDirectory=directory we read files from
 * Return: The built index, exits non-zero if error with memory allocation
 */
static index_t *index_build(char *pageDirectory)
{       
    // intialise modules
    int id = 1;
    index_t *index = assertp(index_new(600), "indexer index_new");
    webpage_t *page;

    // go through each page in the directory
    while((page = load_page(pageDirectory, id)) != NULL){
        index_page(index, page, id); // index the page
        webpage_delete(page); // delete page
        id += 1; // go on to next file (if it exists)
    }

    return index;
}

/* Purpose: Maps words to (id, count) pairs to keep track of the count of each word in every file
 * Parameters: index=the built index, page=webpage we search words in id=document ID
 * Return: Void
 */
static void index_page(index_t *index, webpage_t *page, int id)
{
    int pos = 0;
    char *word;

    // get all words from the page
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        // skip trivial words
        if(strlen(word) >= 3){
            normalize_word(word); // normalise word
            counters_t *counter = index_find(index, word); // find counters related to word in index

            // if key is not in index
            if(counter == NULL){
                counters_t *new_counter = assertp(counters_new(), "index_page counters_new"); // create new counters
                counters_add(new_counter, id); // increment new counters
                index_insert(index, word, new_counter); // insert into index, with word being the key
            }
            else{
                counters_add(counter, id); // increment count by 1
            }
        }
        // free word
        free(word);
    }
}