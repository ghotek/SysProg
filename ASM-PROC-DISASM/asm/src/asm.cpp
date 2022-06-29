#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "universal_stack.h"
#include "file_read.h"
#include "asm.h"

int main() {

    COMMAND   commands_arr[NUMBER_OF_COMMANDS] = {};
    LABELS    labels                               ;

    FILE_READ code_file;
    FILE_READ asm_file ;

    code_file.file_name = "code.txt";
    asm_file.file_name  = "asm.txt" ;

    int err = 0;

    command_add("push",   1,  0);
    command_add("pop" ,   2,  1);
    command_add("dif" ,   3,  2);
    command_add("mul" ,   4,  3);
    command_add("add" ,   5,  4);
    command_add("sub" ,   6,  5);
    command_add("out" ,   7,  6);
    command_add("ja"  ,   8,  7);
    command_add("jae" ,   9,  8);
    command_add("jb"  ,  10,  9);
    command_add("jbe" ,  11, 10);
    command_add("je"  ,  12, 11);
    command_add("jne" ,  13, 12);
    command_add("jm"  ,  14, 13);
    command_add("jf"  ,  15, 14);
    command_add("hlt" ,  16, 15);
    command_add("in"  ,  17, 16);
    command_add("call",  18, 17);

    if ((err = FileRead(&code_file)))
        return err;

    if ((err = LabelsListCtor(&code_file, &labels)) &&
        (err != NO_LABELS))
        return err;

    if ((err = Assembler(&code_file, &asm_file,
                          commands_arr, &labels)))
        return err;

    if ((err = LabelsListDtor(&labels)))
        return err;

    printf("Assembled\n");

    return 0;
}

