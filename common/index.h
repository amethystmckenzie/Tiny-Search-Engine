/* 
 * index.h - header file for CS50 index module
 *
 * An *index* is a set of (key, counter) pairs. It acts similiar to a hashtable, 
 * with additional functions/features.
 *
 * Amethyst McKenzie, May 9 2022, cs50
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include "counters.h"
#include "hashtable.h"

/**************** global types ****************/
typedef hashtable_t index_t;  // opaque to users of the module

/**************** APIs ****************/

/**************** index_new ****************/
/* Create a new (empty) index table.
 *
 * Caller provides:
 *   number of slots to be used for the index (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t *index_new(const int num_slots);

/**************** index_insert ****************/
/* Insert counters item, identified by key (string), into the given index table.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key, valid pointer for counter.
 * We return:
 *   false if key exists in index, any parameter is NULL, or error;
 *   true if new item was inserted.
 * Notes:
 *   The key string is copied for use by the index; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool index_insert(index_t *index, const char *key, counters_t *counter);

/**************** index_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if index is NULL, key is NULL, or key is not found.
 * Notes:
 *   the index is unchanged by this operation.
 */
void *index_find(index_t *index, const char *key);

/**************** index_load ****************/
/* Turns a file into an index
 *
 * Caller provides:
 *  valid pointer to file name
 * We return:
 *  pointer to an index table
 *  NULL if index is NULL, index_file or fp is NULL 
 */
index_t *index_load(char* index_file);

/**************** index_save ****************/
/*
 * Saves the index table to a file
 *
 * Caller provides:
 *  valid pointer to index, valid pointer to file name
 * We return:
 * false if index table couldn't be saved to the file or any parameters are NULL
 * true if index table was saved to the file successfully
 */
bool index_save(index_t* index, char* index_file);

/**************** index_delete ****************/
/* Delete index, calling a delete function on each counter.
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if index==NULL, do nothing.
 *   call the itemdelete on each item.
 *   free all the key strings, and the index itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in index_insert.
 */
void index_delete(index_t *index, void (*itemdelete)(void *item));

#endif  // __INDEX_H