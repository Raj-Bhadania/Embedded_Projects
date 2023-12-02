#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Header.h"


int main(void)
{
    while(1)
    {
        unsigned int input;
        bool readKnob = readKnobInputs(&input);

        if (readKnob == false)
        {
            continue;
        }
        unsigned int result = 0;
        int8_t error = 0;
        unsigned int led = 0;

        // using bitwise operation derive left right and operator value from input variable

        unsigned int left = input & 0x000F;
        unsigned int operator = input & 0x00F0;
        unsigned int right = (input & 0x0F00) >> 8;

        Log_to_File(true, "Value of operands in Main\n");
        Log_to_File(true, "Left: %d\tOperator: %04x\tRight: %d\n", left, operator, right);

        switch(operator){
            case ADD:
                    result = left + right;
                    break;
            
            case SUB: 
                    result = left - right;
                    break;

            case MUL:
                    result = left * right;
                    if(result > 127){
                        error = 1;
                        result = 0;
                    }
                    break;

            case DIV:
                    if(right == 0){
                        error = 1;
                        result = 0;
                        break;
                    }       // this will not perform operation if right operand is Zero.
                    result = left / right;
                    break;

            default:
                    error = 1;
                    result = 0;
                    break;

        }

        Log_to_File(true, "Storing Value of Result and Error after Arithmatic Opearation in Main\n");
        Log_to_File(true, "Result Value in Main: %d\n", result);
        Log_to_File(true, "Result Value in Main: %d\n", error);

        led = (result << 1) | error;
        writeLedOutput(led);

        char decision;
        printf("\nDo you want to perform calculation, enter 'y' for yes 'n' for no:\t");
        scanf(" %c", &decision);

        if(decision == 'n')
        {
            Log_to_File(false, "\n\n<-<-<-<-<-USER CHOSE TO EXIT FROM APPLICATION->->->->->\n\n");
            exit(1);
        }

        Log_to_File(false, "\n\n<-<-<-<-<-<-<-<-<-<-USER WANTS TO CONTINUE->->->->->->->->->->\n\n");

    }
    return 0;
}