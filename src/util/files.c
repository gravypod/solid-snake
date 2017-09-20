//
// Created by gravypod on 9/20/17.
//

#include "files.h"

#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

long int fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

char* read_file(const char* filename)
{
    const size_t file_size = (size_t) fsize(filename);
    FILE *f;

    if (file_size == -1 || !(f = fopen(filename, "rb"))) {
        return NULL;
    }

    size_t data_left = file_size;
    char *buffer = malloc(file_size + 1);
    char *tmp    = buffer;

    while (data_left > 0)
    {
        const size_t len = fread((void *) tmp, sizeof(char), sizeof(buffer), f);
        data_left -= len;
        tmp       += len;
    }
    buffer[file_size] = 0;

    fclose(f);

    return buffer;
}