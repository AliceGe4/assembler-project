#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "assembler.h"
#include "macro.h"
#include "label.h"
#include "hashtable.h"
#include "globals.h"
/**
 * Assmeble process and assembles a file, this is the main fucntion that includes all the steps and eventually prints the file as requested.
 * @param filename The filename given by the system. The extension to it is added later.
 * @return returns the flag that determines succses of failure of the program.
*/
boolean assemble(char* filename);

int main(int argc, char* argv[]) {
	int i;
	MEMORY memory;
	LINE after_pre_assembler, f_object_name, f_external_name, f_entry_name;
	FILE* f;
	FILE* f_object;
	FILE* f_external;
	FILE* f_entry;
	boolean flag = TRUE;
	/*flag is a general flag to determine if the function is succsesful so far, it is used to terminate it in case of an error.*/

	for (i = 1; i < argc; ++i) {

		flag = assemble(argv[i]);

		if (!flag) {
			printf("%s File has problems", argv[i]);
		}
		else
		{
			strcpy(after_pre_assembler, argv[i]);
			strcpy(f_object_name, argv[i]);
			strcpy(f_external_name, argv[i]);
			strcpy(f_entry_name, argv[i]);
			strcpy(after_pre_assembler + strlen(after_pre_assembler), ".am"); /*Adds .am to our files*/
			strcpy(f_object_name + strlen(f_object_name), ".ob"); /*Adds .ob to our files*/
			strcpy(f_external_name + strlen(f_external_name), ".ext"); /*Adds .ext to our files*/
			strcpy(f_entry_name + strlen(f_entry_name), ".ent"); /*Adds .ent to our files*/
			f = fopen(after_pre_assembler, "r");

			if (f==NULL)
			{
				printf("File error at %s\n", after_pre_assembler);
				continue;
			}
			initiate_memory(&memory);
			if(first_pass(&memory, f) == FALSE)
			{
				printf("First pass failed in %s\n", argv[i]);
				fclose(f);
				free_memory(&memory);
			}
			else
			{
				fclose(f);
				f = fopen(after_pre_assembler, "r");
				f_object = fopen(f_object_name, "w");
				f_external = fopen(f_external_name, "w");
				f_entry = fopen(f_entry_name, "w");
				if(second_pass(&memory, f, f_object, f_external, f_entry) == FALSE)
				{
					printf("Second pass failed in %s\n", argv[i]);
					fclose(f);
					fclose(f_object);
					fclose(f_external);
					fclose(f_entry);
					remove(f_object_name);
					remove(f_external_name);
					remove(f_entry_name);
				}
				free_memory(&memory);
			}
			
		}


	}
	return 0;
}

boolean assemble(char* filename) {
	/*Start of pre assembler*/
	FILE* f;
	LINE str_line;
	LINE new_file_name; /*string to store the name of the new file*/
	FILE* z;
	LINE str;
	MACRO_DATA* macro_data; /*Pointer to our Macro table*/
	HASHTABLE macro_hash; /*creates a hash for macros*/
	int i;
	char* analyze_macro; 
	char* no_spaces_pointer; /*A pointer that allows us to skip blank spaces*/
	strcpy(str, filename);
	strcpy(str + strlen(str), ".as");
	f = fopen(str, "r");
	strcpy(new_file_name, filename);
	strcpy(new_file_name + strlen(new_file_name), ".am"); /*Adds .am to our files*/
	z = fopen(new_file_name, "w"); /*opens the new file in write*/
	initiate_hash(&macro_hash);
	if (f == NULL) {
		printf("%s is not readable\n", str);
		return FALSE;
	}
	if (z == NULL) {
		printf("%s is not writeable\n", new_file_name);
		return FALSE;
	}/*Here we go through the file and work the pre-assembler on it, turning macros into lines and printing into .am*/
	while (fgets(str_line, MAX_LINE_CONTENT, f)) {
		no_spaces_pointer = skip_blank(str_line); /*skips to the next non blank index*/
		analyze_macro = is_mcr(no_spaces_pointer); /*Checks if the pointer is a start of macro declecration aka mcr*/
		if (analyze_macro != NULL) {
			set_macro(analyze_macro, f, &macro_hash); /*If it is a macro that wasn't set before, we set into the hash table using the macro version of it.*/
		}
		else {
			macro_data = pull_item_hash(&macro_hash, no_spaces_pointer, compare_macro); /*in case the macro was set before, we pull it from the table*/
			if (macro_data != NULL) {
				for (i = 0; i < macro_data->content_size; i++) { 
					fprintf(z, "%s\n", macro_data->macro_contents[i]); /*searching the table one by one.*/ 
				}
			}
			else {

					fprintf(z, "%s", no_spaces_pointer);
			
			}
		}
	}
	fclose(f);
	fclose(z);
	free_hashtable(&macro_hash, free_macro_key, free_macro_data); /*we free the hashtable and the macros in order to get to the next file.*/
	return TRUE;
} /*End of pre assembler.*/


