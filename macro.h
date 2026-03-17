#ifndef _MACROS_H
#define _MACROS_H
#define MAX_MACRO_NAME 80
#define MCR_SPACE "mcr "
#define MCR_END "endmcr"
#include <stdio.h>
#include "hashtable.h"
#include "globals.h"

typedef struct {
    char macro_line[MAX_LINE_CONTENT];
}MACRO_CONTENT;

typedef struct {
    LINE* macro_contents;
    int content_size;
}MACRO_DATA;
/**In this struct we have macro_name and the struct that contains all macro details
  * macro_details contains an array of structs and its size(Each struct contains char with a constant size that is there to store lines of the macro.)
  */
typedef struct {
    char* macro_name;
    MACRO_DATA* macro_details;

}MACRO;
/** 
* compares to macros m1 and m2
* @param m1 The first macro
* @param m2 The second macro
* @return Returns an int using strcmp depending if they were the same
*/
int compare_macro(void* m1, void* m2);
/**
* inserts the macro into the hashtable
* @param hash a pointer to the macro Hashtable
* @param macro a pointer to the macro
* @return returns whether insert item hash managed to insert the macro into the hash
*/
int insert_macro(HASHTABLE* hash, MACRO* macro);
/**
* inserts the line into the macro
* @param hash a pointer to the macro Hashtable
* @param line a constant char of a line that we want to insert into the macro
*/
void insert_line_macro(MACRO* macro, const char* line);
/**
* iniiates a macro into hashtable
* @param hash a pointer to the macro Hashtable
* @param macro_name the name of the macro
*/
void initiate_macro(MACRO* macro, const char* macro_name);
/**
*checks to see if its a start of mcr
* @param str_line a line in a string to see if its a mcr command line
* @return returns a NULL if mcr wasn't discovered, if it was it is noted and returns a skip to next blank function in order to find the macro name.
*/
char* is_mcr(LINE str_line);
/**
* Sets macro into the hash table
* @param analyze_macro the line we want to be analyzed to set into the hash
* @param macro_hash a pointer to our HASHTABLE
*/
void set_macro(char* analyze_macro, FILE* f, HASHTABLE* macro_hash); 
/**
* Frees the macro key
* @param macro_key the key of the macro
*/
void free_macro_key(void* macro_key);
/**
* Frees the macro data
* @param macro_details the details inside of the data to be freed.
*/
void free_macro_data(void* macro_details);
#endif
