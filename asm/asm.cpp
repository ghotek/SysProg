#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "universal_stack.h"
#include "file_read.h"
#include "asm.h"

/*
    проверять наличие аргументов можно с помощи sscnaf см. тхт на раб столе
*/
int main() {

    COMMAND commands_arr[NUMBER_OF_COMMANDS] = {};
    FILE_READ code_file;
    FILE_READ asm_file;

    code_file.file_name = "code.txt";
    asm_file.file_name  = "asm.txt" ;

    command_add("push", 1, 1, 0);
    command_add("pop",  2, 0, 1);
    command_add("dif",  3, 0, 2);
    command_add("mul",  4, 0, 3);
    command_add("add",  5, 0, 4);
    command_add("sub",  6, 0, 5);
    command_add("out",  7, 0, 6);
    command_add("hlt",  8, 0, 7);

    for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
        printf("# name : {%s}\t\tcode : {%#x}\n", commands_arr[i].command_name,
                                                  commands_arr[i].command_code);

    Dump(FileRead(&code_file));

    for (char* i = code_file.text; i < code_file.text + code_file.file_sz; i++)
        printf("%c", *i);

    Assembler(&code_file, &asm_file, commands_arr);

    return 0;
}

void ErrorTreatment(int err, const char* func_name, size_t n_line) {

    switch (err) {

        case 0:
            break;

        case OPEN_ERR:
            printf("# Line : {%d} in %s reason is OPEN_ERR\n", n_line, func_name);
            break;

        case READ_ERR:
            printf("# Line : {%d} in %s reason is READ_ERR\n", n_line, func_name);
            break;

        case END_ERR:
            printf("# Line : {%d} in %s reason is END_ERR\n", n_line, func_name);
            break;

        case BUF_MEM_ERR:
            printf("# Line : {%d} in %s reason is BUF_MEM_ERR\n", n_line, func_name);
            break;

        default:
            printf("# Line : {%d} in %s reason is unknown error\n", n_line, func_name);
            break;
    };
}

int Assembler(FILE_READ* code_file, FILE_READ* asm_file, COMMAND com_arr[]) {

    assert(com_arr);
    assert(code_file);
    assert(code_file->text);

    Stack stk;

    StackUniversalCtor(&stk);

    char* tmp = NULL;
    char* str = code_file->text;
    char command[20];
    double arg = 0;

    while((tmp = strchr(str, '\n'))) {

        if (sscanf(str, "%s%lg", command, &arg) == 2) {

            for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
                if (com_arr[i].args_exist)
                    if (!strcmp(com_arr[i].command_name, command)) {

                        StackUniversalPush(&stk, &com_arr[i].command_code, COMMAND_SIZE);
                        StackUniversalPush(&stk, &arg, VARIABLE_SIZE);
                    }
        } else {

           for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
                if (!com_arr[i].args_exist)
                    if (!strcmp(com_arr[i].command_name, command))
                        StackUniversalPush(&stk, &com_arr[i].command_code, COMMAND_SIZE);

        }

        str = tmp + 1;
    }

    DumpIt(&stk);

    printf("\n\n FILE WRITE :: %d \n\n", FileWrite(stk.data, stk.sz, asm_file));

    StackDtor(&stk);

    return 0;
}

