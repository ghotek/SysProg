#define DOUBLE_STACK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include "universal_stack.h"
#include "file_read.h"
#include "processor.h"

// unsafe double comparing in jumps, to do EPS = 10e-7
/*  todo

func:
    ...
    ret

call func
*/

int main() {

    char* ip    = NULL;
    char* info  = NULL;
    char* value = NULL;
    double arg  = 0   ;

    int mem_flag = 0;
    int reg_flag = 0;
    int imm_flag = 0;
    int hlt_flag = 1;

    double REG[REG_AMOUNT] = {};
    int reg_name = 0;

    char* RAM = NULL;
    int ram_size = 2;

    if (!(RAM = (char*)calloc(ram_size, sizeof(char))))
        return BUF_MEM_ERR;

    Stack st;
    FILE_READ asm_file;

    asm_file.file_name = "D:\\progs\\prj\\DedHw\\asm\\asm.txt";

    FileRead(&asm_file);

    info = asm_file.text;

    ip = info + 3;

    StackCtor(&st, double);

    while(hlt_flag)
        switch(*ip) {

            case CMD_PUSH:
                if (*info)       mem_flag = 1;
                if (*(info + 1)) {

                    if (*(ip + 1) == 'a') reg_name = 0;
                    if (*(ip + 1) == 'b') reg_name = 1;
                    if (*(ip + 1) == 'c') reg_name = 2;
                    if (*(ip + 1) == 'd') reg_name = 3;

                    reg_flag = 1;
                }
                if (*(info + 2)) imm_flag = 1;

                if (mem_flag) {

                    if (reg_flag && imm_flag) {

                        value = ip + 2;
                        arg = *(double*)value + REG[reg_name];

                        ram_size += (int)arg;

                        info = value + sizeof(double);
                        ip = info + 3;
                    } else {

                        if (reg_flag) {

                            ram_size += (int)REG[reg_name];

                            info = ip + 2;
                            ip   = info + 3;
                        } else {

                            value = ip + 1;
                            ram_size += (int)(*(double*)value);

                            info = value + sizeof(double);
                            ip = info + 3;
                        }
                    }

                    if (!(RAM = (char*)realloc(RAM, ram_size * sizeof(char))))
                        return BUF_MEM_ERR;

                    mem_flag = 0;
                    reg_flag = 0;
                    imm_flag = 0;

                    break;
                } else {

                    if (reg_flag && imm_flag) {

                        value = ip + 2;
                        arg = *(double*)value + REG[reg_name];

                        StackPushD(&st, &arg);

                        info = value + sizeof(double);
                        ip = info + 3;
                    } else {

                        if (reg_flag) {

                            StackPushD(&st, REG + reg_name);

                            info = ip + 2;
                            ip   = info + 3;
                        } else {

                            value = ip + 1;
                            StackPushD(&st, value);

                            info = value + sizeof(double);
                            ip = info + 3;
                        }
                    }

                    reg_flag = 0;
                    imm_flag = 0;

                    break;
                }

                break;

            case CMD_POP:

                if (*(info + 1)) {

                    if (*(ip + 1) == 'a') reg_name = 0;
                    if (*(ip + 1) == 'b') reg_name = 1;
                    if (*(ip + 1) == 'c') reg_name = 2;
                    if (*(ip + 1) == 'd') reg_name = 3;

                    reg_flag = 1;
                }

                if (reg_flag) {

                    StackPopD(&st, &arg);
                    REG[reg_name] = arg;

                    info = ip + 2;
                    ip = info + 3;
                    reg_flag = 0;

                    break;
                } else {

                    StackPopD(&st, &arg);

                    info = ip + 1;
                    ip = info + 3;
                    break;
                }

                break;

            case CMD_DIF:

                dif(&st);

                info = ip + 1;
                ip = info + 3;
                break;

            case CMD_MUL:

                mul(&st);

                info = ip + 1;
                ip = info + 3;
                break;

            case CMD_ADD:

                add(&st);

                info = ip + 1;
                ip = info + 3;
                break;

            case CMD_SUB:

                sub(&st);

                info = ip + 1;
                ip = info + 3;
                break;

            case CMD_JA:

                if (ja(&st)) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_JAE:

                if (jae(&st)) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_JB:

                if (jb(&st)) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_JBE:

                if (jbe(&st)) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_JE:

                if (je(&st)) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_JNE:

                if (jne(&st)) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_JM:

                if (jm()) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_JF:

                if (jf()) {

                    info = asm_file.text + *(size_t*)(ip + 1);
                    ip = info + 3;
                } else {

                    info = ip + 1 + sizeof(size_t);
                    ip = info + 3;
                }

                break;

            case CMD_IN:

                printf("in ");
                scanf("%lg", &arg);

                StackPushD(&st, &arg);

                info = ip + 1;
                ip = info + 3;
                break;

            case CMD_OUT:

                if (StackPopD(&st, &arg) == STACK_UNDERFLOW)
                    return STACK_UNDERFLOW;

                printf("out item : {%lg}\n", arg);

                info = ip + 1;
                ip = info + 3;
                break;

            case CMD_HLT:
                hlt_flag = 0;
                break;

            default:
                printf("Shit happens\tip : {%#x}\n", *ip);
                hlt_flag = 0;
                break;
        };

    StackDtor(&st);

    return 0;
}

int add(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    double add_res = val1 + val2;

    StackPushD(stk, &add_res);

    return 0;
}

int sub(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    double sub_res = val1 - val2;

    StackPushD(stk, &sub_res);

    return 0;
}

int mul(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    double mul_res = val1 * val2;

    StackPushD(stk, &mul_res);

    return 0;
}

int dif(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    assert(val2);

    double dif_res = val1 / val2;

    StackPushD(stk, &dif_res);

    return 0;
}

int ja(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    StackPushD(stk, &val2);
    StackPushD(stk, &val1);

    return val1 > val2;
}

int jae(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    StackPushD(stk, &val2);
    StackPushD(stk, &val1);

    return val1 >= val2;
}

int jb(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    StackPushD(stk, &val2);
    StackPushD(stk, &val1);

    return val1 < val2;
}
int jbe(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    StackPushD(stk, &val2);
    StackPushD(stk, &val1);

    return val1 <= val2;
}

int je(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    StackPushD(stk, &val2);
    StackPushD(stk, &val1);

    return val1 == val2;
}

int jne(Stack* stk) {

    assert(stk);

    double val1 = 0, val2 = 0;

    StackPopD(stk, &val1);
    StackPopD(stk, &val2);

    StackPushD(stk, &val2);
    StackPushD(stk, &val1);

    return val1 != val2;
}

int jm() {

    time_t curr_time = time(NULL);
    char *time_str = ctime(&curr_time);

    if (strstr(time_str, "Mon"))
        return 1;

    return 0;
}

int jf() {

    time_t curr_time = time(NULL);
    char *time_str = ctime(&curr_time);

    if (strstr(time_str, "Fr"))
        return 1;

    return 0;
}
