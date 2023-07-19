#ifndef BMP_PARSER_H
#define BMP_PARSER_H

#include <stdint.h>
#include <stdio.h>
#include "constants.h"

#define CHUNK_SIZE 4096
#define BMP_HEADER_SIZE 54

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