int Assembler(FILE_READ* code_file, FILE_READ* asm_file, COMMAND com_arr[], LABELS* labels) {

    assert(com_arr);
    assert(code_file);
    assert(code_file->text);
    assert(labels);

    Stack stk;

    StackUniversalCtor(&stk);

    char*  tmp         = NULL           ;
    char*  str         = code_file->text;
    char   command[20] = {}             ;
    char   lab[20]     = {}             ;
    double arg         = NAN            ;
    size_t lab_place   = 0              ;

    size_t ok        = 0;
    char   reg_name  = 0;

    COMMAND_INFO cur_cmd;

    while((tmp = strchr(str, '\n')) && (str < code_file->text + code_file->file_sz)) {

        if (*str == ':') {

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s [%cx + %lg]%n", command, &reg_name, &arg, &ok);

        if (ok) {

            cur_cmd.mem = 1;
            cur_cmd.reg = 1;
            cur_cmd.imm = 1;

            StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
            StackUniversalPush(&stk, &com_arr[0].command_code, COMMAND_SIZE     );
            StackUniversalPush(&stk, &reg_name               , REG_SIZE         );
            StackUniversalPush(&stk, &arg                    , VARIABLE_SIZE    );

            reg_name = 0  ;
            arg      = NAN;
            ok       = 0  ;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s [%cx]%n", command, &reg_name, &ok);

        if (ok && isalpha(reg_name)) {

            cur_cmd.mem = 1;
            cur_cmd.reg = 1;
            cur_cmd.imm = 0;

            StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
            StackUniversalPush(&stk, &com_arr[0].command_code, COMMAND_SIZE     );
            StackUniversalPush(&stk, &reg_name               , REG_SIZE         );

            reg_name = 0;
            ok       = 0;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s [%lg]%n", command, &arg, &ok);

        if (ok) {

            cur_cmd.mem = 1;
            cur_cmd.reg = 0;
            cur_cmd.imm = 1;

            StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
            StackUniversalPush(&stk, &com_arr[0].command_code, COMMAND_SIZE     );
            StackUniversalPush(&stk, &arg                    , VARIABLE_SIZE    );

            arg = NAN;
            ok  = 0  ;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %cx + %lg%n", command, &reg_name, &arg, &ok);

        if (ok) {

            cur_cmd.mem = 0;
            cur_cmd.reg = 1;
            cur_cmd.imm = 1;

            StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
            StackUniversalPush(&stk, &com_arr[0].command_code, COMMAND_SIZE     );
            StackUniversalPush(&stk, &reg_name               , REG_SIZE         );
            StackUniversalPush(&stk, &arg                    , VARIABLE_SIZE    );

            reg_name = 0  ;
            arg      = NAN;
            ok       = 0  ;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %cx%n", command, &reg_name, &ok);

        if (ok && isalpha(reg_name)) {

            cur_cmd.mem = 0;
            cur_cmd.reg = 1;
            cur_cmd.imm = 0;

            for (int j = 0; j < NUMBER_OF_COMMANDS; j++)
                if (!strcmp(com_arr[j].command_name, command)) {

                    StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
                    StackUniversalPush(&stk, &com_arr[j].command_code, COMMAND_SIZE     );
                    StackUniversalPush(&stk, &reg_name               , REG_SIZE         );
                    break;
            }

            reg_name = 0;
            ok       = 0;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %lg%n", command, &arg, &ok);

        if (ok) {

            cur_cmd.mem = 0;
            cur_cmd.reg = 0;
            cur_cmd.imm = 1;

            StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
            StackUniversalPush(&stk, &com_arr[0].command_code, COMMAND_SIZE     );
            StackUniversalPush(&stk, &arg                    , VARIABLE_SIZE    );

            arg = NAN;
            ok  = 0  ;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %s%n", command, lab, &ok);

        if (ok) {

            int lab_stat = 0;

            cur_cmd.mem = 0;
            cur_cmd.reg = 0;
            cur_cmd.imm = 0;

            for (int j = 0; j < NUMBER_OF_COMMANDS; j++)
                if (!strcmp(com_arr[j].command_name, command)) {

                    if (labels->labels_count) {

                        for (LABEL* l = labels->label_arr;  l < labels->label_arr + labels->labels_count; l++)
                            if (!strcmp(l->label_name, lab)) {

                                lab_place = l->label_place;

                                StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
                                StackUniversalPush(&stk, &com_arr[j].command_code, COMMAND_SIZE     );
                                StackUniversalPush(&stk, &lab_place              , LABEL_SIZE       );

                                lab_stat = 1;

                                break;
                            }
                    }

                    if (!lab_stat) {

                        StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
                        StackUniversalPush(&stk, &com_arr[j].command_code, COMMAND_SIZE     );

                        break;
                    }
                }

            ok       = 0;
            lab_stat = 0;
            for (int i = 0; i < 20; i++) lab[i] = 0;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s%n", command, &ok);

        if (ok) {

            cur_cmd.mem = 0;
            cur_cmd.reg = 0;
            cur_cmd.imm = 0;

            for (int j = 0; j < NUMBER_OF_COMMANDS; j++)
                if (!strcmp(com_arr[j].command_name, command)) {

                    StackUniversalPush(&stk, &cur_cmd                , COMMAND_INFO_SIZE);
                    StackUniversalPush(&stk, &com_arr[j].command_code, COMMAND_SIZE     );
                    break;
                }

            ok = 0;

            str = tmp + 1;
            continue;
        }

        str = tmp + 1;
    }

    int err = 0;

    if ((err = FileWrite(stk.data, stk.sz, asm_file)))
        return err;

    free(code_file->text);

    StackDtor(&stk);

    return 0;
}

int LabelsListCtor(FILE_READ* code_file, LABELS* labels) {

    assert(code_file);
    assert(code_file->text);
    assert(labels);

    char* str = code_file->text;
    char* tmp = NULL;
    size_t labels_count = 0;

    while ((tmp = strchr(str, ':'))) {

        labels_count++;
        str = tmp + 1;
    }

    labels->labels_count = labels_count;

    if (!labels_count)
        return NO_LABELS;

    if (!(labels->label_arr = (LABEL*)calloc(labels_count, sizeof(LABEL))))
        return BUF_MEM_ERR;

    str = code_file->text;

    size_t cur_place = 0;
    int    i         = 0;

    if (strchr(str, '\r')) {

        while((tmp = strchr(str, ':'))) {

            tmp++;
            str = strchr(tmp, '\n');

            if (!((labels->label_arr)->label_name = (char*)calloc(str - tmp, sizeof(char))))
                return BUF_MEM_ERR;

            strncpy((labels->label_arr)->label_name, tmp, str - tmp - 1);
            *((labels->label_arr)->label_name + (str - tmp)) = '\0';

            (labels->label_arr)++;
            str = tmp;
        }

        (labels->label_arr) -= labels->labels_count;

        str = code_file->text;

        while((tmp = strchr(str, ':'))) {

            cur_place = CurrentPlace(tmp, labels, code_file);

            (labels->label_arr + i)->label_place = cur_place;

            i++;
            str = tmp + 1;
        }
    } else {

        while((tmp = strchr(str, ':'))) {

            tmp++;
            str = strchr(tmp, '\n');

            if (!((labels->label_arr)->label_name = (char*)calloc(str - tmp, sizeof(char))))
                return BUF_MEM_ERR;

            strncpy((labels->label_arr)->label_name, tmp, str - tmp);
            *((labels->label_arr)->label_name + (str - tmp)) = '\0';

            (labels->label_arr)++;
            str = tmp;
        }

        (labels->label_arr) -= labels->labels_count;

        str = code_file->text;

        while((tmp = strchr(str, ':'))) {

            cur_place = CurrentPlace(tmp, labels, code_file);

            (labels->label_arr)->label_place = cur_place;

            (labels->label_arr)++;
            str = tmp + 1;
        }

        (labels->label_arr) -= labels->labels_count;

        str = code_file->text;

        while((tmp = strchr(str, ':'))) {

            cur_place = CurrentPlace(tmp, labels, code_file);

            (labels->label_arr + i)->label_place = cur_place;

            i++;
            str = tmp + 1;
        }
    }

    return 0;
}

size_t CurrentPlace(char* label_place, LABELS* labels, FILE_READ* code_file) {

    assert(label_place);
    assert(labels);
    assert(code_file);

    size_t cur_place = 0;

    char* str = code_file->text;
    char* tmp = NULL           ;

    char   command[20] = {} ;
    char   lab[20]     = {} ;
    double arg         = NAN;
    int lab_stat = 0;

    size_t ok        = 0;
    char   reg_name  = 0;

    while((str < label_place) && (tmp = strchr(str, '\n'))) {

        if (*str == ':') {

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s [%cx + %lg]%n", command, &reg_name, &arg, &ok);

        if (ok) {

            reg_name = 0  ;
            arg      = NAN;
            ok       = 0  ;

            cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE + REG_SIZE + VARIABLE_SIZE;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s [%cx]%n", command, &reg_name, &ok);

        if (ok && isalpha(reg_name)) {

            reg_name = 0;
            ok       = 0;

            cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE + REG_SIZE;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s [%lg]%n", command, &arg, &ok);

        if (ok) {

            arg = NAN;
            ok  = 0  ;

            cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE + VARIABLE_SIZE;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %cx + %lg%n", command, &reg_name, &arg, &ok);

        if (ok) {

            reg_name = 0  ;
            arg      = NAN;
            ok       = 0  ;

            cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE + REG_SIZE + VARIABLE_SIZE;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %cx%n", command, &reg_name, &ok);

        if (ok && isalpha(reg_name)) {

            reg_name = 0;
            ok       = 0;

            cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE + REG_SIZE;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %lg%n", command, &arg, &ok);

        if (ok) {

            arg = NAN;
            ok  = 0  ;

            cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE + VARIABLE_SIZE;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s %s%n", command, lab, &ok);

        if (ok) {

            if (lab[0] != ':') {

                for (LABEL* l = labels->label_arr;  l < labels->label_arr + labels->labels_count; l++)
                    if (!strcmp(l->label_name, lab)) {

                        lab_stat = 1;

                        break;
                    }
            }

            if (lab_stat) {

                cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE + LABEL_SIZE;

            } else {

                cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE;

            }


            ok       = 0;
            lab_stat = 0;
            for (int i = 0; i < 20; i++) lab[i] = 0;

            str = tmp + 1;
            continue;
        }

        sscanf(str, "%s%n", command, &ok);

        if (ok) {

            cur_place += COMMAND_INFO_SIZE + COMMAND_SIZE;

            ok = 0;

            str = tmp + 1;
            continue;
        }

        str = tmp + 1;
    }

    return cur_place;
}

int LabelsListDtor(LABELS* labels) {

    assert(labels);

    for (LABEL* l = labels->label_arr;  l < labels->label_arr + labels->labels_count; l++) {

        free(l->label_name)  ;
        l->label_name  = NULL;
        l->label_place = 0   ;
    }

    labels->labels_count = 0;

    return 0;
}

