# TSE Querier Design Spec

The **querier** reads an index produced by the TSE Indexer and outputs a list of documents in decreasing order from a crawler-produced page directory that answers queriers provided by the user using standard input (stdin). 

The **querier** uses "and" / "or" operators that dictate the documents fed back to the user. If stdin sees an "and", a webpage must contain all the words in that sequence until an "or" is found, or EOF. Alternatively, if stdin sees an "or", a webpage must only contain that one word, it doesn't matter what the subsequent or previous words in the query were. 

**querier** parses through the stdin and prints out a clean query to the user, and uses this clean array to look for words and key operators. 

Contents:
* User interface
* Inputs and Outputs
* Functional decomposition into modules
* Pseudo code for logic/algorithmic flow
* Dataflow through modules
* Major data structures
* Testing plan

### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments:

```
querier pageDirectory indexFilename
```

where `pageDirectory` is a directory of files produced by `crawler`, and `indexFilename` is a readable file (assume it has been produced by TSE Indexer).

For example: 

``` bash
$ ./querier ../crawler/letters-6 index-letters-6
```

### Inputs and outputs

Input: The inputs come from two places: the command-line parameters (see the User Interface above) and stdin. Once querier is run with the appropiate command-lines, the user will be prompted queries until the user exits the program (by doing ^D). 

Output: A "clean" version of the query and a list of webpage URLS with their score, docID, and webpage. The output is sorted in decreasing order by score. Each query is printed on a new line.

Example:
```
Query: computational or biology or coding the and kelp or home 
Matches 9 documents (ranked):
score	3 doc	9: http://cs50tse.cs.dartmouth.edu/tse/letters/C.html
score	2 doc	1: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
score	1 doc	2: http://cs50tse.cs.dartmouth.edu/tse/letters/A.html
score	1 doc	3: http://cs50tse.cs.dartmouth.edu/tse/letters/B.html
score	1 doc	4: http://cs50tse.cs.dartmouth.edu/tse/letters/E.html
score	1 doc	5: http://cs50tse.cs.dartmouth.edu/tse/letters/G.html
score	1 doc	6: http://cs50tse.cs.dartmouth.edu/tse/letters/H.html
score	1 doc	7: http://cs50tse.cs.dartmouth.edu/tse/letters/F.html
score	1 doc	8: http://cs50tse.cs.dartmouth.edu/tse/letters/D.html
```

The program does not permit invalid characters (non-alpha, except for spaces), and will print out an error message if it sees one. Similarily, the query is printed back to the user after being parsed and validated (not starting with a key operator, nor two operators being next to each other). 

See REQUIREMENTS spec for more details about formatting and validating.

### Functional decomposition into modules

Here are the following modules and functions:

 1. *main* which gets parsed arguments, loops through the stdin, and runs querier
 2. *parse_arguments* which parses arguments from the command-line and validates parameters
 3. *parse_query*, which parses the stdin and stores each word into an array
 4. *valid_query* which validates the query and makes sure it is of proper format
 5. *querier*, which merges and intersects counters based on "and"/"or" operators and returns a final counter that stores the docID and updated count of all found words
 6. *create_s_array* which creates an array of sorted (decreasing order by score) struct `counternodes` that each store a `docID` and `score`. 

And some imported modules that provide data structures and other functions:
 1. *index* which maps found words of valid length to their respective docID and count
 2. *counters* which maps docIDs to counts, as well as initialising and adding to the count for each word
 3. *load_page*, which converts files in `pageDirectory` back into webpages
 4. *index_load*, which loads an index from a TSE Indexer produced file. 

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:
1. Execute from a command line as shown in the User Interface
2. Parse the command line and validate parameters
3. Get (and validate) stdin from the user
4. Parse the stdin and store each inputted word into an array
5. Validate the query
6. Run querier on the clean, validated query
7. Store each result from querier into a sorted array of struct `counternode`
8. Print the array back to the user
9. Clean up data structures

The querier completes when the user enters ^D, which exits the program. 

### Dataflow through modules

 1. *main* which gets parsed arguments, loops through the stdin, checks for invalid characters, validates the query, runs querier, builds the sorted array of structs, prints out the results back to the user. 
 2. *parse_arguments* which parses arguments from the command-line and validates parameters
 3. *parse_query*, which parses the stdin and stores each word into an array
 4. *valid_query* which validates the query and makes sure it is of proper format
 5. *querier*, which merges and intersects counters based on "and"/"or" operators and returns a final counter that stores the docID and updated count of all found words
 6. *create_s_array* which creates an array of sorted (decreasing order by score) struct `counternodes` that each store a `docID` and `score`. 

### Major data structures

Two helper modules provide data structures:
 1. *index* that map words found in webpages to their docIDs and count.
 2. *counters* of (docID, count) pairs that keep track of how many times a word appear within a docID.

### Testing plan

*Unit testing*. A small test program to test each module to make sure it does what it's supposed to do.

*Integration testing*. Assemble the querier and test it as a whole.
In each case, examine the output files carefully to be sure they have the the right contents. Compare them with the example outputs found in `/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/` to ensure correctness.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

1. Test the querier with a reasonably sized TSE Crawler produced directory and TSE Indexer produced indexfile. 

2. Repeat Test 1 with other TSE Crawler produced directories and TSE Indexer produced index files of various sizes. 