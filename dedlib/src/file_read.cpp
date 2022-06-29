#include <stdlib.h>
#include <assert.h>
#include "file_read.h"

int FileRead(FILE_READ* rd_file) {

    assert(rd_file);

    if (!(rd_file->file = fopen(rd_file->file_name, "rb")))
        return OPEN_ERR;

    if (FileSize(rd_file))
        return READ_ERR;

    if (!(rd_file->text = (char*)calloc(rd_file->file_sz + 1, sizeof(char))))
        return BUF_MEM_ERR;

    if (fread(rd_file->text, sizeof(char),
              rd_file->file_sz, rd_file->file) != rd_file->file_sz) {

        if (feof(rd_file->file))
            return END_ERR;
        else
            return READ_ERR;
    }

    if (fclose(rd_file->file) == EOF)
        return CLOSE_ERR;

    return 0;
}

int FileSize(FILE_READ* rd_file) {

    assert(rd_file);

    if (fseek(rd_file->file, 0, SEEK_END) == -1)
        return READ_ERR;

    rd_file->file_sz = ftell(rd_file->file);

    if (fseek(rd_file->file, 0, SEEK_SET) == -1)
        return READ_ERR;

    return 0;
}

int FileFree(FILE_READ* rd_file) {

    assert(rd_file);

    rd_file->file_sz = 0;

    free(rd_file->text);
    rd_file->text = NULL;

    rd_file->file_name = 0;

    rd_file->file = NULL;

    return 0;
}

int FileWrite(const void* data, size_t data_size, FILE_READ* wr_file) {

    assert(data);

    if (!(wr_file->file = fopen(wr_file->file_name, "wb")))
        return OPEN_ERR;

    if (!(fwrite((char*)data, sizeof(char), data_size, wr_file->file)))
        return WRITE_ERR;

    if (fclose(wr_file->file) == EOF)
        return CLOSE_ERR;

    return 0;
}
