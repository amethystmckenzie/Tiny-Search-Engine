# TSE Indexer Design Spec

Recall the [Indexer Requirements Spec](REQUIREMENTS.md); the **indexer** reads the document files produced by the TSE crawler, builds an index, and writes that index to a file.

The indexer parses webpages in a Crawler-produced directory and extracts words (longer than 2 characters), keeping track of where the word was found (docID), and how many times it appeared (count) within each docID. When the indexing process is complete, the querier can be developed using the index.

Contents:
* User interface
* Inputs and Outputs
* Functional decomposition into modules
* Pseudo code for logic/algorithmic flow
* Dataflow through modules
* Major data structures
* Testing plan

### User interface

The indexer's's only interface with the user is on the command-line; it must always have two arguments:

```
indexer pageDirectory indexFilename
```

where `pageDirectory` is a directory of files produced by `crawler`, and `indexFilename` is a writable file (it doesn't have to exist first).

For example: 

``` bash
$ ./indexer ../crawler/letters-3 index-letters-3
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: Save the indexed webpages to a file

Within a file, we write
 * The found word
 * The docID
 * The count of the word at in that specific docID
 
 Thus, the file obtains the format `word docID count [docID count]...` for as many times the word appears in a certain docID.
 
 Each word is printed on a new line, where the format repeats. 

 The file is sorted using the bash command `sort` during testing so that the words appear in alphabetical order.

See REQUIREMENTS spec for more details about formatting.

### Functional decomposition into modules

Here are the following modules and functions:

Indexer:
 1. *main* which gets parsed arguments, builds, and saves the index.
 2. *parse_arguments* which parses arguments from the command-line and validates parameters
 3. *index_build*, which loops over all webpages in the crawler directory and indexes them, building an index.
 4. *index_page*, which maps the words found in each webpage to their docID and count.

And some imported modules that provide data structures and other functions:
 1. *index* which maps found words of valid length to their respective docID and count
 2. *counters* which maps docIDs to counts, as well as initialising and adding to the count for each word
 3. *load_page*, which converts files in `pageDirectory` back into webpages
 4. *index_save*, which outputs the index to the appropriate file

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:
1. Execute from a command line as shown in the User Interface
2. Parse the command line and validate parameters
3. Build the index by loading data from `pageDirectory`
4. Save the index to an external file `indexFilename`
5. Clean up data structures

The indexer completes and exits when it has no more files to index.

### Dataflow through modules

Indexer:
 1. *main* parses parameters and calls *index_build* to build the index from the `pageDirectory`  and calls *index_save* to save the index to `indexFilename`.
 2. *index_build* builds the index by looping through all the converted webpages from `pageDirectory` (using *load_page*) and calls *index_page* on each webpage.
 3. *index_page* goes through every word in a webpage and maps it to a docID and count of appearances.
 4. *load_page* converts eeach file in `pageDirectory` to a webpage by scanning and parsing through the file and creating a new webpage based on the parsed contents.
 5. *index_save* outputs the built index to `indexFilename` by iterating through the entire index and printing it to the file.

### Major data structures

Two helper modules provide data structures:
 1. *index* that map words found in webpages to their docIDs and count.
 2. *counters* of (docID, count) pairs that keep track of how many times a word appear within a docID.

### Testing plan

*Unit testing*. A small test program to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the indexer and test it as a whole.
In each case, examine the output files carefully to be sure they have the the right contents. Compare them with the example outputs found in `/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/` to ensure correctness.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

1. Test the indexer with a reasonably sized Crawler-produced directory 
Sort the file produced by `indexer` as well as the associated example output and compare them to check that they are identical.

2. Repeat Test 1 with other Crawler-produced directories of various sizes and links.