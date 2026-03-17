#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "label.h"


/* compares labels l1 and l2*/
int compare_label(void* l1, void* l2) {
    char* first_label = (char*)l1;
    char* second_label = (char*)l2;
    return strcmp(first_label, second_label);
}
/* inserts the label into the hashtable*/
int insert_label(HASHTABLE* hash, LABEL* label){
    return insert_item_hash(hash, label->label_name, label->label_details, compare_label); 
}
/*Iniates the memory of the label_hash*/
void initiate_label(LABEL* label, const char* label_name) {
    label->label_details = NULL;
    label->label_name = (char*)malloc(sizeof(char) * (strlen(label_name) + 1));
if(label->label_name == NULL)
    {
        return;
    }
    strcpy(label->label_name, label_name);
}
/*checks to see if its a start of label*/
char* is_label_defintion(char* str_line, FUNCTIONS_NAME* functions_name) {
    FUNCTIONS function;
    LINE temp_line;
    char* label_name;
    char* is_label;
    LINE label_check_line;
    char* temp_pointer;
    is_label = strstr(str_line, ":"); /*A check to make sure that it is indeed a label that contains :*/
    if (is_label == NULL) {
        return NULL;
    }
    strcpy(label_check_line, str_line);
    temp_pointer = strstr(label_check_line, ":"); /*setting a temp pointer to delete anything after the label*/
    if(temp_pointer == NULL)
    {
        return NULL;
    }
    *temp_pointer = '\0';
    delete_end(label_check_line);
    function = is_function_defintion(label_check_line, functions_name); 
    if (function != INVALID) /*A check to make sure that our label isn't one of the 16 functions*/
    {
        printf("Label cannot be an exsisting function name");
        return NULL;
    }
    if (isalpha(str_line[0]) == 0) { /*A check to make sure its a valid label*/
        printf("label must start with a letter");
        return NULL;
    }
    else {
        strcpy(temp_line, str_line);
        temp_line[is_label - str_line] = '\0'; /*temp line is set to everything that is before the ':'*/
    }
    label_name = (char*)malloc(sizeof(char) * (strlen(temp_line) + 1)); /*we set the name of the label and create memory for it.*/
	if (label_name == NULL)
    {
        return NULL;
    }
    strcpy(label_name, temp_line);
    return label_name;
}
/*Frees the label key*/
void free_label_key(void* label_key) {
    char* label_name = (char*)label_key;
    free(label_name);
}
/*Frees the label data*/
void free_label_data(void* label_value) {
    LABEL_DATA* label_details = (LABEL_DATA*)label_value;
    free(label_details);
}
void insert_data_label(LABEL* label, MEMORY* memory, LABEL_DATA* labal_data){ /*Function adapated from insert Hashtable, we insert the decimal base and the type.*/
    if (label->label_details == NULL) {
        label->label_details = (LABEL_DATA*)malloc(sizeof(LABEL_DATA));
    }
    if(label->label_details == NULL)
    {
        return;
    }
    label->label_details->label_decimal_base = labal_data->label_decimal_base; /*acessing decimal base used for counting functions*/
    label->label_details->label_type = labal_data->label_type; /*accessing the type of the label*/
}
char* instruction_type(const char* str_line) { /*A general function to determine what type of instruction this is*/

    int i;
    char temp_string[MAX_TYPE_SIZE]; 
    char* is_instruction;
    is_instruction = strstr(str_line, "."); /*moving to the . inside the line to access the type*/
    if (is_instruction == NULL) {
        printf("there is no . after the label");
        return NULL; 
    }
    for (i = 0; i < strlen(STRING_TYPE); i++) {
        if (is_instruction[i] == ' ' || is_instruction[i] == '\t') {
            break;
        }
        temp_string[i] = is_instruction[i]; /*We create a string that contains whichever data type it is, we chose it to stop at STRING_TYPE as its the longest it can run.*/
    }
    temp_string[i] = '\0';
    
    is_instruction = (char*)malloc(sizeof(char) * (strlen(temp_string) + 1)); /*allocating memory for the instruction*/
if(is_instruction == NULL)
    {
        return NULL;
    }
    strcpy(is_instruction, temp_string);
    return is_instruction;
}
boolean translate_string(char* str_line, MEMORY* memory, FILE* f_object, int* decimal_adress) {
    int temp_num, i, temp_string_size;
    char* is_string;
    char* temp_string;
    boolean empty_string;
    empty_string = TRUE;
    delete_end(str_line);
    fix_line_end(str_line);
    is_string = strstr(str_line, ".");
    is_string += strlen(STRING_TYPE); /*Getting to the end of .string so we can skip the blanks in order to get to the numbers. This points to either a tab or a space after a.*/
	if(is_string == NULL)
    {
        return FALSE;
    }
    if (*is_string != ' ' && *is_string != '\t') {
        printf("Expected a space or a tab after .string");
        return FALSE;
    }
    is_string = skip_blank(is_string); /*we move our pointer to the start of the string*/
    if (*is_string == '\0' || *is_string == '\n') {
        printf("The command is empty");
        return FALSE;
    }
    if (*is_string != '\"') {
        printf("Invalid start to string command, must start with commas"); /*Making sure that the string starts with a valid start*/
        return FALSE;
    }
    temp_string = strchr(str_line, '\"'); /*Making sure that our string has " in it*/
    if(temp_string == NULL)
    {
        printf("String does not contain quoation marks");
        return FALSE;
    }
    temp_string++;
    temp_string = strchr(temp_string, '\"'); /*A check that we have closing quotation marks*/
    if(temp_string == NULL)
    {
        printf("Invalid string");
        return FALSE;
    }
    else 
    {
        temp_string++; /*skip to the item after the quotaion marks.*/
       
        if(skip_blank(temp_string) != NULL)
        {
            printf("string data contains extra information after the closing quotaion mark");
            return FALSE;
        }
    }
    is_string++; /*setting the string to the location after the " mark.*/
    temp_string_size = (int)strlen(is_string);
    for (i=0; i < temp_string_size; i++)
    {
        if (*is_string == '\"') 
        {
            if (empty_string == FALSE) 
            {
                break;
            }
            else 
            {
                printf("The string is empty"); /*In case we found an empty string, this will only checked once*/
                return FALSE;
            }
        }
        else {
            temp_num = (int)(*is_string);
            if (f_object != NULL) 
            {
                fprintf(f_object, "%d\t", *decimal_adress); /*This is only acess in the second pass in order to print into our table of obj*/
                print_to_binary(temp_num, f_object);
                *decimal_adress += 1;
            }
            else
            {
                pushInt(temp_num, memory->memory_struct, &memory->DC);
            }
            empty_string = FALSE;
        }
        is_string++;
    }
    if (f_object != NULL)
    {
        fprintf(f_object, "%d\t", *decimal_adress); /*Acess that is left specifically for second pass.*/
        print_to_binary(0, f_object);
        *decimal_adress += 1;
    }
    else
    {
        pushInt(0, memory->memory_struct, &memory->DC); /*Pushing the ints of the string into our hashtable.*/
    }
    return TRUE;
}





