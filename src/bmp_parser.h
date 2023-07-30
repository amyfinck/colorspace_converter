#ifndef BMP_PARSER_H
#define BMP_PARSER_H

#include <stdint.h>
#include <stdio.h>
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

uint32_t get_buffer_row_bytes(uint32_t img_width)
{
    uint32_t row_byte_count = get_row_byte_count(img_width);
    uint32_t row_pixels_byte_count = img_width * PIXEL_M;
    return row_byte_count - row_pixels_byte_count;
}

void read_rgb(RGB_pixel_t *in_pixels, FILE *in_file, uint32_t index)
{
    printf("hello\n");
    fread(&in_pixels[index].B, 1, 1, in_file);
    fread(&in_pixels[index].G, 1, 1, in_file);
    fread(&in_pixels[index].R, 1, 1, in_file);
}

void write_rgb(RGB_pixel_t *in_pixels, FILE *out_file, uint32_t index)
{
    fwrite(&in_pixels[index].B, 1, 1, out_file);
    fwrite(&in_pixels[index].G, 1, 1, out_file);
    fwrite(&in_pixels[index].R, 1, 1, out_file);
}

void write_luma(YCC_pixel_t *in_pixels, FILE *out_file, uint32_t index)
{
    fwrite(&in_pixels[index].Y, 1, 1, out_file);
    fwrite(&in_pixels[index].Y, 1, 1, out_file);
    fwrite(&in_pixels[index].Y, 1, 1, out_file);
}

void write_cb(YCC_pixel_t *in_pixels, FILE *out_file, uint32_t index)
{
    uint8_t base_color = 128;
    fwrite(&in_pixels[index].Cb, 1, 1, out_file);
    fwrite(&base_color, 1, 1, out_file);
    fwrite(&base_color, 1, 1, out_file);
}

void write_cr(YCC_pixel_t *in_pixels, FILE *out_file, uint32_t index)
{
    uint8_t base_color = 128;
    fwrite(&base_color, 1, 1, out_file);
    fwrite(&base_color, 1, 1, out_file);
    fwrite(&in_pixels[index].Cr, 1, 1, out_file);
}

void rgb_pixels_file_handler(RGB_pixel_t *pixels, FILE *file, header_t * header, void (*io_func)(RGB_pixel_t *, FILE*, uint32_t))
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            io_func(pixels, file, index);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

void ycc_pixels_file_handler(YCC_pixel_t *pixels, header_t *header, FILE *file, void (*io_func)( YCC_pixel_t*, FILE *, uint32_t))
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            io_func(pixels, file, index);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

#endif