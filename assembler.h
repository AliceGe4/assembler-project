#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H
#include "globals.h"
#include "hashtable.h"

typedef struct{
	BITS_STRUCT memory_struct[MEMORY_SIZE]; /*Struct to store symbols*/
	REGISTERS registers;
	int is_label;
	int DC, IC;
	HASHTABLE* label_hash;
}MEMORY;
/**
* first_pass the function that handles the first pass
* @param memory a pointer to our memory array
* @param f our file that is being currently read from after pre-assembler
* @return returns true if there were no errors
*/
boolean first_pass(MEMORY* memory, FILE* f);
/**
* second_pass the main function that runs the second pass
* @param f_origin the original file after the pre-assembler
* @param f_object the object file to have our objects printed into
* @param f_extern the extern file to have our extern data printed into
* @param f_entry the entry file to have our entry data printed into
* @return returns true if second pass was sucssesful
*/
boolean second_pass(MEMORY* memory, FILE* f_origin, FILE* f_object, FILE* f_extern, FILE* f_entry);
/**
* initiate_memory a function that initiates memroy for our memory table
* @param memory a pointer to our memory array
*/
void initiate_memory(MEMORY* memory);
/**
* free_memory a function that frees memroy from our memory table, used to clean up failed files.
* @param memory a pointer to our memory array
*/
void free_memory(MEMORY* memory);
#endif
