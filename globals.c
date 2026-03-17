#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include <math.h>
#include <ctype.h>

/*Asigning number to each functioning and copying its name.*/
void init_functions_name(FUNCTIONS_NAME* functions_command) {
    strcpy(functions_command->functions_name[0], "mov");
    strcpy(functions_command->functions_name[1], "cmp");
    strcpy(functions_command->functions_name[2], "add");
    strcpy(functions_command->functions_name[3], "sub");
    strcpy(functions_command->functions_name[4], "not");
    strcpy(functions_command->functions_name[5], "clr");
    strcpy(functions_command->functions_name[6], "lea");
    strcpy(functions_command->functions_name[7], "inc");
    strcpy(functions_command->functions_name[8], "dec");
    strcpy(functions_command->functions_name[9], "jmp");
    strcpy(functions_command->functions_name[10], "bne");
    strcpy(functions_command->functions_name[11], "red");
    strcpy(functions_command->functions_name[12], "prn");
    strcpy(functions_command->functions_name[13], "jsr");
    strcpy(functions_command->functions_name[14], "rts");
    strcpy(functions_command->functions_name[15], "stop");
}


FUNCTIONS is_function_defintion(LINE str_line, FUNCTIONS_NAME* functions_name) 
{
    int i;
    
    char* temp_pointer;
    char* temp_pointer2;
    LINE temp_line;
    strcpy(temp_line, str_line);
    temp_pointer = temp_line;
    if ((temp_pointer2 = strstr(temp_line, ":")) != NULL) {
        temp_pointer = strstr(temp_line, ":");
        temp_pointer++; /*Skipping the ":" to get to the space after the label*/
        temp_pointer = skip_blank(temp_pointer);
    }
    strncpy(temp_line, temp_pointer, 4);
    if(temp_pointer[3]!='p') /*Making sure that the function isn't stop*/
    {
        temp_pointer[3] = '\0';
    }
    else /*In case the function is stop*/
    {
        temp_pointer[4] = '\0';
    }

    for(i=0; i< FUNCTIONS_COUNT; i++)
    {
        if (strcmp(temp_pointer, functions_name->functions_name[i]) == 0) 
        {
            return (FUNCTIONS)i;
        }
    }
	return INVALID;
}

/*This function returns the pointer of the first char that isn't a space or a tab.*/
char* skip_blank(LINE str_line) { 
    int i;
    for (i = 0; i < strlen(str_line); i++) {
        if (str_line[i] != '\t' && str_line[i] != ' ') {
            return str_line + i;
        }
    }
    return NULL;
}
/*Check to see if the line is empty.*/
boolean is_empty_line(LINE str_line) { 
    char* no_blanks;
    no_blanks = skip_blank(str_line);
    if (no_blanks == NULL) {
        return TRUE;
    }
    if (*no_blanks == '\n' || *no_blanks == '\0' || *no_blanks == ';') {
        return TRUE;
    }
    return FALSE;
}
/*Fixes postfix of a line from \n to \0*/
void fix_line_end(LINE str_line) {
    str_line[strcspn(str_line, "\n")] = '\0';
}
/*Pushes ints into the bit array*/
void pushInt(int value, BITS_STRUCT* array, int* size) {
    if (*size < MEMORY_SIZE) {

        array[*size].bits = value;
        (*size)++;
    }
}

void print_to_binary(int temp_num, FILE* f_object)
{
    char print[BIT_SIZE];
    BITS_STRUCT result;
    int i;
    BITS_STRUCT bits;
    memset(print, 0, sizeof(print));
    bits.bits = 0x0;
    result.bits = 0x0;
    bits.bits |= temp_num;
    for (i = 0; i < BIT_SIZE; i++)
    {
        result.bits = bits.bits << (BIT_SIZE - 1);
        result.bits >>= (BIT_SIZE - 1);
        if (result.bits == 0)
        {
            print[i] = '.';
        }
        else
        {
            print[i] = '/';
        }
        bits.bits >>= 1;
    }
    for (i = BIT_SIZE - 1; i >= 0; i--)
    {
        fprintf(f_object, "%c", print[i]);
    }
    fprintf(f_object, "\n");
}

