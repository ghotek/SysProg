#define DOUBLE_STACK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "universal_stack.h"
#include "file_read.h"
#include "processor.h"

int main() {

    char* ip = NULL;
    char* value = NULL;
    double out = 0;

    size_t flag = 1;

    Stack st;
    FILE_READ asm_file;

    asm_file.file_name = "asm.txt";

    FileRead(&asm_file);

    ip = asm_file.text;

    StackCtor(&st, double);

    while (flag)
        switch (*ip) {

            case 0x1:
                value = ip + 1;
                StackPushD(&st, value);

                ip = value + sizeof(double);
                break;

            case 0x2:
                StackPopD(&st, &out);
                ip += 1;
                break;

            case 0x3:
                printf("dif\n");
                ip += 1;
                break;

            case 0x4:
                printf("mul\n");
                ip += 1;
                break;

            case 0x5:
                printf("add\n");
                add(&st);
                ip += 1;
                break;

            case 0x6:
                printf("sub\n");
                sub(&st);
                ip += 1;
                break;

            case 0x7:
                printf("out\n");
                StackPopD(&st, &out);
                printf("\npopped item : {%lg}\n", out);
                ip += 1;
                break;

            case 0x8:
                printf("hlt\n");
                flag = 0;
                break;

            default:
                printf("Shit happens\tip : {%#x}\n", *ip);
                break;
        };

    DumpIt(&st);

    StackDtor(&st);

    return 0;
}
//считываю в стек всю инфу из файла и parse этот текст

/*
    command_add("push", 1, 1, 0);  +
    command_add("pop",  2, 0, 1);  +
    command_add("dif",  3, 0, 2);  -
    command_add("mul",  4, 0, 3);  +
    command_add("add",  5, 0, 4);  +
    command_add("sub",  6, 0, 5);  +
    command_add("out",  7, 0, 6);  +
    command_add("hlt",  8, 0, 7);  +
*/

int add(Stack* stk) {

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    double add_res = val1 + val2;

    StackPushD(stk, &add_res);

    return 0;
}

int sub(Stack* stk) {

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    double add_res = val1 - val2;

    StackPushD(stk, &add_res);

    return 0;
}

int mul(Stack* stc) {

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    double add_res = val1 * val2;

    StackPushD(stk, &add_res);

    return 0;
}

int dif(Stack* stc) {

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    double add_res = val1 / val2;

    StackPushD(stk, &add_res);

    return 0;
}
