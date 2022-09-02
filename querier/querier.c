/* 
 * querier.c - Answers questions from the user by outputting the best matching webpages (in decreasing order by their score)
 *
 * usage: ./querier pageDirectory indexFilename
 * where pageDirectory is the pathname of a directory produced by the Crawler
 * where indexFilename is the pathname of a file produced by the Indexer.
 * 
 * Amethyst McKenzie, May 18 2022, cs50
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "file.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"
#include "counters.h"
#include "memory.h"
#include "webpage.h"
#include <unistd.h>

/******** local data types *******/
struct twocts {
	counters_t *result;
	counters_t *another;
};

struct counternode {
    int docID;
    int score;
};

/******** function declarations *******/

/* query */
static void parse_arguments(const int c, char *argv[], char **pageDirectory, char **indexFilename);
static void query_print(char **array);
static char **parse_query(char *line);
static bool valid_query(char **array);
static bool valid_character(char *line);

/* merging and intersecting counters */
static void counters_intersect(counters_t* ct1, counters_t* ct2); 
static void intersect_helper(void *arg, const int key, const int count);
static void counters_union(counters_t* ct1, counters_t* ct2);
static void counters_union_helper(void *arg, const int key, int count);
static void itemcount(void *arg, int key, int count);

/* counters -> array */
static struct counternode **create_s_array(counters_t *ct);
static void fill_array(void *arg, const int key, int count);
static void insertion_sort(struct counternode **array, int n);
static void array_delete(struct counternode **array);

/* querier */
static counters_t *querier(char **query, char *indexFilename);
static void found_documents(struct counternode **sorted_array);

/* prompt user */
static void prompt(void);
int fileno(FILE *stream);

/* calcuate minimum */
static inline int min(const int a, const int b) {
    return (a < b ? a : b);
}

/* Purpose: Parse arguments from command-line and run querier
 * Parameters: c = number of arguments, argv= argument
 * Return: Exit code (0 if ran successfully, non-0 if error)
 */
int main(const int c, char* argv[])
{
    // querier parameters from command line
    char *pageDirectory = NULL;
    char *indexFilename = NULL;

    // parse arguments, function exits if erros with parsing
    parse_arguments(c, argv, &pageDirectory, &indexFilename);
    
    printf("-----------------------------------------------\n");
    // prompt user (if reading from terminal)
    prompt();

    // clean/flush the output buffer
    fflush(stdout);

    char *line;
    // read standard input from the user 
    while((line = freadlinep(stdin)) != NULL){

        // if no invalid characters (and the line isn't just empty spaces)
        if(valid_character(line)){
            // create an array of words (query) by parsing through the line
            char **query = parse_query(line);
            
            // if the query is valid
            if(valid_query(query)){
                // print the normalised query
                printf("Query: ");
                query_print(query);
                printf("\n");

                // run querier
                counters_t *result = querier(query, indexFilename);

                // create a sorted array of the documents from the result
                struct counternode **sorted_array = sorted_array = create_s_array(result);

                // keeps track of how many documents were found
                found_documents(sorted_array);

                webpage_t *webpage;
                // print out ranked results
                for(int i = 0; sorted_array[i] != NULL; i++){
                    webpage = load_page(pageDirectory, sorted_array[i]->docID);
                    printf("score\t%d doc\t%d: %s\n", sorted_array[i]->score, sorted_array[i]->docID, webpage_getURL(webpage));
                    webpage_delete(webpage);
                }
                // clean-up
                array_delete(sorted_array);
                counters_delete(result);
            }

            free(query);
        }

        free(line);

        // re-prompt user
        prompt();
        printf("-----------------------------------------------\n");

        // clean/flush the output buffer and move buffered data to console
        fflush(stdout);
    }
}

/* Purpose: Helper function for main--parses the command-line arguments
 * Parameters: c = number of arguments, argv = argument, pageDirectory = directory we get webpages from, indexFilename = file we read from
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

    // check if valid, readable file
    *indexFilename = argv[2];
    FILE *fp;
    if((fp = fopen(*indexFilename, "r")) == NULL){
        fprintf(stderr, "Could not open %s for reading\n", *indexFilename);
        exit(3);
    }
 
    // clean up
    fclose(fp);
}

/* Purpose: Merges or intersects counters based on "and"/"or" operators
 * Parameters: query = clean parsed query from user, indexFilename = index we read from
 * Return: The final counter (after potential merges and intersections) (Exit non-zero if issues with NULL data structures, or memory)
 */
