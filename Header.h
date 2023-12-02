
#ifndef HEADER_H

#define HEADER_H

// standdard library inclusion
#include <stdint.h>

// used defined micro's for main file
#define ADD 0x0010
#define MUL 0x0020
#define SUB 0x0040
#define DIV 0x0080

// declaration of used defined functions, defined in Support.c
void Dec_To_Binary(unsigned n);
bool readKnobInputs(unsigned *dataPtr);
void writeLedOutput(unsigned data);
uint8_t read_operand_value(uint16_t, uint32_t *reg_address);
void Log_to_File(bool date_stamp, const char *message, ...);

#endif

/*
#ifndef, #define, #endif are preprocessor directives are used together,
and known as "INCLUDE GUARD", this will prevent same header file,
from being included multiple time, which could lead to redeclaration 
of symbol and other problems.
*/