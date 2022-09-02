/* 
*
* pagedir.c - Handles shared code related to pageDirectory and files
* 
*
* Amethyst McKenzie, May 9 2022, cs50
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "memory.h"
#include "webpage.h"
#include "file.h"

// file created with each new crawler directory
char *crawlerFile = ".crawler";

/**************** valid_directory() ****************/
/* see pagedir.h for description */
bool valid_directory(char *directoryname)
{   
    // check parameter
    if(directoryname == NULL){
        return false;
    }

    // create filename
    int fnameLen = strlen(directoryname) + strlen(crawlerFile) + 2;
    char fname[fnameLen];
    sprintf(fname, "%s/%s", directoryname, crawlerFile);

    // open file for writing
    FILE *file = fopen(fname, "w");

    // if file could not be opened or written to, directory doesn't exit/is invalid
    if(file == NULL){
        return false;
    }

    // clean up
    fclose(file);

    return true;
}

/**************** page_saver() ****************/
/* see pagedir.h for description */
void page_saver(webpage_t* page, char* directoryname, int id)
{   
    // check parameters
    if(page == NULL){
        assertp(NULL, "pagedir_save gets NULL page");
    }

    if(directoryname == NULL){
        assertp(NULL, "pagedir_save gets NULL directory");
    }

    assertp(webpage_getURL(page), "pagedir_save gets NULL url");
    assertp(webpage_getHTML(page), "pagedir_save gets NULL html");

    // create filename
    int fnameLen = strlen(directoryname)+12;
    char fname[fnameLen];

    // create filename with proper format and unique document id
    sprintf(fname, "%s/%d", directoryname, id);

    // write contents of the page to file (if possible)
	FILE *fp = fopen(fname, "w");
	assertp(fp, "cannot open file for writing\n");
	fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
    
    // clean up
	fclose(fp);	
    
}

/**************** crawler_directory() ****************/
/* see pagedir.h for description */
bool crawler_directory(char *dir)
{   
    if(dir == NULL){
        return false;
    }

    // check if .crawler file is in directory
    int fnameLen = strlen(dir)+1 + strlen(crawlerFile);
    char fname[fnameLen];
    sprintf(fname, "%s/%s", dir, crawlerFile);

    FILE *file = fopen(fname, "r");

    // if file could not be opened for reading, directory was not produced by crawler
    if(file == NULL){
        return false;
    }

    // clean up
    fclose(file);
    return true;

}

/**************** load_page() ****************/
/* see pagedir.h for description */
webpage_t *load_page(char *pageDirectory, int id)
{   
    // check parameters
    if(pageDirectory == NULL){
        return NULL;
    }

    // initialise
    FILE *fp;
    webpage_t *webpage;

    // create filename with proper format and unique document id
    int fnameLen = strlen(pageDirectory)+12;
    char fname[fnameLen];
    sprintf(fname, "%s/%d", pageDirectory, id);

    // check if file can be opened for reading
    if((fp = fopen(fname, "r")) == NULL){
        return NULL;
    }

    char *url = freadlinep(fp); // get the url
    char *c_depth = freadlinep(fp); // get depth
    char *html = freadfilep(fp); // get html

    // convert depth to integer
    int depth = sscanf(c_depth, "%d", &depth);
    free(c_depth); // free the character depth
    
    // create a webpage based on found data
    webpage = assertp(webpage_new(url, depth, html), "error with pagedir webpage_new");

    fclose(fp);
    return webpage;
}