static counters_t *querier(char **query, char *indexFilename)
{
    counters_t *tmp = NULL;
    counters_t *result = assertp(counters_new(), "counters result invalid");

    index_t *index = index_load(indexFilename);

    // check the query for key "and"/"or" words
    for(int i = 0; query[i] != NULL; i++){ 
        // if "or"   
        if(strncmp(query[i], "or", 2) == 0){
            // merge tmp with result
            counters_union(tmp, result);
            // reset tmp
            counters_delete(tmp);
            tmp = NULL;
        }
        // if not "and" (if we see "and", just continue)
        else if (strncmp(query[i], "and", 3) != 0) {
            // find the word from the index
            counters_t *counter = index_find(index, query[i]);
            // if the word wasn't found, reset the sequence
            if(counter == NULL){
                counters_delete(tmp);
                tmp = NULL;
            }

            // update tmp
            if(tmp == NULL){
                // create new tmp and merge with counter if tmp is NULL
                tmp = assertp(counters_new(), "counters tmp invalid"); // current 'and' cluster
                counters_union(counter, tmp);
            }
            else{
                // intersect the counters
                counters_intersect(tmp, counter);
            }
        }
    }

    // end of query:
    if (tmp != NULL) {
        // merge tmp with result and reset tmp
        counters_union(tmp, result);
        counters_delete(tmp);
        tmp = NULL;
    }

    // clean-up
    index_delete(index, NULL);

    return result;
}

/* Purpose: Parse the user input and create a clean query 
 * Parameters: line = stdin from the user
 * Return: The parsed query (Exits non-zero if error with memory allocation)
 */
static char **parse_query(char *line)
{   
    // initialise variables
    char *word;
    char *rest;
    
    // initialise query
    char **query = assertp(calloc(strlen(line)+1, sizeof(char *)), "parse_query query"); 
    int query_index = 0; // keeps track of the array index

    // tokenise string
    for(rest=line; *rest != '\0'; ) {
        // find first letter of the word
        for(word=rest; *word != '\0' && isspace(*word); word++){
        }
        // find end of word
        for(rest=word; *rest != '\0' && isalpha(*rest); rest++){
        }
        // isolate word
        if(*rest != '\0'){
            // set rest position to end of word to prepare it for getting the next word
            *rest++ = '\0';
        }
        // normalise word add it to the array
        if(*word != '\0'){
            normalize_word(word);
            query[query_index] = word;
            query_index++;
        }
    }

    return query;
}

/* Purpose: Finds the number of documents in the sorted array
 * Parameters: sorted_array = sorted array of documents
 * Return: Void
 */
static void found_documents(struct counternode **sorted_array)
{
    int ndocs = 0; // number of matched documents

    // keeps track of how many documents were found
    for(int i = 0; sorted_array[i] != NULL; i++){
        ndocs++;
    }

    // prints them back to the user
    if(ndocs == 0){
        printf("No documents match.\n");
    }
    else {
        printf("Matches %d documents (ranked):\n", ndocs);
    }
}

/* Purpose: Checks if a character is valid or if the query is made up of only spaces
 * Parameters: line = stdin from the user
 * Return: True if the character is valid, false if it isn't (or the query is just spaces)
 */
static bool valid_character(char *line)
{
    // number of spaces in the line
    int nspaces = 0; 
    // checks each character in the line
    for(int i = 0; i < strlen(line); i++){
        if(isalpha(line[i])){
            continue;
        }
        else if(isspace(line[i])){
            nspaces++;
            continue;
        }
        else {
            fprintf(stderr, "Error: invalid character '%c' in query\n", line[i]);
            return false;
        }
    }

    // the query is only spaces
    if(nspaces == strlen(line)){
        return false;
    }

    return true;
}

/* Purpose: Validates the query 
 * Parameters: query = parsed query
 * Return: True if query is valid, false if it is invalid
 */
static bool valid_query(char **query)
{
    bool operator;

    // check if operator starts a query
    if(strncmp(query[0], "and", 3) == 0 || strncmp(query[0], "or", 2) == 0 ){
        fprintf(stderr, "Error: '%s' cannot be first\n", query[0]);
        return false;
    }

    // check if operator ends a query
    for(int i = 0; query[i] != NULL; i++){
        if(query[i+1] == NULL && ((strncmp(query[i], "and", 3) == 0 || (strncmp(query[i], "or", 2)) == 0))){
            fprintf(stderr, "Error: '%s' cannot be last\n", query[i]);
            return false;
        }
    }

    // check if two operators are next to each other
    for(int i = 0; query[i] != NULL; i++){
        if(strncmp(query[i], "and", 3) == 0 || strncmp(query[i], "or", 2) == 0){
            operator = true;
        }
        else {
            operator = false;
        }
        
        if(query[i+1] != NULL){
            if((strncmp(query[i+1], "or", 2) == 0 || strncmp(query[i+1], "and", 3) == 0) && operator){
                fprintf(stderr, "Error: Operators cannot be next to each other\n");
                return false;
            }
        }
    }
    
    return true;
}

