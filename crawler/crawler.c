/* 
*
* crawler.c - crawls a website and retrieves webpages starting with a specified URL.
*
* input: command-line parameters
* output: we save each explored webpage to a file, one file per page.
*
* usage: ./crawler seedURL pageDirectory maxDepth
* 
*
* Amethyst McKenzie, April 29 2022, cs50
*/

#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "memory.h"
#include "pagedir.h"

/* function declarations */
static void crawler(char *url, char *pageDir, int maxDepth);
static void page_fetcher(webpage_t *webpage);
static void page_scanner(webpage_t *webpage, bag_t *bag, hashtable_t *hash);
inline static void logr(const char *word, const int depth, const char *url);
static void itemdelete(void *item);

/* Purpose: Parse arguments from command-line and call crawler
 * Parameters: c = number of arguments, argv= argument
 * Return: exit code (0 if ran successfully, non-0 if error)
 */
int main(const int c, char* argv[])
{

    char *seedURL = argv[1];
    char *pageDirectory = argv[2];
    int maxDepth;

    char excess;
    int MAX = 10; // max possible value for maxDepth
    int MIN = 0; // min possible value for maxDepth

    // check if wrong number of arguments
    if(c != 4){
        fprintf(stderr, "Wrong number of arguments. Usage: ./crawler seedURL pageDirectory maxDepth\n");
        return 1;
    }

    // check if maxDepth is a valid integer
    if((sscanf(argv[3], "%d%c", &maxDepth, &excess)) != 1) {
        fprintf(stderr, "maxDepth must be an integer.\n");
        return 2;
    }

    // check if maxDepth is within the appropiate range
    if(maxDepth > MAX || maxDepth < MIN){
        fprintf(stderr, "maxDepth %d out of range. maxDepth must be within the range [%d,%d]\n", maxDepth, MIN, MAX);
        return 3;
    }

    // check if the pageDirectory exists and is writable
    if(!valid_directory(pageDirectory)){
        fprintf(stderr, "Error. %s not a valid directory\n", pageDirectory);
        return 4;
    }

    // check and normalise and URL
    if(!NormalizeURL(seedURL)){
        fprintf(stderr, "%s cannot be normalised\n", seedURL);
        return 5;
    }

    // check if URL is an internal link
    if(!IsInternalURL(seedURL)){
        fprintf(stderr, "%s not an internal URL\n", seedURL);
        return 6;
    }

    // run crawler
    crawler(seedURL, pageDirectory, maxDepth);

    return 0;

}

/* Purpose: Crawl a website 
 * Parameters: url=seedURL, pageDir=directory page will save to, maxDepth=maximum depth to crawl until
 * Return: No return (exits if error with memory allocation)
 */
static void crawler(char *url, char *pageDir, int maxDepth)
{
    int id = 1; // document ID

    // initialising data structures
    bag_t *pages;
    hashtable_t *visitedURLS;

    const int SIZE = 200;
    pages = assertp(bag_new(), "bag_new");
    visitedURLS = assertp(hashtable_new(SIZE), "hashtable_new");

    // allocating space for url and copying it
    char* urlcopy = assertp(malloc(strlen(url)+1), "url copy");
    strcpy(urlcopy, url);

    // creating webpage with depth 0
    webpage_t *webpage = assertp(webpage_new(urlcopy, 0, NULL), "webpage_new");

    bag_insert(pages, webpage); // inserting web page into bag (to be crawled)
    hashtable_insert(visitedURLS, urlcopy, ""); // this url has been visited 

    webpage_t *poppedPage; 
    // while there are still webpages to crawl
    while((poppedPage = bag_extract(pages)) != NULL){
        page_fetcher(poppedPage); // get HTML if fetchable
        // if the HTML was fetched (no longer NULL)
        if(webpage_getHTML(poppedPage) != NULL){
            page_saver(poppedPage, pageDir, id); // write webpage to pageDir with document ID as filename
            id += 1; // increment id number 
        
            // if depth has not exceeded the maxDepth, scan for more URLs
            if(webpage_getDepth(poppedPage) < maxDepth){
                page_scanner(poppedPage, pages, visitedURLS);
            
            }
        }

        // delete webpage
        webpage_delete(poppedPage);
    }

    // clean up
    bag_delete(pages, itemdelete);
    hashtable_delete(visitedURLS, NULL);
}

/* Purpose: Fetches a webpagae
 * Parameters: webpage=one of the webpages in the bag
 * Return: No return
 */
static void page_fetcher(webpage_t *webpage){
    if(webpage_fetch(webpage)){
        logr("Fetched", webpage_getDepth(webpage), webpage_getURL(webpage));
    }
    else {
        // failed to fetch the page
        logr("Failed to fetch", webpage_getDepth(webpage), webpage_getURL(webpage));
    }
}


/* Purpose: Scans page for links and retrieves all urls found in a page
 * Parameters: webpage=one of the webpages in the bag, bag=webpages to be searched, hash=webpages visited
 * Return: No return (exits if error with memory allocation)
 */
static void page_scanner(webpage_t *webpage, bag_t *bag, hashtable_t *hash)
{

    assertp(webpage, "page_scan page==NULL");
    assertp(bag, "page_scan bag==NULL");
    assertp(hash, "page_scan hash==NULL");

    char *result = NULL;
    int pos = 0; // current position in HTML buffer

    logr("Scanning", webpage_getDepth(webpage), webpage_getURL(webpage));
    while ((result = webpage_getNextURL(webpage, &pos)) != NULL) {
        logr("Found", webpage_getDepth(webpage), result);

        if(!IsInternalURL(result)){
            logr("IgnExtrn", webpage_getDepth(webpage), result); // ignore external link
            free(result);
            continue;
        }

        // if the link isn't unique (alreadu in hashtable and did not insert)
        if(!hashtable_insert(hash, result, "")){
            logr("IgnDupl", webpage_getDepth(webpage), result); // ignore duplicate link
            free(result);
            continue;
        }

        if(NormalizeURL(result)){
            webpage_t *page = assertp(webpage_new(result, webpage_getDepth(webpage)+1, NULL), "page_scan webpage_new");
            bag_insert(bag, page); // add to webpages to visit
            logr("Added", webpage_getDepth(webpage), webpage_getURL(page));
        
        }
    }
}


/* log one word (1-9 chars) about a given url */
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}


/* delete an item */
static void itemdelete(void *item)
{
  if (item != NULL) {
    free(item);   
  }
}
