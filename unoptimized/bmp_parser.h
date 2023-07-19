#ifndef BMP_PARSER_H
#define BMP_PARSER_H

#include <stdint.h>
#include <stdio.h>
#include "constants.h"

#define PIXEL_M 3

uint32_t get_file_size(FILE *in_file)
{
    fseek(in_file, 0, SEEK_END);
    return ftell(in_file);
}

uint32_t *get_file_offset(FILE *in_file)
{
    uint32_t *buffer;
    exit_on_error(fseek(in_file, 10, SEEK_SET) != 0, "Error: Seeking offset position failed");
    exit_on_error(fread(buffer, 4, 1, in_file) == 0, "Error: Reading file offset failed");
    return buffer;
}

uint32_t *get_file_width(FILE *in_file) 
{
    uint32_t *buffer;
    exit_on_error(fseek(in_file, 18, SEEK_SET) != 0, "Error: Seeking width position failed");
    exit_on_error(fread(buffer, 4, 1, in_file) == 0, "Error: Reading file width failed");
    return buffer;
}

uint32_t* get_file_height(FILE *in_file)
{
    uint32_t *buffer;
    exit_on_error(fseek(in_file, 22, SEEK_SET) != 0, "Error: Seeking height position failed");
    exit_on_error(fread(buffer, 4, 1, in_file) == 0, "Error: Reading file height failed");
    return buffer;
}

uint32_t get_row_byte_count(RGB_image_t *in_img)
{
    uint32_t row_bytes = in_img->width * PIXEL_M;
    if (row_bytes % 4 != 0)
    {
        uint32_t padding = 4 - (row_bytes % 4);
        row_bytes += padding;
    }
    return row_bytes;
}

void set_img_header_from_file(RGB_image_t *in_img, FILE *in_file)
{
    in_img->offset = *get_file_offset(in_file);
    in_img->width = *get_file_width(in_file);
    in_img->height = *get_file_height(in_file);
}

void set_img_pixels_from_file(RGB_image_t *in_img, FILE *in_file)
{
    exit_on_error(fseek(in_file, in_img->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t row_byte_count = get_row_byte_count(in_img);
    uint32_t row_pixels_byte_count = in_img->width * PIXEL_M;
    uint32_t buffer_row_bytes = row_byte_count - row_pixels_byte_count;
    uint32_t row, column;
    for (row = 0; row < in_img->height; row++) 
    {
        for (column = 0; column < in_img->width; column++)
        {
            uint32_t index = row * in_img->width + column;
            fread(&in_img->pixels[index].B, 1, 1, in_file);
            fread(&in_img->pixels[index].G, 1, 1, in_file);
            fread(&in_img->pixels[index].R, 1, 1, in_file);
            if (column == in_img->width - 1 && buffer_row_bytes != 0)
                fseek(in_file, buffer_row_bytes, SEEK_CUR);

            ++in_img->pixel_count;
        }
    }
}

void file_to_rgb();
void rgb_to_file();
void file_to_ycc();
void ycc_to_file();

#endif