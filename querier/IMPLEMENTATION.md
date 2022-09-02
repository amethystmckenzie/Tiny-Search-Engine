# TSE Querier Implementation Spec
## Amethyst McKenzie CS50 Spring 2022

### General Implemenation
I implemented querier by following the pseudocode detailed in the DESIGN spec. 

The *querier* reads an index file produced by TSE Indexer, `indexFilename`, and a page directory produced by the TSE Crawler, `pageDirectory`, and answers search queries submitted via stdin.

It parses the inputted query, validates the input, converts it into an array, and tries to find any documents that match those words. Additionally, it uses "and"/"or" operators that determine which documents will be returned to the user. 

The user inputs the `pageDirectory` and `indexFilename` via command-line arguments. 

### Functions and Modules

I implemented *querier* using the following functions and modules:

#### Query
 1. *main* which gets parsed arguments, loops through the stdin, and runs querier
 2. *parse_arguments* which parses arguments from the command-line and validates parameters
 3. *parse_query* which parses the stdin and stores each word into an array
 4. *query_print* which loops through the array and prints the query back to the user
 5. *valid_query* which validates the query and ensures it is in the proper format
 6. *valid_character* which checks if there are any invalid characters in the stdin

```c
static void parse_arguments(const int c, char *argv[], char **pageDirectory, char **indexFilename);
static void query_print(char **array);
static char **parse_query(char *line);
static bool valid_query(char **array);
static bool valid_character(char *line);
```

#### Merging and Intersecting Counters
7. *counters_intersect* which intersects two counters, essentially finding the set of documents that match all given words. This is used for "and" operations or when there are only spaces that separate words. This function was provided and created by Professor Zhou.
8. *intersect_helper* which checks if the document exists in the other counters set and updates the first counters set accordingly. This function was provided and created by Professor Zhou.
9. *counters_union* which merges two counters, essetinaly finding the set of documents that match either of the words given. This is used for "or" operations.
10. *counters_union_helper* which checks if the document exists, and updates the count/score accordingly.
11. *itemcount* which counts all the non-null items in a counters data structure.

```c
static void counters_intersect(counters_t* ct1, counters_t* ct2); 
static void intersect_helper(void *arg, const int key, const int count);
static void counters_union(counters_t* ct1, counters_t* ct2);
static void counters_union_helper(void *arg, const int key, int count);
static void itemcount(void *arg, int key, int count);
```

#### Counters -> Array
12. *create_s_array* which creates an array of struct counternodes from the counters docID and score, sorted in decreasing order by score.
13. *fill_array* which is a helper function for *create_s_array* that fills the array with counternode structs
14. *insertion_sort* which sorts the array of structs in decreasing order by score
15. *array_delete* which loops through the entire array and frees each item, then freeing the array itself.

```c
static struct counternode **create_s_array(counters_t *ct);
static void fill_array(void *arg, const int key, int count);
static void insertion_sort(struct counternode **array, int n);
static void array_delete(struct counternode **array);
```

#### Querier

16. *querier* which merges or intersects counters based on "and"/"or" operators
17. *found_documents* which is a helper function for *main* that finds the number of documents in the sorted array.

```c
static counters_t *querier(char **query, char *indexFilename);
static void found_documents(struct counternode **sorted_array);
```

#### Prompt User
18. *prompt* which is a function provided and created by Professor Zhou that only prompts the user if stdin is the terminal.
19. *fileno* which is a function provided by the *<unistd.h>* library that gets the integer file descriptor associated with the stream pointed to by stream

```c
static void prompt(void);
int fileno(FILE *stream);
```

#### Other
20. *min* which is a function provided and created by Professor Zhou that calculates the minimum 
```c
static inline int min(const int a, const int b)
```

### Data Structures and Interfaces
*index* of words mapped to their docID location and count
*counters* of (docID, count) pairs
A struct *twocts* that contains two *counters* data structures
A struct *counternode* that contains a docID and count variable.

```c
struct twocts {
	counters_t *result;
	counters_t *another;
};

struct counternode {
    int docID;
    int score;
};
```

`querier.c` implements the following functions through index.h, webpage.h, pagedir.h, word.h, memory.h and file.h:

index.h
```c
void *index_find(index_t *index, const char *key);
void *index_find(index_t *index, const char *key);
void index_delete(index_t *index, void (*itemdelete)(void *item));
```

webpage.h
```c
char *webpage_getURL(const webpage_t *page);
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
```

*load_page* converts files in `pageDirectory` back into webpages
*crawler_directory* checks if `pageDirectory` is produced by Crawler
*normalise_word* normalises a word (sets word to lowercase)
*index_load* loads an index file produced by TSE Indexer back into an index data structure
*index_find* which returns the item associated with the given key in an index.

