#ifndef FILE_READ_H_INCLUDED
#define FILE_READ_H_INCLUDED

#include <stddef.h>
#include <stdio.h>

typedef struct {

    size_t file_sz;
    char* text;
    char* file_name;
    FILE* file;
} FILE_READ;

typedef enum {

    OPEN_ERR = 10,
    READ_ERR = 11,
    END_ERR = 12,
    BUF_MEM_ERR = 13,
    CLOSE_ERR = 14,
    WRITE_ERR = 15
} FILE_READ_ERRORS_LIST;

int FileRead(FILE_READ* rd_file);
int FileSize(FILE_READ* rd_file);
int FileFree(FILE_READ* rd_file);
int FileWrite(const void* data, size_t data_size, FILE_READ* wr_file);

#endif // FILE_READ_H_INCLUDED