/*Start of first pass*/
boolean first_pass(MEMORY* memory, FILE* f) { 
	boolean no_errors;
	boolean table_insert;
	HASHTABLE_ITEM* hashtable_item;
	LABEL_DATA* label_data;
	LABEL_DATA* temp_label;
	FUNCTIONS_NAME functions_name;
	FUNCTIONS function_type;
	char* label_flag_pointer;
	int line_count;
	int i;
	int L;
	LINE str_line;
	char* data_type;
	line_count = 1;
	no_errors = TRUE;
	memory->IC = 0, memory->DC = 0;
	memory->label_hash = (HASHTABLE*)malloc(sizeof(HASHTABLE)); 
	initiate_hash(memory->label_hash); 
	init_functions_name(&functions_name);
	while (fgets(str_line, MAX_LINE_CONTENT, f)) {
		if(is_empty_line(str_line))
		{
			line_count++;
			continue;
		}
		if (strlen(str_line) > MAX_LINE_CONTENT) {
			printf("Line is too long, Error at line: %d\n", line_count);
			no_errors = FALSE;
		}
		label_flag_pointer = is_label_defintion(str_line, &functions_name); /*We check if its a label*/
		if (label_flag_pointer != NULL) {   /*Note - label_flag_pointer is currently the name of the label aka XYZ*/
			if(strlen(label_flag_pointer) > MAX_LABEL_SIZE)
			{
				printf("Max label name is 30\n");
				no_errors = FALSE;
				line_count++;
				continue;
			}
			memory->is_label = 1; /*Turning is_label flag as true.*/
		}
		if (strstr(str_line, DATA_TYPE) != NULL || strstr(str_line, STRING_TYPE) != NULL)
		{
			if (memory->is_label == 1)
			{
				temp_label = (LABEL_DATA*)pull_item_hash(memory->label_hash, label_flag_pointer, compare_label); 
				if (temp_label != NULL)
				{
					printf("Label name already exsists, Error at line: %d\n", line_count);
					no_errors = FALSE;
				}
				else
				{
					label_data = (LABEL_DATA*)malloc(sizeof(LABEL_DATA));
					if(label_data == NULL)
					{
						printf("Failed to allocate memory, Error at line: %d\n", line_count);
						no_errors = FALSE;
					}
					else 
					{
						label_data->label_decimal_base = memory->DC;

						label_data->label_type = DATA;

						insert_item_hash(memory->label_hash, label_flag_pointer, label_data, compare_label);

						memory->is_label = 0;
					}
				}
			}
			data_type = strstr(str_line, DATA_TYPE);
			if (data_type != NULL) /*compare that the string we found in instruction_type is ".data/0" */
			{
				table_insert = translate_data(str_line, memory, NULL, NULL); /*Step 1.7 Code the data into the data table*/
				if (table_insert != TRUE)
				{
					printf(", Error at line: %d\n", line_count);
					no_errors = FALSE;
				}
			}
			data_type = strstr(str_line, STRING_TYPE);
			if (data_type != NULL)
			{
				table_insert = translate_string(str_line, memory, NULL, NULL); /*Step 1.7 Code the string's data into the data table*/
				if (table_insert != TRUE)
				{
					printf(", Error at line: %d\n", line_count);
					no_errors = FALSE;
				}
			}
			line_count++;
			continue;
		}
		/*Start of 1.8*/
		if (strstr(str_line, EXTERN_TYPE) != NULL || strstr(str_line, ENTRY_TYPE) != NULL) 
		{ /*Preforming 1.9*/
			if(memory->is_label ==1)
			{
				printf("Warning label flag is on during extern or entry, Warning at line: %d\n", line_count);
				memory->is_label = 0;
			}
			data_type = strstr(str_line, EXTERN_TYPE);
			if (data_type != NULL)
			{
				if (extract_labels_extern_entry(str_line, memory, EXTERN_TYPE, EXTERNAL, NULL, NULL) == FALSE) 
				{
					printf(", Error at line: %d\n", line_count);
					no_errors = FALSE;
				}
				
			}
			line_count++;
			continue;
		}
		if (memory->is_label == 1) /*Start of 1.11 checking if label flag is on*/
		{
			temp_label = (LABEL_DATA*)pull_item_hash(memory->label_hash, label_flag_pointer, compare_label);
			if (temp_label != NULL)
			{
				printf("Label name already exsists, Error at line: %d\n", line_count);
				no_errors = FALSE;
			}
			else
			{
				label_data = (LABEL_DATA*)malloc(sizeof(LABEL_DATA));
					if(label_data == NULL)
					{
						printf("Failed to allocate memory, Error at line: %d\n", line_count);
						no_errors = FALSE;
					}
					else 
					{
				label_data->label_decimal_base = memory->IC;
				label_data->label_type = CODE;
				insert_item_hash(memory->label_hash, label_flag_pointer, label_data, compare_label);
				memory->is_label = 0; 
				}	
			} /*Finished step 1.11*/
		} /* Start of 1.12*/
		function_type = is_function_defintion(str_line, &functions_name); /*returns the int of the currently running function, is invalid if doesnt exsist.*/
		if (function_type == INVALID)
		{
			printf("opname doesn't exsist, Error at line %d\n", line_count);
			line_count++;
			continue;
		}
		
		L = calculate_L(str_line, function_type, memory, NULL, NULL, NULL);  /*General function to calculate L, used in second pass for printing.*/
		if(L== 0)
		{
			no_errors = FALSE;
			printf(", Error at line: %d\n", line_count);
		}
		memory->IC += L; /*Increasing our IC counter by L*/
		line_count++;	
	}
	if (no_errors == FALSE) { /*General flag that indicates found errors*/
		printf("First pass failed, errors in code.\n");
		return FALSE;
	}
	for (i = 0; i < memory->label_hash->size_ht; i++)  /*Stage 1.17 increasing the IC of data types inside of our table*/
	{
		hashtable_item = &memory->label_hash->ht[i];
		label_data = (LABEL_DATA*)hashtable_item->value;
		if (label_data->label_type == DATA)
		{
			label_data->label_decimal_base += memory->IC;
		}
	}
	return no_errors;
}

