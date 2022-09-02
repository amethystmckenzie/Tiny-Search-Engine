/* 
 * indextest.c - testing file for 'index' module
 *
 * usage: ./indextest oldIndexFilename newIndexFilename
 * where `oldIndexFilename` is the name of a file produced by the indexer
 * where `newIndexFilename` is the name of a file into which the index should be written.
 * 
 * Amethyst McKenzie, May 9 2022, cs50
 */

#include <stdio.h>
#include "index.h"
#include "memory.h"
#include "file.h"

int main(int c, char* argv[])
{
    // check for valid number of arguments
    if(c != 3){
        fprintf(stderr, "Wrong number of arguments. Usage: ./indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }

    // create files and filenames
    char *oldIndexFilename = argv[1];
    char *newIndexFilename = argv[2];
    
    FILE *oldIndex = fopen(oldIndexFilename, "r");
    assertp(oldIndex, "index test reading oldIndex");

    // load index from indexer file
    index_t *index = index_load(oldIndexFilename);

    FILE *newIndex = fopen(newIndexFilename, "w");
    assertp(newIndex, "index test writing newIndex");

    // save loaded index to a new file
    index_save(index, newIndexFilename);

    // clean up
    index_delete(index, NULL);
    fclose(oldIndex);
    fclose(newIndex);

    return 0;
}