void print_array_to_binary(int* arr, FILE* f_object)
{
    int i, num;
    for (i = BIT_SIZE - 1; i >= 0; i--)
    {
        num = arr[i];
        if (num == 0)
        {
            fprintf(f_object, ".");
        }
        else
        {
            fprintf(f_object, "/");
        }
    }
    fprintf(f_object, "\n");
}

void delete_end(char* str_line) {
    int i;
    for (i = (int)strlen(str_line); i >= 0; i--)
    {
        if (str_line[i] != ' ' && str_line[i] != '\t')
        {
            str_line[i] = '\0';
            break;
        }
    }
}
boolean is_digit_only(char* str_line) {
    if (*str_line == '#')
    {
        str_line++;
        if (*str_line == '-' || *str_line == '+')
        {
            str_line++;
        }
        while (*str_line != '\0')
        {
            if (!isdigit(*str_line))
            {
                return FALSE;
            }
            str_line++;
        }
        return TRUE;
    }
    return FALSE;
}

boolean is_register(char* str_line)
{
    return (boolean)(strlen(str_line) == 2 && str_line[0] == 'r' && str_line[1] >= '0' && str_line[1] <= '7');
}

boolean is_label(char* str_line)
{
    
    if (*str_line == 'r')
    {
        str_line++;
        if (*str_line <= '7' && *str_line >= '0')
        {
            str_line++;
            if (*str_line == ' ' || *str_line == '\t' || *str_line == '\0')
            {
                return FALSE;
            }
            str_line--;
        }
        str_line--;
    }
    
    if (!isalpha(*str_line))
    {
        return FALSE;
    }
    for (; *str_line != '\0'; str_line++) 
    {
        if (!isalnum(*str_line))
        {
            return FALSE;
        }
    }
    return TRUE;
}

boolean translate_to_bits(int value, int* arr) 
{
    int i, current_value, result;
    if (value < MIN_VALUE)
    {
        printf("Cannot represent this number in 14bits");
        return FALSE;
    }
    current_value = MIN_VALUE;
    arr[BIT_SIZE - 1] = 1; /* Setting the first bit to 1 when we found that the number is a negative */ 
        for (i = BIT_SIZE - 2; i >= 0;i--)
        {
            result = (int)pow(2, i);
            if (current_value + result <= value)
            {
                arr[i] = 1;
                current_value += result;
            }
            else
            {
                arr[i] = 0;
            }
        }
    return TRUE;
}

int register_op_des(int i) 
{
    switch (i)
    {
    case 0:
        return 0;
    case 1:
        return R1_DESTINATION_OP;
    case 2:
        return R2_DESTINATION_OP;
    case 3:
        return R3_DESTINATION_OP;
    case 4:
        return R4_DESTINATION_OP;
    case 5:
        return R5_DESTINATION_OP;
    case 6:
        return R6_DESTINATION_OP;
    case 7:
        return R7_DESTINATION_OP;

    default: return -1;
        break;
    }
}
int type_op_code(FUNCTIONS function_type) { 
    switch (function_type)
    {
    case MOV:
        return 0;
    case CMP:
        return CMP_OP;
    case ADD:
        return ADD_OP;
    case SUB:
        return SUB_OP;
    case NOT:
        return NOT_OP;
    case CLR:
        return CLR_OP;
    case LEA:
        return LEA_OP;
    case INC:
        return INC_OP;
    case DEC:
        return DEC_OP;
    case JMP:
        return JMP_OP;
    case BNE:
        return BNE_OP;
    case RED:
        return RED_OP;
    case PRN:
        return PRN_OP;
    case JSR:
        return JSR_OP;
    case RTS:
        return RTS_OP;
    case STOP:
        return STOP_OP;


    default: return -1;
        break;
    }
}
