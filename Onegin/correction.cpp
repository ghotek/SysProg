#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "correctionHeader.h"

/*!
    Corrected file path without name.
*/
const char* c_file_path   = "D:\\progs\\prj\\DedHw\\oneg\\";

/*!
    Uncorrect file name.
*/
const char* unc_file_name = "hamlet.txt";

/*!
    Corrected file name.
*/
const char* c_file_name   = "c_hamlet.txt";

/*!
    Error with end of file.
*/
const int   END_ERR       = 3;

/*!
    Error with file reading.
*/
const int   RD_ERR        = 4;

/*!
    Error with dynamic memory
*/
const int   MEM_ERR       = 5;

/*!
    Error with writing in stream.
*/
const int   WR_ERR        = 6;

int main() {

    FILE* unc_file = fopen(unc_file_name, "r");
    FILE* c_file = fopen(c_file_name, "w");
    unsigned int sz_f = 0;

    if (!unc_file && !c_file) printf("# Cannot open files\n");
    else printf("# Files is opened\n");

    if ((sz_f = FileSize(unc_file)) == RD_ERR)
        printf("# File read error\n");

    printf("# File size is :: {%d}\n", sz_f);

    switch (FileCorrection(unc_file, c_file, sz_f)) {

        case 1:
            printf("# Correct file was created\n");
            printf("# Path :: %s\%s", c_file_path, c_file_name);
            break;

        case END_ERR:
            printf("# Premature end of file\n");
            break;

        case RD_ERR:
            printf("# File read error\n");
            break;

        case WR_ERR:
            printf("# File write error\n");
            break;

        default:
            printf("# main(): unknown error");
            return 1;
            break;
    }

    fclose(unc_file);
    fclose(c_file);

    return 0;
}

unsigned int FileSize(FILE* f) {
    unsigned int sz = 0;

    if(fseek(f, 0, SEEK_END) != 0)
        return RD_ERR;

    sz = ftell(f);

    if(fseek(f, 0, SEEK_SET) != 0)
        return RD_ERR;

    return sz;
}

int FileCorrection(FILE* unc_f, FILE* c_f, unsigned int sz_f) {
    char *buf = NULL, *str = NULL, *tmp = NULL;
    int win_key = 0;

    if (!(buf = (char*)calloc(sizeof(char), sz_f)))
        return MEM_ERR;

    str = buf;

    if (fread(buf, sizeof(char), sz_f, unc_f) != sz_f) {

        if (feof(unc_f)) return END_ERR;
        else return RD_ERR;
    }

    if (strchr(buf, '\r')) win_key = 1;

    if (!win_key) {
        while((tmp = strchr(str, '\n'))) {//   {str}->__{str}->afkadnnf{tmp}->\n\0skldalf\n\0\n\n\0  -> __jksaf\0\0dnas\0\0
                                          //   __{str}->afkadnnf\0{tmp}->\0skldalf\n\0
            //printf("{%d} \t {%d} \t {%d} \t {%d}\n", *(tmp-1), *tmp, *(tmp + 1), *(tmp+2));
            //all text :: skbjbaf\n\n\n\nhksbafj\nsbafj\nkns...kdn\n\0
            //*tmp = '\0';
            while(*str == ' ' || *str == '\t' || *str == '\0') str++;
            //we have only one \0 in end of text

            unsigned int end_symbols = 0;
            while(*tmp == '\n') {//__fldnkan\n\n\ndnf   --->    {s}->fdlfdf\0\0{t}->\0fkff
                *tmp = '\0';
                tmp++;
                end_symbols++;
            }
            /*tmp -= end_symbols - 1;

            end_symbols = 0;

            while(!isalpha(*tmp)) {
                *tmp = '\0';
                tmp--;
                end_symbols++;
            }   */
            //tmp += end_symbols;
            //tmp--;
            /*
            unsigned int end_symbols = 0;
            while(*tmp == '\0' || *tmp == ' ' || *tmp == '\t') {
                    end_symbols++;
                    if (!(*tmp == '\0')) *tmp = '\0';
                    tmp--;
            }
            tmp++;
            while(*tmp == '\0') tmp++;*/

            unsigned int str_length = tmp - str;
            if (fwrite(str, sizeof(char), str_length, c_f) == str_length) {
                //printf("# tmp before :: {%d}\n", *tmp);
                //tmp = tmp + end_symbols;
                str = tmp;
                //end_symbols = 0;
                printf("# str :: {%d}\n", strchr(str, '\n'));
            } else return WR_ERR;
        }
    } else {
        while((tmp = strchr(str, '\r'))) {

            *tmp = '\0';
            *(tmp - 1) = '\0';
            while(*str == ' ' || *str == '\t') str++;

            unsigned int str_length = tmp - str;
            if (fwrite(str, sizeof(char), str_length, c_f) == str_length)
                str = tmp + 2;
            else return WR_ERR;
        }
    }
    printf("# Size of corrected file :: {%d}\n", FileSize(c_f));
    free(buf);
    return 1;
}

void DoubleSpaceDelete(char* buf) {
    char* tmp = NULL;
    while((tmp = strstr(buf, "  "))) {
        strcpy(tmp, tmp + 1);
    }
}
