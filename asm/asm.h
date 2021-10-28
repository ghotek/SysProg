#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include <stddef.h>

typedef struct {

   const char* command_name;
   char command_code;
   int args_exist;
} COMMAND;

const int NUMBER_OF_COMMANDS = 8;

#define command_add(name, code, exist, number)                     \
        commands_arr[number].command_name = name;                  \
        commands_arr[number].command_code = code;                  \
        commands_arr[number].args_exist = exist;

void ErrorTreatment(int err, const char* func_name, size_t n_line);

#define Dump(err) \
        ErrorTreatment(err, __PRETTY_FUNCTION__, __LINE__)

typedef enum {

    VARIABLE_SIZE = 8,
    COMMAND_SIZE  = 1
} COMMANDS_ASSEMB;

//-----------------

int Assembler(FILE_READ* code_file, FILE_READ* asm_file,COMMAND com_arr[]);

#endif // ASM_H_INCLUDED
