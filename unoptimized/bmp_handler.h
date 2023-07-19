#ifndef BMP_HANDLER_H
#define BMP_HANDLER_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"

#define PATH_MAX 256
#define CHUNK_SIZE 4096
#define BMP_HEADER_SIZE 54

char *get_abs_path(char *rel_path)
{
    char abs_path[PATH_MAX];
    exit_on_error(abs_path == NULL, "Unable to allocate memory for path");
    exit_on_error(realpath(rel_path, abs_path) == NULL, "Cannot find absolute path");
    return abs_path;
}

void copy_file_header(FILE *src_file, FILE *dest_file)
{
    unsigned char header[BMP_HEADER_SIZE];
    fread(header, sizeof(unsigned char), BMP_HEADER_SIZE, src_file);
    fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, dest_file);
}

void copy_file_contents()
{
}

void copy_file(FILE *src_file, FILE *dest_file)
{
    size_t bytes_read;
    unsigned char buffer[CHUNK_SIZE];
    while ((bytes_read = fread(buffer, sizeof(unsigned char), CHUNK_SIZE, src_file)) > 0)
    {
        size_t bytes_write = fwrite(buffer, sizeof(unsigned char), bytes_read, dest_file);
        exit_on_error(bytes_write != bytes_read, "Error: Unable to write to file");
    }
}

#endif