### Pseduocode

1. main
- Call *parse_arguments* to parse through command-line arguments and validate parameters.
- Prompt the user and flush (to clean/flush the output buffer)
- While you keep getting a question from the user
    - validate the characters (*valid_character*)
        - parse query and store it in an array
        - validate the query (*valid_query*)
            - Print the parsed, clean, validated query
            - Run querier (*querier*) and store it in a counters data structure
            - Create the sorted array (*create_s_array*)
            - Find all the documents outputted back to the user (*found_documents*)
            - For every struct in the sorted array
                - load a webpage (*load_page)
                - print the score, docID, and URL
                - delete webpage (*webpage_delete*)
            - Delete array and counters
        - free query
    - free line
    - reprompt
    - flush

2. parse_arguments
- Parse command-line arguments
- Check if command-line arguments are valid
- Exit non-zero if any errors

3. parse_query
- Initialise variables
- Allocate space to store the query and intialise a variable for its length
- Tokenise the string by:
    - Finding the beginning of each word (char *word) 
        - Find the end of the word (where there's a space) (char *rest)
    - Chop the string by setting *rest to '\0'
    - Increment rest to move to next position
    - If the word is not '\0'
        - normalise it (*normalize_word*)
        - add it to the array
        - increment the length of the array

4. valid_query
- Initialise boolean
- Parse through the query and check if the first element is equal to an operator
    - If it does, print out an error message and return false
- Loop through the query, parse through it, and check if the last element is equal to an operator
    - If it does, print out an error message and return false
- Loop through the query and check if a word equals an operator
    - If it does, set a boolean to true
    - Else, set it to false
    - If we haven't reached the end of the query
        - Check if the next word is an operator and that the boolean is true
            - If it is, we know that two operators are next to each other, so print out an error message and return false
- Return true

5. valid_character
- Initialise integer
- For every character in the stdin
    - If alpha, continue
    - If space, incrementer integer, and continue
    - Else, print out an error message and return false
- If the integer equals the length of the line
    - Return false, because that means the query is just empty spaces
- Return true

6. query_print
- For every element in the query, print.

7. normalise_word (which is in the common directory)
- For every character in the word
    - lowercase the character

8. counters_intersect
- Use the struct *twoctrs* to store two counters
- Iterate through the counters and set the score for each document (which is the minimum of the count for each document)

9. counters_union
- Iterate through both counters
- Check if the document exists and update the count accordingly

10. create_s_array
- Iterate through the counter to calculate the number of elements in the counters set
- Allocate the appropiate space to the struct *counternode*
- Iterate through the counters again and fill the array
- Sort the array in decreasing order

11. fill_array
- Initialise variables
- While the array isn't empty, increment the length variable
- If the document has a score greater than 0
    - Allocate the appropiate size for a counternode and fill it
    - Add to the array

12. insertion_sort
- Initialise *counternode*
- For every element in the array
- If the element is NULL, break
    - Set *counternode* equal to the element
    - Move elements that are less than the counter score to one position ahead of their current position

13. array_delete
- If the array exists
    - For every element in the array
        - Free it
    - Free the array

14. found_documents
- Initialise an integer (ndocs)
- Loop through the sorted array
- Increment ndocs
- If ndcos is equal to 0
    - Print that no documents were found
- Else, print how many matched documents were found

15. crawler_directory (which is in the common directory)
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

For any invalid characters, an error message is printed back to the user and they are re-prompted.

Likewise, for any invalid queries, an error message is printed back to the user and they are re-prompted. 

### Testing

The `querier.c` program reads hardcoded data and follows the testing procedure outlined in the DESIGN spec

Various functionality tests are conducted:

Integration Testing:
* TEST 0-2: Invalid Command-Line Arguments
* TEST 3-8: Invalid Inputs
* TEST 9: No Documents Match
* TEST 10-12: fuzzquery testing

Fuzzquery is a program provided by cs50 that generates random queries to test the functionality of `querier.c` more extensively.

- Tests the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

- Tests the querier with a reasonably sized TSE Crawler produced directory and TSE Indexer produced indexfile using fuzzquery. 

- Repeats Test 1 with other TSE Crawler produced directories and TSE Indexer produced index files of various sizes. 

Unit Testing:
- I tested these functions individually before proceeding to next steps:

*parse_query*
Where I ensured that when I printed out the parsed query that the entire line was normalised and no invalid characters were present.

*valid_query and valid_character*
Making sure that the program was only taking in valid queries of the appropiate format.

*querier*
Where I checked with the associated index file that my querier was producing the right counters, and that and/or operations were working correctly

Regression Testing:
- I didn't do any regression testing.