// inclusion of C defined Library
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

// used define header file
#include "Header.h"

/*
This function reads user data and operation data from register (our case we asked user to enter those register vallue manually),
devide them into left, right and operation operand using bitwise operation
stores it as 32 bits unsigened into address of input variable of main function
where; 0 to 3 bits- left and 8 to 11bits - right operands
bit 4, 5, 6, 7, for Addition, Multiplication, Subtraction and Division respectively.
return true on sucessful reading from register and writing into input varaible
*/
bool readKnobInputs(unsigned *dataPtr)
{
    uint32_t user_input = 0;
	printf("\nEnter User_input Register value: ");
	scanf("%x", &user_input);
	
	uint32_t user_operation = 0;
	printf("\nEnter User_Operation Register value: ");
	scanf("%x", &user_operation);
	
    Log_to_File(true, "Storing User Entered Register Data\n");
    Log_to_File(true, "User Input: 0x%08X\n", user_input);
    Log_to_File(true, "User Operation: 0x%08X\n", user_operation);

	uint32_t left_operand = 0;
	uint32_t right_operand = 0;

	
	left_operand = (uint32_t)read_operand_value(16, &user_input);
	right_operand = (uint32_t)read_operand_value(0, &user_input);

	
	printf("\nleft_operand value is: %d", left_operand);
    Log_to_File(true, "Left-Operand value: %d\n", left_operand);

	printf("\nright_operand value is: %d", right_operand);
    Log_to_File(true, "Right-Operand value: %d\n", right_operand);



    if(left_operand > 16 || right_operand > 16 || user_operation > 8){
        return false;
    }

    *dataPtr = left_operand | (user_operation << 4) | (right_operand << 8);

    printf("\n\nData Sending Package: 0x%08x", *dataPtr);
    Log_to_File(true, "Data is ready to be stored into input variable address\n");
    Log_to_File(true, "Data package at Read-Knob Function:\t0x%08X\n", *dataPtr);

    return true;
}

/*
Function will take unsigned data as input and store into output register
(our case we are displaying into terminal for better understanding)
*/
void writeLedOutput(unsigned data)
{
    // this function will take unsigned integer data varialble which carries 
    // result from arithmatic operations and error value, and separate them into 
    // result and error and display them the way user want.

    Log_to_File(true, "Result Data Received into writeLedOutput Function\n");

    Log_to_File(true, "Storing result for better understanding\n");
    Log_to_File(true, "Result in integer form:\t%d\n", (int8_t)(data>>1));


    Dec_To_Binary(data>>1);
    
    // printing Error LED bit
    printf("\tError = %d\n", data&0x01);
    Log_to_File(false, "\tError = %d\n", data&0x01);

}

/*
To display data into Binary form for better user understanding
*/
void Dec_To_Binary(unsigned n)
{
    int numberOfBits = 8; // number of bits we want for our binary conversation
    int bitmask = 1 << (numberOfBits -1);

    // below logic is for displaying integer into binary.

    printf("\nBinay Value of Result is: ");
    Log_to_File(true, "Binay Value of Result is: ");

    for (int i = 0; i < numberOfBits; i++)
    {
        // Use a bitwise AND operation to check the bit at the current position
        
        if (n & bitmask) 
        {
            printf("1");
            Log_to_File(false, "1");
        } 
        else 
        {
            printf("0");
            Log_to_File(false, "0");
        }

        // Shift the bitmask to the right for the next bit
        bitmask >>= 1;
    }

}

/*
Run though memory address bits from given bits range and scan which bit is active high
and return that value as operand as bit 0 active is same as 0 input from user
bit 15 correspond to F as user entered data.
takes two arguments, first starting position from where we want to start scanning
second, address of register from where we want to scan.
*/

uint8_t read_operand_value(uint16_t start_pos, uint32_t *reg_address)
{
    uint16_t bitmask = 1;
    uint8_t operand = 0;
	uint32_t input_data = *reg_address;
    
    
	if(start_pos > 15)
	{
		input_data >>= 16;
	}
	
	for(uint8_t i = 0; i < 16; i++)
	{
		if(input_data & bitmask)
			{
				operand = i;
				break;
			}
		bitmask <<= 1;
	}
	return operand;
}

/*
it can take multiple arguments; but first two are mendatory
frist; ask user to include date and time stamp
second; string message to log
... means it can take undeclared number of arguments at time of use
return log message with/without time stamp into defined text file
*/

void Log_to_File(bool date_stamp, const char *message, ...)
{
    
    FILE *file = fopen("/Users/rajbhadania/C_Projects/Embedded_Projects/Calculator_Data_log.txt", "a");
    // declaration of FILE type pointer to 'file' variable
    // fopen will open file into append mode at given address
    // on sucess it will return pointer to 'file' if not than return NULL to 'file' pointer

    if(file != NULL)  // if file was opened sucessfully than will execute if block
    {
        time_t current_time;  // declare current_time variable of type time_t

        time(&current_time);  // this will store current calender time as single arithmatic value into address of current_time variable

        struct tm *local_time = localtime(&current_time); // fill local_time struct of tm type with values that represent the corresponding local time.

        char *ascitime_return = asctime(local_time); // store that string return of ascitime function into char array
        ascitime_return[strcspn(ascitime_return, "\n")] = '\0'; // modify string where \n character of ascitime_return will be modified to NULL
                // to terminate string from there
                // strcspn function used to find first occurance of \n and it returns integer which is index of \n character so we can replace with NULL 

        va_list args; // go through attached source for better understanding
        va_start(args, message);
        
        if(date_stamp){

            fprintf(file, "[%s]\t\t", ascitime_return);
            vfprintf(file, message, args);
        }else{
            vfprintf(file, message, args);
        }
        va_end(args);

        fclose(file);
    }
}
// Source:
// https://www.tutorialspoint.com/c_standard_library/c_function_asctime.htm
// https://www.tutorialspoint.com/c_standard_library/c_function_vfprintf.htm

// Read Knob Function defination to get data from user directly
// Can be used for problem statement one
// In case of Reading Data from user use this function


/*
bool readKnobInputs(unsigned *dataPtr)
{
    unsigned int left;
    unsigned int right;
    unsigned int operator;
    char operation;
 
    printf("Enter Left Operand value in HEX: ");
    scanf("%x", &left);
    printf("Enter Right Operand value in HEX: ");
    scanf("%x", &right);

    printf("\nChoose Operation from +, -, *, and /: ");
    scanf(" %c", &operation);

    // switch case will compare operation value and set operator according to that
    // if user enter any other operation than listed it will ask user to enter value again
    
    switch(operation)
    {
        case '+': operator = 0x0010;
                  break;
        case '-': operator = 0x0040;
                  break;
        case '*': operator = 0x0020;
                  break;
        case '/': operator = 0x0080;
                  break;
        default:  printf("\nInvalid Operation\n");
                  return false;
    }

    *dataPtr = left | operator | (right << 8);
    printf("\n\nleft = %d   operator = 0x%04x  right = %d\n", left, operator, right);

    return true;
}

*/