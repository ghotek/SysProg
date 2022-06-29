#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include <stddef.h>

typedef struct {

   const char* command_name;
   char        command_code;
} COMMAND;

typedef struct {

    char mem;
    char reg;
    char imm;
} COMMAND_INFO;

typedef struct {

    char*  label_name ;
    size_t label_place;
} LABEL;

typedef struct {

    LABEL* label_arr   ;
    size_t labels_count;
} LABELS;

const int NUMBER_OF_COMMANDS = 17;
const int NO_LABELS          = 34;

#define command_add(name, code, number)                            \
        commands_arr[number].command_name = name;                  \
        commands_arr[number].command_code = code;

typedef enum {

    VARIABLE_SIZE     = sizeof(double),
    REG_SIZE          = sizeof(char)  ,
    COMMAND_SIZE      = sizeof(char)  ,
    LABEL_SIZE        = sizeof(size_t),
    COMMAND_INFO_SIZE = sizeof(COMMAND_INFO)
} COMMANDS_ASSEMB;

//-----------------

int Assembler(FILE_READ* code_file, FILE_READ* asm_file,
              COMMAND    com_arr[], LABELS* labels);
int LabelsListCtor(FILE_READ* code_file, LABELS* labels);
int LabelsListDtor(LABELS* labels);
size_t CurrentPlace(char* label_place, LABELS* labels, FILE_READ* code_file);

#endif // ASM_H_INCLUDED