boolean translate_data(char* str_line, MEMORY* memory, FILE* f_object, int* decimal_adress){ 
        LINE temp_line;
        int arr[BIT_SIZE];
        char* is_data;
        int j;
        int temp_num; /*used later in function that puts numbers into the memory.*/
        char final_array[MAX_LINE_CONTENT][MAX_LINE_CONTENT]; /*An array of strings in order to be printed later.*/
        boolean expecting_comma;
        boolean was_comma;
        boolean was_symbol;
        j = 0;
        expecting_comma = FALSE; /*Checks that mark we need a comma to follow*/
        was_comma = FALSE; /*A check that is there to make sure we don't have two commas following each other*/
        was_symbol = FALSE; /*A check to make sure a + or a - is followed by an int*/
        delete_end(str_line);
        fix_line_end(str_line);
        temp_line[0] = '\0';
        is_data = strstr(str_line, "."); /*Making sure that this is indeed a proper data.*/
        if (is_data == NULL) 
        {
            printf("No . in data");
            return FALSE;
        }
        is_data += strlen(DATA_TYPE); /*Getting to the end of .data so we can skip the blanks in order to get to the numbers. This points to either a tab or a space after a.*/
        if (*is_data != ' ' && *is_data != '\t') {
            printf("Expected a space or a tab after .data");
                return FALSE;
        }
        while(*is_data!='\0') 
        {
            if ((*is_data == ' ' || *is_data == '\t' || *is_data == ',') && was_symbol == TRUE) 
            { /*Safety net in case last symbol was a +/-*/
                printf("Expected an integer after a + or a -");
                return FALSE;
            }
            if (*is_data == ' ' || *is_data == '\t') /*A in function skip blank function*/
            {
                is_data++;
            }
            else if (*is_data != ' ' && *is_data != '\t') /*In case we do not have a blank area, we investigate what's inside.*/
            {
                if (*is_data == '+' || *is_data == '-') /*Handling a + or - case*/
                {
                    if (was_symbol == TRUE) /*Our check is triggered, we can't have two signs follow one after another*/
                    {
                        printf("Expected an integer after a + or a -");
                            return FALSE;
                    }
                    sprintf(temp_line + strlen(temp_line), "%c", *is_data); /*We save the number into temp_line*/
                    was_symbol = TRUE;
                    is_data++;
                }
                else if ((isdigit(*is_data)) != 0 || *is_data == 0)  /*We found an int*/
                {
                    sprintf(temp_line + strlen(temp_line), "%c", *is_data); /*Place our int into temp_line*/
                    expecting_comma = TRUE;
                    was_symbol = FALSE;
                    was_comma = FALSE;
                    is_data++;
                }
                else if(*is_data == ',') /*In case we found a comma we enter here*/
                {
                    if (expecting_comma == FALSE) 
                    {
                        printf("Invalid placement of ,"); /*We can't have two commas one after another*/
                            return FALSE;
                    }
                    was_comma = TRUE;
                    expecting_comma = FALSE;
                    temp_line[strlen(temp_line) + 1] = '\0';
                    strcpy(final_array[j], temp_line);
                    strcpy(temp_line, ""); /*reset temp_line*/
                    j++; /*We increase the pointer of our array of strings in order to input the next int*/
                    is_data++;  
                }
                else 
                {
                    printf("unidentified input, should be an int, a comma or a +/-");
                    return FALSE; /*in case its not a digit, space, comma, or +/ -we send back an error.*/
                }
            }
            


        }
        if (strcmp(temp_line, "")!=0)
        {
            temp_line[strlen(temp_line) + 1] = '\0';
            strcpy(final_array[j], temp_line); /*We copy our line into our array of strings*/
            strcpy(temp_line, "");
            j++;
        }
        if (was_comma == TRUE || was_symbol == TRUE) { /*Making sure that our data type ends well*/
            printf("data type must end with an int");
            return FALSE;
        }
        final_array[j][0] = '\0';/*sets the last index of the array to be NULL*/
        j = 0;
        while (final_array[j][0] != '\0') {
            temp_num = atoi(final_array[j]); /*This is to take the item isndie the slot of of our array of strings and turn it into an int*/
            if(f_object!= NULL) /*Inside second pass*/
            {
                fprintf(f_object, "%d\t", *decimal_adress);
                if (temp_num < 0) /*In case we have a negative number, we use translate to bits as it handles negative numbers*/
                {
                    if (translate_to_bits(temp_num, arr) == FALSE)
                    {
                        return FALSE;
                    }
                    print_array_to_binary(arr, f_object); /*Print our array that the last function creates into the obj file*/
                }
                else
                {
                    print_to_binary(temp_num, f_object); /*in case its not a negative number, we use print to binary and print to obj file*/
                }
                *decimal_adress += 1;
            }
            else {
                pushInt(temp_num, memory->memory_struct, &memory->DC); /*push our int into the table of DC*/
            }
            j++;
            
        }
        return TRUE; /*If everyting went well, we return TRUE*/
    }

    
    boolean extract_labels_extern_entry(char* str_line, MEMORY* memory, char* skip_type, LABEL_TYPES label_type, FILE* entry_extern, int* decimal_adress) 
    { /*1.9 goes through the string of externs*/
        char* temp_pointer; 
        char* temp_pointer2;
        char* dynamic_label_name;
        LABEL_DATA* label_data;
        char label_name[MAX_LINE_CONTENT];
        delete_end(str_line);
        fix_line_end(str_line);
        temp_pointer = strstr(str_line, skip_type); /*We skip over our type extern/entry*/
        temp_pointer += strlen(skip_type);
	label_name[MAX_LINE_CONTENT - 1] = '\0';
	if (temp_pointer == NULL) {
            printf("temp_pointer is NULL");
            return FALSE;
        }
        if (*temp_pointer != ' ' && *temp_pointer != '\t') { /*in case our line isn't written well without spaces*/
            printf("No space between labels and %s", skip_type);
            return FALSE;
        }
        temp_pointer++;
        while(*temp_pointer!='\0') /*function to handle more than one extern/entry item.*/
        {
            temp_pointer = skip_blank(temp_pointer);
            temp_pointer2 = strstr(temp_pointer, ","); /*A pointer that helps us in case we have more than one item.*/
            if (temp_pointer == temp_pointer2) 
            {
                printf("Invalid placement of comma");
                return FALSE;
            }
            if (temp_pointer == '\0') /*A check in case our type is invalid*/
            {
                printf("Invalid type");
                return FALSE;
            }
            if(temp_pointer2!=NULL)
            {
                strncpy(label_name, temp_pointer, temp_pointer2 - temp_pointer);
		label_name[temp_pointer2 - temp_pointer] = '\0';
            }
            else
            {
                strcpy(label_name, temp_pointer);
                if (strlen(label_name) >= MAX_LABEL_SIZE) /*in case our label that we are trying to input into the file is too big.*/
                {
                    printf("label exceeds the max size of 30");
                    return FALSE;
                }
                if(is_label(temp_pointer) == FALSE) /*in case its not a label that is possible within our world.*/
                {
                    printf("Invalid input of a label");
                    return FALSE;
                }
                *temp_pointer = '\0'; /*This is set here in order to not repeat the loop after discovering the only label*/
            }
            label_data = (LABEL_DATA*)malloc(sizeof(LABEL_DATA)); /*allocating memeory to our label*/
            if (label_data == NULL)
            {
                printf("Failed to allocate memory");
                return FALSE;
            }
            else
            {
                label_data->label_type = label_type; /*setting the type of the label according to entry/extern*/
                dynamic_label_name = (char*)malloc(sizeof(char) * (strlen(label_name) + 1));
                if (dynamic_label_name == NULL)
                {
                    printf("Failed to allocate memory");
                    return FALSE;
                }
                else {
                    if (strlen(label_name) >= MAX_LABEL_SIZE) /*Net in case the size is too big to fit the label*/
                    {
                        printf("label exceeds the max size of 30");
                        return FALSE;
                    }
                    strcpy(dynamic_label_name, label_name);
                }
            }
            if (label_type == ENTRY) /*setting our previously used labels to be entry type.*/
            {
                free(label_data);
                free(dynamic_label_name);
                label_data = (LABEL_DATA*)pull_item_hash(memory->label_hash, label_name, compare_label);
                if (label_data == NULL) 
                {
                    printf("Declared on entry that doesn't exsist in our file.");
                    return FALSE;
                }
                else
                {
                    label_data->label_type = ENTRY;
                }
            }
            if (strlen(label_name) >= MAX_LABEL_SIZE)
            {
                printf("label exceeds the max size of 30");
                return FALSE;
            }
            else /*In case this is an extern, we use this function to handle it.*/
            {
                if (strlen(label_name) >= MAX_LABEL_SIZE)
                {
                    printf("label exceeds the max size of 30");
                    return FALSE;
                }
                if (insert_item_hash(memory->label_hash, dynamic_label_name, label_data, compare_label) == 0) {
                    free(label_data);
                    free(dynamic_label_name);
                }
            }  
        }
        return TRUE;
    }

    int calculate_L(LINE str_line, FUNCTIONS function_type, MEMORY* memory, FILE* f_object, int* decimal_adress, FILE* f_extern)
    {
        LABEL_DATA* label_data;
        char* no_blanks;
        char* temp_pointer;
        char* parentheses_pointer;
        char* temp_pointer1;
        char* jump_label;
        OPERAND_TYPE source_operand, destination_operand;
        char left[MAX_LINE_CONTENT];
        char right[MAX_LINE_CONTENT];
        char jump_array[MAX_LINE_CONTENT];
        int i;
        int temp_num;
        char* pointer_comma;
        fix_line_end(str_line);
        delete_end(str_line);
        if ((temp_pointer = strstr(str_line, ":")) != NULL) {
            no_blanks = strstr(str_line, ":");
            no_blanks++;
            no_blanks = skip_blank(no_blanks);
        }
        else
        {
            no_blanks = str_line;
        }
        if (function_type != STOP) /*start of our function check*/
        {
            if (function_type == RTS) /*If our function is RTS*/
            {
                no_blanks = no_blanks + 3; /*We skip to the next spot that isn't a function*/
                no_blanks = skip_blank(no_blanks);
                if (no_blanks != NULL)
                {
                    printf("RTS cannot recieve operands"); /*RTS is incapable of recieving data*/
                    return 0;
                }
                if (f_object != NULL)
                {
                    /*Printing the data that our line holds as per instruction in second pass*/
                    /*The two last parameters are ignored in non jmp type functions*/
                    first_op_to_binary(decimal_adress, f_object, RTS_OP, FALSE, IMMIDIATE_TYPE, FALSE, IMMIDIATE_TYPE, JUMP_TYPE, JUMP_TYPE);
                }
                return 1;
            }

            if (function_type == PRN)
            {
                no_blanks = no_blanks + 3; /*We skip to the next spot that isn't a function*/
                if (*no_blanks != ' ' && *no_blanks != '\t')
                {
                    printf("There is no space in between the function and the call");
                    return 0;
                }
                no_blanks = skip_blank(no_blanks);
                if (*no_blanks == '#') /*A check for immidiate param*/
                {
                    if (is_digit_only(no_blanks) == FALSE) /*Making sure its set well.*/
                    {
                        printf("# must be followed by an int");
                        return 0;
                    }
                    else {
                        if (f_object != NULL) /*Inside second pass*/
                        {
                            if (is_label(no_blanks)) /*A check that the label was defined in the first pass*/
                            {
                                label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);
                                if (label_data == NULL)
                                {
                                    printf("Label not found in second pass");
                                    return 0;
                                }
                            }
                            /*Printing the data that our line holds as per instruction in second pass*/
                            /*The two last parameters are ignored in non jmp type functions*/
                            first_op_to_binary(decimal_adress, f_object, PRN_OP, FALSE, IMMIDIATE_TYPE, TRUE, IMMIDIATE_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                        }
                        return 2; /*Return 2 as its the max L can be in a single param function*/
                    }
                }
                if (is_label(no_blanks) || is_register(no_blanks))
                {
                    if (f_object != NULL) /*Inside second pass*/
                    {
                        if (is_label(no_blanks) == TRUE) /*A check that the label was defined in the first pass*/
                        {
                            
                            
                                label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);
                                if (label_data == NULL)
                                {
                                    printf("Label not found in second pass");
                                    return 0;
                                }
                                /*Printing the data that our line holds as per instruction in second pass*/
                                /*The two last parameters are ignored in non jmp type functions*/
                            first_op_to_binary(decimal_adress, f_object, PRN_OP, FALSE, LABEL_TYPE, TRUE, LABEL_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                        }
                        else
                        {
                            /*Printing the data that our line holds as per instruction in second pass*/
                            /*The two last parameters are ignored in non jmp type functions*/
                            first_op_to_binary(decimal_adress, f_object, PRN_OP, FALSE, LABEL_TYPE, TRUE, REGISTER_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                        }
                    }
                    return 2; /*Return 2 as its the max L can be in a single param function*/
                }
                printf("invalid prn line");
                return 0;
            }
            if (function_type == NOT || function_type == CLR || function_type == INC || function_type == DEC || function_type == RED)
            {
                no_blanks = no_blanks + 3; /*We skip to the next spot that isn't a function*/
                if (*no_blanks != ' ' && *no_blanks != '\t')
                {
                    printf("There is no space in between the function and the call");
                    return 0;
                }
                no_blanks = skip_blank(no_blanks);
                if (*no_blanks == '#') /*this functions don't accept immidiate functions*/
                {
                    printf("Invalid placement of adress #");
                    return 0;
                }
                if (is_label(no_blanks) || is_register(no_blanks))
                {
                    if (f_object != NULL)
                    {
                        if (is_label(no_blanks) == TRUE)
                        {
                            label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);
                            if (label_data == NULL)  /*A check that the label was defined in the first pass*/
                            {
                                printf("Label not found in second pass");
                                return 0;
                            }
                            /*The two last parameters are ignored in non jmp type functions*/
                            /*Printing the data that our line holds as per instruction in second pass*/
                            first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), FALSE, IMMIDIATE_TYPE, TRUE, LABEL_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                        }
                        else /*handling a register case*/
                        {
                            /*The two last parameters are ignored in non jmp type functions*/
                            /*Printing the data that our line holds as per instruction in second pass*/
                            first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), FALSE, IMMIDIATE_TYPE, TRUE, REGISTER_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                        }
                    }
                    return 2; /*Return 2 as its the max L can be in a single param function*/
                }
                printf("NOT, CLR, INC, DEC, RED,PRN can only have one operand");
                return 0;
            }
            if (function_type == LEA)  /*start of LEA validation check*/
            {
                no_blanks = no_blanks + 3; /*We skip to the next spot that isn't a function*/
                if (*no_blanks != ' ' && *no_blanks != '\t')
                {
                    printf("There is no space in between the function and the call");
                    return 0;
                }
                no_blanks = skip_blank(no_blanks);
                pointer_comma = strstr(no_blanks, ",");
                if (strstr(no_blanks, "#") != NULL) {
                    printf("LEA does not accept immidiate numbers");
                    return 0;
                }
                if (pointer_comma == NULL) /*A check to make sure we have a comma*/
                {
                    printf("Missing a comma");
                    return 0;
                }
                if (strstr(pointer_comma + 1, ",") != NULL)
                {
                    printf("Too many commas or operands");
                    return 0;
                }
                strncpy(left, no_blanks, pointer_comma - no_blanks); /*copy everything from the start to the comma*/
                left[pointer_comma - no_blanks] = '\0';
                strcpy(right, pointer_comma + 1);
                pointer_comma = skip_blank(right);
                delete_end(left);
                delete_end(right);
                no_blanks = left;
                pointer_comma = right; /*Making sure that our pointers are set to the right location*/
                pointer_comma = skip_blank(pointer_comma);
                if (is_label(no_blanks) == FALSE)
                {
                    printf("Invalid first operand");
                    return 0;
                }
                if (is_label(pointer_comma) || is_register(pointer_comma))
                {
                    if (f_object != NULL)
                    {
                        if(is_label(no_blanks) == TRUE)
                        {
                            label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);
                            if (label_data == NULL) /*A check that the label was defined in the first pass*/
                            {
                                printf("Label not found in second pass");
                                return 0;
                            }
                        }
                        if (is_label(pointer_comma) == TRUE)
                        {
                            label_data = pull_item_hash(memory->label_hash, pointer_comma, compare_label);
                            if (label_data == NULL) /*A check that the label was defined in the first pass*/
                            {
                                printf("Label not found in second pass");
                                return 0;
                            }
                            /*The two last parameters are ignored in non jmp type functions*/
                            /*Printing the data that our line holds as per instruction in second pass*/
                            first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), TRUE, LABEL_TYPE, TRUE, LABEL_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                            /*Second run of the function to write the adress of the second param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, pointer_comma, f_extern);
                        }
                        else
                        {
                            /*The two last parameters are ignored in non jmp type functions*/
                            /*Printing the data that our line holds as per instruction in second pass*/
                            first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), FALSE, IMMIDIATE_TYPE, TRUE, REGISTER_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                            /*Second run of the function to write the adress of the second param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, pointer_comma, f_extern);
                        }
                    }
                    return 3; /*Return 3 as its the max a function of to parameters can return*/
                }
                printf("Invalid second operand");
                return 0;
            } /*end of LEA validation check*/
            if (function_type == CMP) /*start of CMP validation check*/
            {
                no_blanks = no_blanks + 3; /*We skip to the next spot that isn't a function*/
                if (*no_blanks != ' ' && *no_blanks != '\t')
                {
                    printf("There is no space in between the function and the call");
                    return 0;
                }
                no_blanks = skip_blank(no_blanks);
                pointer_comma = strstr(no_blanks, ",");
                if (pointer_comma == NULL)
                {
                    printf("Missing a comma");
                    return 0;
                }
                if (strstr(pointer_comma + 1, ",") != NULL)
                {
                    printf("Too many commas or operands");
                    return 0;
                }
                strncpy(left, no_blanks, pointer_comma - no_blanks); /*copy everything from the start to the comma*/
                left[pointer_comma - no_blanks] = '\0';
                strcpy(right, pointer_comma + 1);
                pointer_comma = skip_blank(right);
                delete_end(left);
                delete_end(right);
                no_blanks = left;
                pointer_comma = right; /*Making sure that our pointers are set to the right location*/
                pointer_comma = skip_blank(right);
                if (*no_blanks == '#') /*We skip over the # mark of immidiate adress to make sure an int follows*/
                {
                    if (is_digit_only(no_blanks) == FALSE)
                    {
                        printf("# must be followed by an int");
                        return 0;
                    }
                }
                else
                {
                    if ((is_label(no_blanks) == FALSE) && (is_register(no_blanks) == FALSE)) /*A check that make sures that our operand is valid*/
                    {
                        printf("Invalid first operand");
                        return 0;
                    }
                }
                if (*pointer_comma == '#') /*We skip over the # mark of immidiate adress to make sure an int follows for the second param*/
                {
                    if (is_digit_only(pointer_comma) == FALSE)
                    {
                        printf("# must be followed by an int");
                        return 0;
                    }
                }
                else
                {
                    if ((is_label(pointer_comma) == FALSE) && (is_register(pointer_comma) == FALSE)) /*A check that make sures that our operand is valid for the second operand.*/
                    {
                        printf("Invalid second operand");
                        return 0;
                    }
                }
                if (is_register(pointer_comma) && is_register(no_blanks)) /*Inside two register case*/
                {
                    if (f_object != NULL) /*inside second pass*/
                    {
                        /*Printing the data that our line holds as per instruction in second pass*/
                        /*The two last parameters are ignored in non jmp type functions*/
                        first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), TRUE, REGISTER_TYPE, TRUE, REGISTER_TYPE, JUMP_TYPE, JUMP_TYPE);
                        no_blanks++;
                        pointer_comma++;

                        temp_num = *no_blanks;
                        temp_num -= 48; /*The -48 is to translate back from ascii*/
                        i = *pointer_comma;
                        i -= 48;
                        temp_num = register_op_des(temp_num);
                        i = register_op_des(i);
                        temp_num <<= 6; 
                        i |= temp_num;
                        fprintf(f_object, "%d\t", *decimal_adress);
                        *decimal_adress += 1;
                        print_to_binary(i, f_object); /*printing our double registers*/
                    }
                    return 2; /*We return 2 as we are inside of two registers case and they take the same line of data in one print of obj*/
                }
                if (f_object != NULL) /*inside second pass*/
                {
                    if (is_label(pointer_comma))
                    {
                        label_data = pull_item_hash(memory->label_hash, pointer_comma, compare_label);
                        if (label_data == NULL) /*A check that the label was defined in the first pass*/
                        {
                            printf("Label not found in second pass");
                            return 0;
                        }
                    }
                    if (is_label(no_blanks))
                    {
                        label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);
                        if (label_data == NULL) /*A check that the label was defined in the first pass*/
                        {
                            printf("Label not found in second pass");
                            return 0;
                        }
                    }
                    /*Defining source_operand and destination_operand for easier handling of operand type*/
                    source_operand = type_operand_type(no_blanks, memory);
                    destination_operand = type_operand_type(pointer_comma, memory);
                    if (source_operand == INVALID_OPERAND_TYPE || destination_operand == INVALID_OPERAND_TYPE)
                    {
                        return 0;
                    }
                    /*Printing the data that our line holds as per instruction in second pass*/
                    /*The two last parameters are ignored in non jmp type functions*/
                    first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), TRUE, source_operand, TRUE, destination_operand, JUMP_TYPE, JUMP_TYPE);
                    /*First run of the function to write the adress of the first param.*/
                    operands_to_binary(decimal_adress, f_object, source_operand == REGISTER_TYPE, memory, no_blanks, f_extern);
                    /*Second run of the function to write the adress of the second param.*/
                    operands_to_binary(decimal_adress, f_object, destination_operand == REGISTER_TYPE, memory, pointer_comma, f_extern);
                }
                return 3; /*We return 3 as its the max L that a two operand function can return.*/
            } /*end of CMP validation check*/
            if (function_type == MOV || function_type == ADD || function_type == SUB) /*start of MOV, ADD, SUB Check*/
            {
                no_blanks = no_blanks + 3; /*We skip to the next spot that isn't a function*/
                if (*no_blanks != ' ' && *no_blanks != '\t')
                {
                    printf("There is no space in between the function and the call");
                    return 0;
                }
                no_blanks = skip_blank(no_blanks);
                pointer_comma = strstr(no_blanks, ",");
                if (pointer_comma == NULL)
                {
                    printf("Missing a comma");
                    return 0;
                }
                if (strstr(pointer_comma + 1, ",") != NULL)
                {
                    printf("Too many commas or operands");
                    return 0;
                }
                strncpy(left, no_blanks, pointer_comma - no_blanks); /*copy everything from the start to the comma*/
                left[pointer_comma - no_blanks] = '\0';
                strcpy(right, pointer_comma + 1);
                pointer_comma = skip_blank(right);
                delete_end(left);
                delete_end(right);
                no_blanks = left;
                pointer_comma = right; /*Making sure that our pointers are set to the right location*/
                pointer_comma = skip_blank(pointer_comma);
                if (*no_blanks == '#')
                {
                    if (is_digit_only(no_blanks) == FALSE) /*We skip over the # mark of immidiate adress to make sure an int follows*/
                    {
                        printf("# must be followed by an int");
                        return 0;
                    }
                }
                else if ((is_label(no_blanks) == FALSE) && (is_register(no_blanks) == FALSE)) /*A check that make sures that our operand is valid*/
                {
                    printf("Invalid first operand");
                    return 0;
                }

                if (is_label(pointer_comma) || is_register(pointer_comma)) /*A check that make sures that our operand is valid for the second operand.*/
                {
            

                    if (is_register(pointer_comma) && is_register(no_blanks)) /*Inside two register case*/
                    {
                        if (f_object != NULL) /*Inside second pass*/
                        {

                            first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), TRUE, REGISTER_TYPE, TRUE, REGISTER_TYPE, JUMP_TYPE, JUMP_TYPE);
                            no_blanks++;
                            pointer_comma++;

                            temp_num = *no_blanks;
                            temp_num -= 48; /*The -48 is to translate back from ascii*/
                            i = *pointer_comma;
                            i -= 48;
                            temp_num = register_op_des(temp_num);
                            i = register_op_des(i);
                            temp_num <<= 6;
                            i |= temp_num;
                            fprintf(f_object, "%d\t", *decimal_adress);
                            *decimal_adress += 1;
                            print_to_binary(i, f_object); /*printing our double registers*/
                        }
                        return 2; /*We return 2 as we are inside of two registers case and they take the same line of data in one print of obj*/
                    }
                    if (f_object != NULL) /*Inside second pass*/
                    {
                        if (is_label(pointer_comma))
                        {
                            label_data = pull_item_hash(memory->label_hash, pointer_comma, compare_label);  /*A check that the label was defined in the first pass*/
                            if (label_data == NULL)
                            {
                                printf("Label not found in second pass");
                                return 0;
                            }
                        }
                        if (is_label(no_blanks))
                        {
                            label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);  /*A check that the label was defined in the first pass*/
                            if (label_data == NULL)
                            {
                                printf("Label not found in second pass");
                                return 0;
                            }
                        }
                        /*Defining source_operand and destination_operand for easier handling of operand type*/
                        source_operand = type_operand_type(no_blanks, memory);
                        destination_operand = type_operand_type(pointer_comma, memory);
                        if (source_operand == INVALID_OPERAND_TYPE || destination_operand == INVALID_OPERAND_TYPE)
                        {
                            return 0;
                        }
                        /*Printing the data that our line holds as per instruction in second pass*/
                        /*The two last parameters are ignored in non jmp type functions*/
                        first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), TRUE, source_operand, TRUE, destination_operand, JUMP_TYPE, JUMP_TYPE);
                        /*First run of the function to write the adress of the first param.*/
                        operands_to_binary(decimal_adress, f_object, source_operand == REGISTER_TYPE, memory, no_blanks, f_extern);
                        /*Second run of the function to write the adress of the second param.*/
                        operands_to_binary(decimal_adress, f_object, destination_operand == REGISTER_TYPE, memory, pointer_comma, f_extern);
                    }
                    return 3; /*We return 3 as its the max L that a two operand function can return.*/


                } /*end of MOV, ADD, SUB Check*/
                printf("Invalid second operand");
                return 0;
            }



            if (function_type == BNE || function_type == JMP || function_type == JSR) /*We are in jmp type, therefore jump_type does matter here.*/
            {
                no_blanks = no_blanks + 3; /*We skip to the next spot that isn't a function*/
                if (*no_blanks != ' ' && *no_blanks != '\t')
                {
                    printf("There is no space in between the function and the call");
                    return 0;
                }
                no_blanks = skip_blank(no_blanks);
                pointer_comma = strstr(str_line, ",");
                parentheses_pointer = strstr(str_line, "(");
                if (pointer_comma != NULL) /*We made sure that the function does have a comma and therefore check if it has a parenthesses*/
                {
                    if (parentheses_pointer == NULL) /*We see here that it is indeed invalid and no point in doing more checks.*/
                    {
                        printf("Function lacks parenthesses");
                        return 0;
                    }
                }
                if (pointer_comma == NULL) /*In case we are in jmp that only accepts one label.*/
                {
                    
                    if (parentheses_pointer == NULL)
                    {
                        if (is_label(no_blanks) == FALSE) /*We make sure its followed by a label*/
                        {
                            printf("Invalid input after %d function", function_type);
                            return 0;
                        }
                        if (f_object != NULL)
                        {
                            if (is_label(no_blanks)) /*A check that the label was defined in the first pass*/
                            {
                                label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);
                                if (label_data == NULL)
                                {
                                    printf("Label not found in second pass");
                                    return 0;
                                }
                            }
                            /*Printing the data that our line holds as per instruction in second pass*/
                            /*We are in jmp type, therefore the correct bits will be filled.*/
                            first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), FALSE, JUMP_TYPE, TRUE, JUMP_TYPE, JUMP_TYPE, JUMP_TYPE);
                            /*First run of the function to write the adress of the first param.*/
                            operands_to_binary(decimal_adress, f_object, FALSE, memory, no_blanks, f_extern);
                        }
                        return 2; /*In case its an empty jmp operand function we only return two as it takes 2 slots in the print of obj*/
                    }

                }
		if (pointer_comma + 1 == NULL) { /*A check in case we have a null following.*/
                    return 0;
                }
                if (strstr(pointer_comma + 1, ",") != NULL)
                {
                    printf("Too many commas or operands");
                    return 0;
                }
                parentheses_pointer = strstr(no_blanks, "(");
                strncpy(left, no_blanks, parentheses_pointer - no_blanks); /*copy everything from the start to "("*/
                left[parentheses_pointer - no_blanks] = '\0';
                delete_end(left);
                strcpy(jump_array, left); 
                jump_label = jump_array;
                if (is_label(jump_label) == FALSE) /*making sure that there is no more than one operand before the (*/
                {
                    printf("Invalid placement of operand in a %d function", function_type);
                    return 0;
                }
                no_blanks = strstr(no_blanks, "(");
                no_blanks++; /*pointing to the item after the "("*/
		if (no_blanks == NULL) /*in case our parentheses are following by empty items*/
                {
                    return 0;
                }
                strncpy(right, no_blanks, pointer_comma - no_blanks); /*copy everything from the start to the comma*/
                right[pointer_comma - no_blanks] = '\0'; /*We are creating a mini array to save our second operand inside the parentheses*/
                strcpy(left, pointer_comma + 1);
                pointer_comma = skip_blank(left);
                delete_end(left);
                delete_end(right);
                if (strstr(pointer_comma, ")") == NULL)
                {
                    printf("Missing closing parentheses");
                    return 0;
                }
                temp_pointer1 = strstr(pointer_comma, ")");
                temp_pointer1++;
		if (temp_pointer1 == 0) 
                {
                    return 0;
                }
                if (*temp_pointer1 != '\0') /*We deleted the end, therefore our pointer must point to the end of the line.*/
                {
                    printf("Given extra data after closing parentheses");
                    return 0;
                }
                left[strlen(left) - 1] = '\0'; /*we remove the ")" at the end.*/
                no_blanks = right;
                pointer_comma = left; /*Making sure that our pointers are set to the right location*/
                pointer_comma = skip_blank(pointer_comma);
                if (*no_blanks == '#') /*We skip over the # mark of immidiate adress to make sure an int follows*/
                {
                    if (is_digit_only(no_blanks) == FALSE)
                    {
                        printf("# must be followed by an int");
                        return 0;
                    }
                }
                else
                {
                    if ((is_label(no_blanks) == FALSE) && (is_register(no_blanks) == FALSE)) /*making sure our operand is valid*/
                    {
                        printf("Invalid first operand");
                        return 0;
                    }
                }
                if (*pointer_comma == '#') /*We skip over the # mark of immidiate adress to make sure an int follows*/
                {
                    if (is_digit_only(pointer_comma) == FALSE)
                    {
                        printf("# must be followed by an int");
                        return 0;
                    }
                }
                else
                {
                    if ((is_label(pointer_comma) == FALSE) && (is_register(pointer_comma) == FALSE)) /*making sure our operand is valid*/
                    {
                        printf("Invalid second operand");
                        return 0;
                    }
                }
                if ((is_register(no_blanks) && is_register(pointer_comma)) == TRUE) /*Inside two register case*/
                {
                    if (f_object != NULL)
                    {
                        /*Printing the data that our line holds as per instruction in second pass*/
                        /*We are in jmp type, therefore the correct bits will be filled.*/
                        first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), TRUE, JUMP_TYPE, TRUE, JUMP_TYPE, REGISTER_TYPE, REGISTER_TYPE);
                        operands_to_binary(decimal_adress, f_object, FALSE, memory, jump_label, f_extern);
                        no_blanks++;
                        pointer_comma++;
                        
                        temp_num = *no_blanks;
                        temp_num -= 48; /*The -48 is to translate back from ascii*/
                        i = *pointer_comma;
                        i -= 48;
                        temp_num = register_op_des(temp_num);
                        i = register_op_des(i);
                        temp_num <<= 6; 
                        i |= temp_num;
                        fprintf(f_object, "%d\t", *decimal_adress);
                        *decimal_adress += 1;
                        print_to_binary(i, f_object); /*printing our double registers*/
                    }
                    return 3; /*If both operands are registers we return one less memory.*/
                }
                if (f_object != NULL)
                {
                    if (is_label(pointer_comma)) /*A check that the label of the second operand was defined in the first pass*/
                    {
                        label_data = pull_item_hash(memory->label_hash, pointer_comma, compare_label); 
                        if (label_data == NULL)
                        {
                            printf("Label not found in second pass");
                            return 0;
                        }
                    }
                    if (is_label(no_blanks)) /*A check that the label of the first operand was defined in the first pass*/
                    {
                        label_data = pull_item_hash(memory->label_hash, no_blanks, compare_label);
                        if (label_data == NULL)
                        {
                            printf("Label not found in second pass");
                            return 0;
                        }
                    }
                    /*Defining source_operand and destination_operand for easier handling of operand type*/
                    source_operand = type_operand_type(no_blanks, memory);
                    destination_operand = type_operand_type(pointer_comma, memory);
                    if (source_operand == INVALID_OPERAND_TYPE || destination_operand == INVALID_OPERAND_TYPE)
                    {
                        return 0;
                    }
                    /*Printing the data that our line holds as per instruction in second pass*/
                     /*We are in jmp type, therefore the correct bits will be filled.*/
                    /*There are three operands_to_binary as we have three adress to print.*/
                    first_op_to_binary(decimal_adress, f_object, type_op_code(function_type), TRUE, JUMP_TYPE, TRUE, JUMP_TYPE, source_operand, destination_operand);
                    operands_to_binary(decimal_adress, f_object, FALSE, memory, jump_label, f_extern);
                    operands_to_binary(decimal_adress, f_object, destination_operand == REGISTER_TYPE, memory, no_blanks, f_extern);
                    operands_to_binary(decimal_adress, f_object, FALSE, memory, pointer_comma, f_extern);
                }
                return 4;/*Return 4 as its the max a function of to jmp type can return*/
            }
        }
        else if (function_type == STOP)
        {
            no_blanks = no_blanks + 4; /*We skip to the next spot that isn't a function*/
            no_blanks = skip_blank(no_blanks);
            if (no_blanks != NULL)
            {
                printf("STOP cannot recieve operands");
                return 0;
            }
            if (f_object != NULL) /*inside second pass*/
            {
                /*Printing the data that our line holds as per instruction in second pass*/
                first_op_to_binary(decimal_adress, f_object, STOP_OP, FALSE, IMMIDIATE_TYPE, FALSE, IMMIDIATE_TYPE, JUMP_TYPE, JUMP_TYPE);
            }
            return 1; /*stop can only return one*/
        }
        printf("invalid function"); /*In case our function was not identified at any point.*/
        return 0;
    }
    
    boolean write_label_to_binary(char* label_name, HASHTABLE* label_hash, FILE* f_object, int* decimal_adress) 
    {
        LABEL_DATA* value;
        int data;
        value = (LABEL_DATA*)pull_item_hash(label_hash, label_name, compare_label); /*We extract our label from our table*/
        if(value == NULL) /*In case the label wasn't found in the first pass, this function only runs in the second pass.*/
        {
            printf("Lable doesn't exsist");
            return FALSE;
        }
        data = value->label_decimal_base; /*We extract the decimal adress of the label*/
        fprintf(f_object, "%d\t", *decimal_adress);
        print_to_binary(data, f_object); /*We print our label to binary into the obj file*/
        *decimal_adress += 1;
        return TRUE;
    }

    void first_op_to_binary(int* decimal_adress, FILE* f_object, int op_code, boolean is_source, OPERAND_TYPE source_type, boolean is_destination, OPERAND_TYPE destination_type, OPERAND_TYPE first_param, OPERAND_TYPE second_param)
    {
        int temp_num;
        int param1_op;
        int param2_op;
        param1_op = 0;
        param2_op = 0;
        temp_num = 0;
        fprintf(f_object, "%d\t", *decimal_adress);
        if(is_source==TRUE) /*In the case we are sending a source type to the function this if will handle it*/
        {
            if(source_type== LABEL_TYPE)
            {
                temp_num = LABEL_TYPE_DESTINATION_OP<<2; /*We push the item two bits*/
            }
            else if (source_type == JUMP_TYPE) /*This is in the case we are in a jump function*/
            {
                if(first_param == LABEL_TYPE) /*The bits are placed inside the end of the print 13-12*/
                {
                    param1_op = LABEL_TYPE_DESTINATION_OP << 10;
                }
                else if (first_param == REGISTER_TYPE)
                {
                    param1_op = REGISTER_TYPE_DESTINATION_OP << 10;
                }
                temp_num |= param1_op;
            }
            else if (source_type == REGISTER_TYPE) /*General case when we are not in JMP*/
            {
                temp_num = REGISTER_TYPE_DESTINATION_OP << 2;
            }
            op_code |= temp_num; /*Writing onto op_code to be printed later, marking 1s where 0s were.*/
        }
        if (is_destination == TRUE) /*the destination case.*/
        {
            if (destination_type == LABEL_TYPE)
            {
                temp_num = LABEL_TYPE_DESTINATION_OP;
            }
            else if (destination_type == JUMP_TYPE)
            {
                temp_num = JUMP_TYPE_DESTINATION_OP; /*This is in the case we are in a jump function*/
                if (second_param == LABEL_TYPE)
                {
                    param2_op = LABEL_TYPE_DESTINATION_OP << 8; /*The bits are placed inside the end of the print 11-10*/
                }
                else if (second_param == REGISTER_TYPE)
                {
                    param2_op = REGISTER_TYPE_DESTINATION_OP << 8;
                }
                temp_num |= param2_op; /*Writing onto op_code to be printed later, marking 1s where 0s were.*/
                
            }
            else if (destination_type == REGISTER_TYPE) /*General case when we are not in JMP*/
            {
                temp_num = REGISTER_TYPE_DESTINATION_OP;
            }
            op_code |= temp_num;
        }
        print_to_binary(op_code, f_object); /*sending op_code to be printed.*/
        *decimal_adress += 1;
    }

    boolean operands_to_binary(int* decimal_adress, FILE* f_object, boolean is_source, MEMORY* memory, char* string, FILE* f_extern)
    { 
        LABEL_DATA* label_data;
        int temp_num;
        int temp_code;
        int immidiate_arr[BIT_SIZE];
        int arr[BIT_SIZE];
        OPERAND_TYPE type;
        temp_code = 0;
        if(is_label(string) == TRUE) /*Determining the type sent and setting it to type*/
        {
            type = LABEL_TYPE;
        }
        else if (is_register(string) == TRUE)
        {
            type = REGISTER_TYPE;
        }
        else
        {
            type = IMMIDIATE_TYPE;
        }
        fprintf(f_object, "%d\t", *decimal_adress);
        *decimal_adress += 1;
         if(type == IMMIDIATE_TYPE)
         {
             temp_num = atoi(string + 1);
             if (temp_num < 0) /*in case we found an immidiate we send to either translate_to_bits to handle negative number*/
             {
                 if (translate_to_bits(temp_num, arr) == FALSE)
                 {
                     return FALSE;
                 }
                 immidiate_arr[0] = 0;
                 immidiate_arr[1] = 0;
                 memcpy(&immidiate_arr[2], &arr[0], (BIT_SIZE - 2) * sizeof(int)); /*We asign the last two digits to be zero as its an immidiate array*/

                 print_array_to_binary(immidiate_arr, f_object);
             }
             if (temp_num >= 0) 
             {
                 print_to_binary(temp_num, f_object); /*otherwise we handle it with print to binary*/
             }

         }
         else if (type == LABEL_TYPE) /*in case of a label, we drag it out of our hash*/
         {
             label_data = (LABEL_DATA*)pull_item_hash(memory->label_hash, string, compare_label);
             
             if (label_data == NULL) {
                 return FALSE;
            }
             temp_code = label_data->label_decimal_base; 
             if (label_data->label_type != EXTERNAL) /*If we are not handling an external, we will proceed as usual*/
             {
                 temp_code += 100; /*increasing by the value of IC 100*/
                 temp_code <<= 2;
             }
             if (label_data->label_type == EXTERNAL) /*in the case of it being external, we print it into our ext file*/
             {
                 fprintf(f_extern, "%s", string);
                 fprintf(f_extern, "\t%d\n", *decimal_adress-1);
                 temp_code = 0;
                 temp_code |= 0x1;
             }
             else
             {
                 temp_code |= 0x2;
             }
             print_to_binary(temp_code, f_object); /*Sending to be printed to f_object*/

         }
         else if (type == REGISTER_TYPE)
         {
             string++; /*advance the string pointer to the number*/
             temp_num = atoi(string);
                 temp_num = register_op_des(temp_num); /*We check which register it is and return its binary code*/
             if (is_source == TRUE)
             {
                 temp_num = temp_num << 6; /*We push it into place*/
                 
             }
             print_to_binary(temp_num, f_object); /*printing our register.*/
         } 
         return TRUE; /*All went well, nice job :)*/
    }
    OPERAND_TYPE type_operand_type(char* string, MEMORY* memory) 
    {

        LABEL_DATA* item; /*We use the item in order to understand what sort operand we are currently using in case of a label*/
        if (is_label(string) == TRUE)
        {
            item = (LABEL_DATA*)pull_item_hash(memory->label_hash, string, compare_label);
            if (item == NULL)
            {
                printf("%s label wasn't defined", string);
                return INVALID_OPERAND_TYPE;
            }
            return LABEL_TYPE;
        }
        else if (is_register(string) == TRUE) /*In case of a register we use is_register.*/
        {
            return REGISTER_TYPE;
        }
        else if (*string == '#')
        {
            if (is_digit_only(string) == FALSE) /*A check if its a valid immediate number*/
            {
                printf("# must be followed by an int");
                return INVALID_OPERAND_TYPE;
            }
            else
            {
                return IMMIDIATE_TYPE;
            }


        }
        else
        {
            return JUMP_TYPE; /*last case scenario its a jump function.*/
        }
    }
