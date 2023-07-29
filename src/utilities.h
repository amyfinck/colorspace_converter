#ifndef UTILITIES_H
#define UTILITIES_H

#include "constants.h"

#define PIXEL_M 3

uint32_t get_row_byte_count(uint32_t img_width)
{
    uint32_t row_bytes = img_width * PIXEL_M;
    if (row_bytes % 4 != 0)
    {
        uint32_t padding = 4 - (row_bytes % 4);
        row_bytes += padding;
    }
    return row_bytes;
}

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
    exit_on_error(condition, "width and height must be divisible by 2");
}

#endif