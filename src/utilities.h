#ifndef UTILITIES_H
#define UTILITIES_H

#include "constants.h"

#define PIXEL_M 3

void resize_file(FILE* file, uint32_t width, uint32_t height)
{
    if (fseek(file, 18, SEEK_SET) != 0){
        printf("Error seeking to width position\n"); ; exit(1);
    }
    fwrite(&width, 4, 1, file);
    if (fseek(file, 22, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); ; exit(1);
    }
    fwrite(&height, 4, 1, file);
}

void check_height_width(uint32_t width, uint32_t height)
{
    int condition = width % 2 != 0 || height % 2 != 0;
    if(condition == 1)
    {
        printf("Error: width and height must be divisible by 2\n");
        exit(1);
    }
}

void write_header(uint32_t offset, FILE* file_to_write, FILE* reference_file)
{
    char buffer[offset];
    size_t bytes_read;
    size_t total_bytes_read = 0;
    fseek(reference_file, 0, SEEK_SET);
    while (total_bytes_read < offset && (bytes_read = fread(buffer, 1, offset - total_bytes_read, reference_file)) > 0)
    {
        if(fwrite(buffer, 1, bytes_read, file_to_write) != bytes_read)
        {
            printf("Error writing header\n");
            exit(1);
        }
        total_bytes_read += bytes_read;
    }
}

#endif