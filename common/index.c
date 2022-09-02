/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Amethyst McKenzie, May 9 2022, cs50
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "hashtable.h"
#include "counters.h"
#include "file.h"

/**************** local functions ****************/
/* not visible outside this file */
static void save_item(void *arg, const char *key, void *item);
static void save_count(void *arg, const int id, int count);
static void ct_delete(void *item);

/**************** index_new() ****************/
/* see index.h for description */
index_t *index_new(const int num_slots)
{
    return hashtable_new(num_slots);
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t *index, const char *key, counters_t *counter)
{
    return hashtable_insert(index, key, counter);

}

/**************** index_find() ****************/
/* see index.h for description */
void *index_find(index_t *index, const char *key)
{
    return hashtable_find(index, key);

}

/**************** index_load() ****************/
/* see index.h for description */
index_t *index_load(char* index_file)
{   
    // initialise
    FILE *fp = fopen(index_file, "r");
    index_t *index = index_new(lines_in_file(fp));

    // check variables
    if(index == NULL || index_file == NULL){
        return NULL;
    }

    if(fp == NULL){
        fprintf(stderr, "failed to open %s\n", index_file);
        return NULL;
    }
    
    char *word;
    // get each word in the file
    while((word = freadwordp(fp)) != NULL){
        int id;
        int count;
        counters_t *c = counters_new();

        if(c == NULL){
            return NULL;
        }

        // check format and set count
        while((fscanf(fp, "%d %d", &id, &count)) > 1){
            counters_set(c, id, count); 
        }

        index_insert(index, word, c); // map word and counters data structure (which maps id and count) by inserting into index
        free(word);
    }

    fclose(fp);
    return index;
}

/**************** index_save() ****************/
/* see index.h for description */
bool index_save(index_t* index, char* index_file)
{   
    // check variables
    if(index == NULL || index_file == NULL){
        return false;
    }

    // check if file can be written to
    FILE *fp = fopen(index_file, "w");
    if(fp == NULL){
        fprintf(stderr, "index_save failed to open %s for writing\n", index_file);
        return false;
    }

    // iterate through the index
    hashtable_iterate(index, fp, save_item);

    // clean up
    fclose(fp);

    return true;
}

/**************** save_item() ****************/
/* helper function for index_save */
static void save_item(void *arg, const char *key, void *item)
{   
    // initialise
    FILE *fp = arg;
    counters_t *c = item;

    // print key, iterate through the count
    fprintf(fp, "%s", key);
    counters_iterate(c, fp, save_count);
    fprintf(fp, "\n");
}

/**************** save_count() ****************/
/* helper function for save_item */
static void save_count(void *arg, const int id, int count)
{
    FILE *fp = arg; // initialise
    fprintf(fp, " %d %d", id, count); // print id and count
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t *index, void (*itemdelete)(void *item))
{   if(index != NULL){
        hashtable_delete(index, ct_delete);
    }
}

/******** delete counters item ********/
/* helper function for index_delete */
static void ct_delete(void *item)
{   
    // cast item to counters type
	counters_t* ct = item;

    // delete
	if (ct != NULL) {
		counters_delete(ct);
	}
}
