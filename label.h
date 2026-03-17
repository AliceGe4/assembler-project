#ifndef _LABEL_H
#define _LABEL_H
#include "assembler.h"
#define DATA_TYPE ".data"
#define STRING_TYPE ".string"
#define EXTERN_TYPE ".extern"
#define ENTRY_TYPE ".entry"
#define MAX_TYPE_SIZE 9
#define MAX_LABEL_SIZE 31
#define LABEL_TYPE_DESTINATION_OP 0x4
#define JUMP_TYPE_DESTINATION_OP 0x8
#define REGISTER_TYPE_DESTINATION_OP 0xC

typedef enum {
    IMMIDIATE_TYPE, LABEL_TYPE, JUMP_TYPE, REGISTER_TYPE, INVALID_OPERAND_TYPE
}OPERAND_TYPE;
/*struct to keep our types of labels*/
typedef enum{
    DATA, EXTERNAL, CODE, ENTRY
}LABEL_TYPES;
/*The data inside of our label hash table*/
typedef struct {
    LABEL_TYPES label_type;
    int label_decimal_base;
}LABEL_DATA;
/*The main table that holds the name and the data table inside of it*/
typedef struct {
    char* label_name;
    LABEL_DATA* label_details;

}LABEL;



/**
* Initiate the label into the corosponding hash table
* @param value a passed int value (or casted to int) to be pushed into the array
* @param array pointer to our BITS array
* @param size a pointer to an int size
*/
void initiate_label(LABEL* label, const char* label_name); 
/**
*compares labels l1 and l2
* @parm l1 the first label to be compared
* @param l2 the second label to be compared
* @return returns 0 if they were the same, otherwise reutrns 1
*/
int compare_label(void* l1, void* l2);
/**
*inserts the label into the hashtable and returns whether we managed to insert it
*@parm hash a pointer to our hashtable function
* @param label the label to be inserted into the hashtable
* @return returns 0 if it succsesfully inserted the label into our hash table
*/
int insert_label(HASHTABLE* hash, LABEL* label);
/**
*checks to see if its a start of label
* @param str_line the line we are currently running on
* @param functions_name a pointer to our table that contains all of our functions
* @return returns label_name the name of the label.
*/
char* is_label_defintion(char* str_line, FUNCTIONS_NAME* functions_name);
/**
* Frees the label key
* @param label_key a void function, generally we send the name of the label into this function.
*/
void free_label_key(void* label_key);
/**
* Frees the label data
* @param label_details the details that  the label contains, could be lines etc.
*/
void free_label_data(void* label_details);
/**
* Checks which instruction it is
* @param str_line accepts our currently running line.
* @return returns the instruction type in the current line.
*/
char* instruction_type(const char* str_line);
/**
* This function handles .data type lines, it knows to accept our line and translate it into proper data to be printed later.
* @param str_line accepts our currently running line.
* @param memory a pointer to our MEMORY array that contains important info such as IC, DC and our label hashtable.
* @param f_object this is the object file that will have our binary print in the second pass
* @param decimal_adress a decmal adress that we use in the second pass to be printed into the object file
* @return returns false if it wasn't succseful at saving the data or translating it.
*/
boolean translate_data(char* str_line, MEMORY* memory, FILE* f_object, int* decimal_adress);
/**
* This function handles .string type lines, it knows to accept our line and translate it into proper data in ascii to be printed later.
* @param str_line accepts our currently running line.
* @param memory a pointer to our MEMORY array that contains important info such as IC, DC and our label hashtable.
* @param f_object this is the object file that will have our binary print in the second pass
* @param decimal_adress a decmal adress that we use in the second pass to be printed into the object file
* @return returns false if it wasn't succseful at saving the data or translating it.
*/
boolean translate_string(char* str_line, MEMORY* memory, FILE* f_object, int* decimal_adress);
/**
* inserts data into our label hash table
* @param label a pointer to our LABEL hashtable
* @param memory a pointer to our MEMORY array that contains important info such as IC, DC and our label hashtable.
* @param label_data a pointer to our contents of label, the struct label_data
*/
void insert_data_label(LABEL* label, MEMORY* memory, LABEL_DATA* labal_data);
/**
* this function is part of 1.9 going through externs and entry strings and handling their data
* @param str_line accepts our currently running line.
* @param memory a pointer to our MEMORY array that contains important info such as IC, DC and our label hashtable.
* @param skip_type this is the type of function that is passsed in a string, extern/entry
* @param label_type the type of function that is used
* @param entry_extern a pointer to our file
*/
boolean extract_labels_extern_entry(char* str_line, MEMORY* memory, char* skip_type, LABEL_TYPES label_type, FILE* entry_extern, int* decimal_adress);
/**
* this function calculates L in the first pass, in the second pass it handles majority of printing into object file
* @param str_line accepts our currently running line.
* @parm function_type the type of function from our 16 functions
* @param memory a pointer to our MEMORY array that contains important info such as IC, DC and our label hashtable.
* @param f_object the object file to write into
* @param decimal_adress a decimal adress used in printing in the second pass
* @param f_extern the extern file to print our externs
* @return returns the length of L, will return 0 if the line was invalid
*/
int calculate_L(LINE str_line, FUNCTIONS function_type, MEMORY* memory, FILE* f_object, int* decimal_adress, FILE* f_extern);
/**
* This function writes a label into binary code inside of the obj file.
* @param label_name the name of the label to be written
* @param label_hash a pointer to our hashtable in order to access its functions
* @param f_object the object file to write into
* @decimal_adress a decimal adress used in printing in the second pass
* @return returns true if it was succsesful
*/
boolean write_label_to_binary(char* label_name, HASHTABLE* label_hash, FILE* f_object, int* decimal_adress);
/**
*first_op_to_binary writes the first line isnide the obj file, containg the corrent bit according to the sorting method.
* @param decimal_adress a decimal adress used in printing in the second pass
* @param f_object the object file to write into
* @param op_code the number of the operation from 0-15
* @param is_source a boolean to check if we are sending a source type
* @param source_type the type of the source operand
* @param is_destination a boolean to check if we are sending a destination type
* @param destination_type the type of the destination operand
* @param first_param first param inside of the jump type (In case we have a jump (operand, operand2))
* @param second_param second param inside of the jump type
*/
void first_op_to_binary(int* decimal_adress, FILE* f_object, int op_code, boolean is_source, OPERAND_TYPE source_type, boolean is_destination, OPERAND_TYPE destination_type, OPERAND_TYPE first_param, OPERAND_TYPE second_param);
/**
* operands_to_binary a general function to filter our operands inside calculate L and be sent into their according print to obj file, also handles extern types
* @param decimal_adress a decimal adress used in printing in the second pass
* @param f_object the object file to write into
* @param is_source used to determine if we should print the data into the source area, important in the case of registers
* @param memory a pointer to our memory table in order to take out the label from our label table.
* @param string a pointer to our string that contains the line with the text of the operand
* @param f_extern inputed here in order to handle object printing into the ext file
*/
boolean operands_to_binary(int* decimal_adress, FILE* f_object, boolean is_source, MEMORY* memory, char* string, FILE* f_extern);
/**
* type_operand_type a function that returns which type of operand it is
* @param string a pointer to our string that contains the line with the text of the operand
* @memory a pointer to our memory table in order to take out the label from our label table.
* @return returns operand type
*/
OPERAND_TYPE type_operand_type(char* string, MEMORY* memory);
#endif
