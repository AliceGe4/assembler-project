#ifndef _GLBOALS_H
#define _GLBOALS_H
#define MAX_LINE_CONTENT 81
#define BIT_SIZE 14
#define REGISTER_COUNT 8
#define FUNCTIONS_COUNT 16
#define MEMORY_SIZE 256
#define MIN_VALUE -8192
#define MAX_OP_SIZE 5
#define MOV_OP 0
#define R1_DESTINATION_OP 0x4
#define R2_DESTINATION_OP 0x8
#define R3_DESTINATION_OP 0xC
#define R4_DESTINATION_OP 0x10
#define R5_DESTINATION_OP 0x14
#define R6_DESTINATION_OP 0x18
#define R7_DESTINATION_OP 0x1C
#define CMP_OP 0x40
#define ADD_OP 0x80
#define SUB_OP 0xC0
#define NOT_OP 0x100
#define CLR_OP 0x140
#define LEA_OP 0x180
#define INC_OP 0x1C0
#define DEC_OP 0x200
#define JMP_OP 0x240
#define BNE_OP 0x280
#define RED_OP 0x2C0
#define PRN_OP 0x300
#define JSR_OP 0x340
#define RTS_OP 0x380
#define STOP_OP 0x3C0
/*Defining a LINE*/
typedef char LINE[MAX_LINE_CONTENT];
/* Defines boolean in C*/
typedef enum boolean {
    FALSE = 0,
    TRUE = 1
}boolean;
/*Table to store Bit data*/
typedef struct {
    int bits : BIT_SIZE;
}BITS_STRUCT;
/*Table to store Register data*/
typedef struct {
    BITS_STRUCT registers[REGISTER_COUNT];
}REGISTERS;
/*Enum for us to move  through all the functions from 0-15 and INVALID*/
typedef enum functions {
    MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, INVALID
}FUNCTIONS;
/*Struct for storing the names of the functions*/
typedef struct{ 
    char functions_name[FUNCTIONS_COUNT][MAX_OP_SIZE];
}FUNCTIONS_NAME;
/**
* is_function_defintion searches through our enum functions table to find which function it is
* @param str_line our current line that is transfered to be checked
* @param functions_name a pointer to our table of functions
* @return returns either the i number that defines our function or INVALID in case it wasn't found.
*/
FUNCTIONS is_function_defintion(LINE str_line, FUNCTIONS_NAME* functions_name);
/**
* iniates function name, saving it as a string.
* @param functions_command point to our enum functions struct
*/
void init_functions_name(FUNCTIONS_NAME* functions_command);
/**
* delete_end deletes the end of a string removing any tabs or spaces
* @param str_line the pointer to the line that the end is removed of
*/
void delete_end(char* str_line);
/**
* checks that the pointer of the string that is passed will only have digits
* @param str_line the pointer to the line that is checked
*/
boolean is_digit_only(char* str_line);
/**
* checks that the pointer of the string that is passed will contain a register and nothing else
* @param str_line the pointer to the line that is checked
*/
boolean is_register(char* str_line);
/**
* checks that the pointer of the string that is passed will contain a label and nothing else (A discover or not discovered label)
* @param str_line the pointer to the line that is checked
*/
boolean is_label(char* str_line);
/**
*A function to skip to the next index that contains something that isnt a blank
* @param str_line a line to be checked for empty spaces
* @return returns the line +i the next index where its not empty
*/
char* skip_blank(LINE str_line); 
/**
*Function that checks if the line is empty
* @param str_line the line that is checked to see if its empty.
* @return returns true if the line is empty and false otherwise.
*/
boolean is_empty_line(LINE str_line);
/**
* Function to fix the end of a line / postfix 
* @param str_line the line that has its end changed between \n and \0.
*/
void fix_line_end(LINE str_line);
/**
* Pushes the passed value into our memory array
* @param value a passed int value (or casted to int) to be pushed into the array
* @param array pointer to our BITS array
* @param size a pointer to an int size
*/
void pushInt(int value, BITS_STRUCT* array, int* size);
/**
* Prints recieved data into binary code of . and / inside of the f_object file
* @param temp_num the data to be printed into the file
* @param f_object a pointer to our f_object file
*/
void print_to_binary(int temp_num, FILE* f_object);
/**
*
*/
boolean translate_to_bits(int value, int* arr);
/**
*
*/
void print_array_to_binary(int* arr, FILE* f_object);
/**/
int register_op_des(int i);
/**/
int type_op_code(FUNCTIONS function_type);

#endif