boolean second_pass(MEMORY* memory, FILE* f_origin, FILE* f_object, FILE* f_extern, FILE* f_entry)
{ /*Start of second pass*/
	boolean no_errors;
	char* skip_label;
	int line_count;
	int decimal_adress;
	int L;
	char* entry_search;
	LINE str_line;
	FUNCTIONS_NAME functions_name;
	FUNCTIONS function_type;
	LABEL_DATA* label_data;
	decimal_adress = 100; /*We increas our decimal adress to 100 for printing*/
	line_count = 1;
	no_errors = TRUE;
	memory->IC = 0;
	init_functions_name(&functions_name);
	while (fgets(str_line, MAX_LINE_CONTENT, f_origin)) /*Start reading from our new origin file, proper start of second pass.*/
	{
		delete_end(str_line); /*An emergency net removing the end of lines*/
		fix_line_end(str_line);
		if((entry_search = strstr(str_line, ENTRY_TYPE)) != NULL) /*Rewriten the way second pass works, this is stage 1.5, if we find .entry, we go over the file and input into our entry file, the order doesn't matter as mentioned in the project.*/
		{
			entry_search += strlen(ENTRY_TYPE); /*We skip over the .entry*/
			entry_search = skip_blank(entry_search);/*We skip over the blank space to get to the label so we can properly extract it from the file.*/
				label_data = pull_item_hash(memory->label_hash, entry_search, compare_label);
				if (label_data != NULL) {
					fprintf(f_entry, "%s\t %d\n", entry_search, label_data->label_decimal_base+100); /*increased by 100 to represent the new table.*/
					line_count++; /*skipping over the line of .entry as it itself doesnt take space in the object and therefore the rest of the checks are not needed.*/
					continue;
				}
		}
		if (is_empty_line(str_line)) /*A skip over an empty line, we kept the empty lines in order to keep the look of the original file.*/
		{
			line_count++;
			continue;
		}
		else
		{
			skip_label = str_line;
			
		}
		if (strstr(skip_label, EXTERN_TYPE) != NULL) /*We handle extern/data/string in second pass as it lessens complication of printing items in first pass*/
		{
			extract_labels_extern_entry(skip_label, memory, EXTERN_TYPE, EXTERNAL, f_extern, &decimal_adress);
			line_count++;
			continue;
		}
		if (strstr(skip_label, DATA_TYPE) != NULL)
		{
			translate_data(str_line, memory, f_object, &decimal_adress); /*upgraded version of translate data from first pass in order to print*/
			line_count++;
			continue;
		}
		if	(strstr(skip_label, STRING_TYPE) != NULL)
		{
			translate_string(str_line, memory, f_object, &decimal_adress);/*upgraded version of translate string from first pass in order to print*/
			line_count++;
			continue;
		}
		function_type = is_function_defintion(str_line, &functions_name);
		
		L = calculate_L(str_line, function_type, memory, f_object, &decimal_adress, f_extern);  /*calculate L that doesn't have NULLS in it will now access the printing function of calculate_L inorder to create the object file, done fully in second pass.*/
		if (L == 0)
		{
			printf(", Error at line: %d\n", line_count);
			no_errors = FALSE;
		}
		memory->IC += L;
		line_count++;
	}
		return no_errors;
}


void free_memory(MEMORY* memory) /*General function to free memory*/
{
	free_hashtable(memory->label_hash, free_label_key, free_label_data);
}

void initiate_memory(MEMORY* memory) /*General function to initiate memory*/
{
	memory->DC = 0;
	memory->IC = 0;
	memory->is_label = 0;
	memory->label_hash = NULL;
}