/* Purpose: Prints the query back to the user
 * Parameters: query = parsed query
 * Return: Void
 */
static void query_print(char **query)
{
    for(int i = 0; query[i] != NULL; i++){
        printf("%s ", query[i]);
    }
}

/* Purpose: Intersects two counters (counter set of documents that match all given words). Used for "and" operator or spaces in between words.
 * Parameters: ct1 = a counter, ct2 = another counter
 * Return: Exits non-zero if any of the counters are NULL
 */
static void counters_intersect(counters_t *ct1, counters_t *ct2)
{
	assertp(ct1, "counters 1 invalid");
	assertp(ct2, "counters 2 invalid");

	struct twocts args = {ct1, ct2}; 
	counters_iterate(ct1, &args, intersect_helper);
}

/* Purpose: Checks if the document exists in the other counters set and updates first counters set accordingly
 * Parameters: arg = two counters struct, key = docID, count = count of words
 * Return: Void
 */
static void intersect_helper(void *arg, const int key, const int count)
{
	struct twocts *two = arg;

    // the score for each document is the minimum of the count for each document  
	counters_set(two->result, key, min(count, counters_get(two->another, key)));
}

/* Purpose: Merges two counters (counter set of documents that match either of the words given). Used for "or" operator. 
 * Parameters: ct1 = a counter, ct2 = another counter
 * Return: Void
 */
static void counters_union(counters_t *ct1, counters_t *ct2)
{   
    // iterate through the second counter set and check if the document exists in the first counter set
    counters_iterate(ct1, ct2, counters_union_helper);
}

/* Purpose: Checks if the document exists, and updates the count accordingly
 * Parameters: arg = a counter, key = docID, val = count of words
 * Return: Void
 */
static void counters_union_helper(void *arg, const int key, int val)
{
    counters_t *ct = arg;

    // get the counter
    int count = counters_get(ct, key);

    // if it doesn't exist yet, set it to the parameters
    if(count == 0){
        counters_set(ct, key, val);
    }

    // if it does exist, update the count
    else {
        counters_set(ct, key, count+val);
    }
}

/* Purpose: Creates an array of struct counternodes from the counters docID and score, sorted in decreasing order by score.
 * Parameters: ct = a counter
 * Return: An array of counternode structs
 */
static struct counternode **create_s_array(counters_t *ct)
{   
    assertp(ct, "create_s_array counters invalid");

    // calculate the number of elements in the counters set
    int n = 0;
    counters_iterate(ct, &n, itemcount);

    // allocate the appropiate size
    struct counternode **array = assertp(calloc(n+1, sizeof(struct counternode)), "array in create_s_array invalid");

    // insert each element into the array, and sort in decreasing order
    counters_iterate(ct, array, fill_array);
    insertion_sort(array, n);

    return array;
}   

/* Purpose: Helper function for create_s_array which fills the array
 * Parameters: ct = arg = an array of counternode structs, docID = unique doc ID, score = updated count
 * Return: Void
 */
static void fill_array(void *arg, const int docID, int score)
{   
    struct counternode **array = arg;
    int length = 0;
    
    // calculate length of array
    while(array[length] != NULL){
        length++;
    }
    
    // if the document matches (has a score greater than 0)
    if(score != 0){
        // allocate the appropiate size for a counternode and fill it
        struct counternode *counter = assertp(malloc(sizeof(struct counternode*)), "counternode in fill_array invalid");
        counter->docID = docID;
        counter->score = score;
        
        // add to the array
        array[length] = counter;
    }
}

/* Purpose: Sorts an array of structs in decreasing order
 * Parameters: array = an array of structs, n = the size of the array
 * Return: Void
 */
static void insertion_sort(struct counternode **array, int n)
{
    int i, j;
    struct counternode *counter;
    for (i = 1; i < n; i++) {
        if(array[i] == NULL){
            break;
        }
        
        counter = array[i];
        j = i-1;

        // Move elements that are less than the counter score to one position ahead of their current position
        while (j >= 0 && array[j]->score < counter->score) {
            array[j+1] = array[j];
            j = j-1;
        }
        array[j+1] = counter;
    }
}

/* Deletes an array of structs */
static void array_delete(struct counternode **array){
    // if the array exists
    if(array != NULL){
        // go through each element and free
        for(int i = 0; array[i] != NULL; i++){
            free(array[i]);
        }
        // free the array itself
        free(array);
    }
}

/* Counts the non-null items in counters */
static void itemcount(void *arg, int key, int count)
{
  int *nitems = arg;

  if (nitems != NULL)
    (*nitems)++;
}

/* Gets the integer file descriptor associated with the stream pointed to by stream */
int fileno(FILE *stream);


/*  Prints a prompt if stdin is a tty (terminal) */
static void prompt(void)
{
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}