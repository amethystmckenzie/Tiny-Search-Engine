/* 
 * word.c - Normalises a word
 *
 *
 * Amethyst McKenzie, May 9 2022, cs50
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**************** NormalizeWord() ****************/
/* see word.h for description */
void normalize_word(char *word)
{   
    char c;
    // loop through word and convert each letter to lowercase
    for(int i = 0; i < strlen(word); i++){
        c = word[i];
        word[i] = tolower(c);
    }
}

