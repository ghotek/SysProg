#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

const int BUF_SIZE        = 1000;

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

int FileRead(char* arr, const char* file_name);
unsigned int FileSize(FILE* f);
void HelpPrint(char* arr, unsigned int sz);

typedef struct srt {
    char* s_sort;
    char* back_sort;
} Sorting;

int main() {

    const char* file_name = "c_hamlet.txt";
    char* arr = NULL;
    //unsigned int* sz_f = 0;

    switch (FileRead(arr, file_name)) {

        case 1:
            printf("# File has been read\n");
            break;

        case END_ERR:
            printf("# Premature end of file\n");
            break;

        case RD_ERR:
            printf("# File read error\n");
            break;

        default:
            printf("# main(): unknown error");
            return 1;
            break;
    }

    //HelpPrint(arr, *sz_f);
    /*
    SetSort(arr, );

    */

    return 0;
}

int FileRead(char* arr, const char* file_name) {
    FILE* f_inp = NULL;
    unsigned int sz = 0;

    if ((f_inp = fopen(file_name, "r")))
        printf("# File %s is opened\n", file_name);
    else
        return RD_ERR;

    sz = FileSize(f_inp);

    printf("# Size of file :: {%d}\n", sz);

    if (!(arr = (char*)calloc(sizeof(char), sz)))
        return MEM_ERR;

    if (fread(arr, sizeof(char), sz, f_inp) != sz) {

        if (feof(f_inp)) return END_ERR;
        else return RD_ERR;
    }

    HelpPrint(arr, sz);
    return 1;
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

void HelpPrint(char* arr, unsigned int sz) {
    printf("# HelpPrint\n# ----------\n");

    for (char* i = arr; i < arr + sz; i++) {

        if (*i == '\0') printf("\n%c", '\0');
        else {
            if (*i == ' ') printf("%c", '+');
            else printf("%c", *i);
        }
    }
    printf("\n#\n# ----------\n");
}
