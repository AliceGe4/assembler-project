#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "macro.h"

/*compares two macros in order to find the macro we request in the hash table.*/
int compare_macro(void* m1, void* m2) {
    char* first_macro = (char*)m1;
    char* second_macro = (char*)m2;
    return strcmp(first_macro, second_macro);
}
/*This functions tries to insert macro into the hashtable.*/
int insert_macro(HASHTABLE* hash, MACRO* macro) {
    return insert_item_hash(hash, macro->macro_name, macro->macro_details, compare_macro);
}
/*Initiates the macro*/
void initiate_macro(MACRO* macro, const char* macro_name) {
    macro->macro_details = NULL;
    macro->macro_name = (char*)malloc(sizeof(char) * (strlen(macro_name) + 1));
	if(macro->macro_name == NULL)
    {
        return;
    }
    strcpy(macro->macro_name, macro_name);
}
/*inserts a line into the macro*/
void insert_line_macro(MACRO* macro, const char* line) {
    LINE* new_contents;
    if (macro->macro_details == NULL) {
        macro->macro_details = (MACRO_DATA*)malloc(sizeof(MACRO_DATA));
        if(macro->macro_details == NULL)
        {
            return;
        }
        macro->macro_details->macro_contents = NULL;
        macro->macro_details->content_size = 0;
    }
    new_contents = (LINE*)realloc(macro->macro_details->macro_contents, sizeof(LINE) * (macro->macro_details->content_size + 1));
    if (new_contents == NULL)
    {
        return;
    }
    macro->macro_details->macro_contents = new_contents;
    strcpy(macro->macro_details->macro_contents[macro->macro_details->content_size], line);
    macro->macro_details->content_size++;
}
/*Check if the start is a mcr in order to iniate the program that contains a macro.*/
char* is_mcr(LINE str_line) {
    char* no_spaces;
    int i;
    if(is_empty_line(str_line) == TRUE) {
        return NULL;
    }
    no_spaces = skip_blank(str_line);
    if(strlen(no_spaces) <= strlen(MCR_SPACE)) {
        return NULL;
    }
    for (i = 0; i < strlen(MCR_SPACE); i++) {
        if (no_spaces[i] != MCR_SPACE[i]) {
            return NULL;
        }
    }
    no_spaces += strlen(MCR_SPACE);
    return skip_blank(no_spaces); /*returns pointer to the start of the macro name.*/
}
void set_macro(char* analyze_macro, FILE* f, HASHTABLE* macro_hash){
    MACRO macro;
    LINE current_line;
    char* no_spaces;
    char* result;
    initiate_macro(&macro, analyze_macro);
    while (TRUE) {
	result = fgets(current_line, MAX_LINE_CONTENT, f);
	if(result== NULL)
{
	return;
	break;
}
        no_spaces = skip_blank(current_line);
        fix_line_end(no_spaces); 
        if (!is_empty_line(current_line)) {
            if (strcmp(no_spaces, MCR_END) == 0) {
                break; /*Ends the macro registering when we find MCR_END*/
            }
            insert_line_macro(&macro, no_spaces);
        }
    }
    insert_item_hash(macro_hash, macro.macro_name, macro.macro_details, compare_macro);
}
void free_macro_key(void* macro_key) { /*Free the macro key for future use*/
    char* macro_name = (char*)macro_key;
    free(macro_name);
}

void free_macro_data(void* macro_value) { /*Free the data for future use*/
    MACRO_DATA* macro_details = (MACRO_DATA*)macro_value;
    free(macro_details->macro_contents);
    free(macro_details);
}
