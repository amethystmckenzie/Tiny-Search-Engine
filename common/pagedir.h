/*  
 * pagedir - header file for CS50 pagedir
 * 
 * 
 * Amethyst McKenzie, May 9 2022, cs50
 */

#include <stdbool.h>
#include "webpage.h"

/**************** valid_directory ****************/
/* 
 * Determine if the pathName provided as pageDirectory is a directory, and is writable
 * Returns NULL if the directory does not exist or is not writable
 * Else, returns non-NULL and closes the file
 * Exits if error with memory allocation
 * 
 */
bool valid_directory(char *directoryName);

/**************** page_saver ****************/
/* 
 * Saves a page to a directory, with each file named after a unique document ID
 * Exits if error with memory allocation
 * 
 */
void page_saver(webpage_t* page, char* directoryname, int id);

/**************** crawler_directory ****************/
/* 
 * Verifies if directory is a Crawler-produced directory
 *
 * Returns:
 *  true if directory was produced by Crawler
 *  false if directory was not produced by Crawler
 */
bool crawler_directory(char *dir);

/**************** load_page ****************/
/* 
 * Creates a webpage from files in the directory
 *
 * Returns:
 *  NULL if directory is NULL or any parameters are NULL
 *  A webpage if file was read successfully, and a new webpage was created succesfully
 * 
 * Exits if error with memory allocation 
 */
webpage_t *load_page(char *pageDirectory, int id);