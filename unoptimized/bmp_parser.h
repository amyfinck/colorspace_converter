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

void get_file_offset(RGB_image_t *in_img, FILE *in_file)
{
    exit_on_error(fseek(in_file, 10, SEEK_SET) != 0, "Seeking offset position failed");
    // exit_on_error(fread(&in_img->offset, 4, 1, in_file) == 0, "Reading file offset failed");
    fread(&in_img->offset, 4, 1, in_file);
}

void get_file_width(RGB_image_t *in_img, FILE *in_file)
{
    exit_on_error(fseek(in_file, 18, SEEK_SET) != 0, "Seeking width position failed");
    // exit_on_error(fread(&in_img->width, 4, 1, in_file) == 0, "Reading file width failed");
    fread(&in_img->offset, 4, 1, in_file);
}

void get_file_height(RGB_image_t *in_img, FILE *in_file)
{
    exit_on_error(fseek(in_file, 22, SEEK_SET) != 0, "Seeking height position failed");
    // exit_on_error(fread(&in_img->height, 4, 1, in_file) == 0, "Reading file height failed");
    fread(&in_img->offset, 4, 1, in_file);
}

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

void set_img_header_from_file(RGB_image_t *in_img, FILE *in_file)
{
    get_file_offset(in_img, in_file);
    get_file_width(in_img, in_file);
    get_file_height(in_img, in_file);
}

void read_rgb(RGB_image_t *in_img, FILE *in_file, uint32_t index)
{
    fread(&in_img->pixels[index].B, 1, 1, in_file);
    fread(&in_img->pixels[index].G, 1, 1, in_file);
    fread(&in_img->pixels[index].R, 1, 1, in_file);
}

void write_rgb(RGB_image_t *in_img, FILE *out_file, uint32_t index)
{
    fwrite(&in_img->pixels[index].B, 1, 1, out_file);
    fwrite(&in_img->pixels[index].G, 1, 1, out_file);
    fwrite(&in_img->pixels[index].R, 1, 1, out_file);
}

void write_luma(YCC_image_t *in_img, FILE *out_file, uint32_t index)
{
    fwrite(&in_img->pixels[index].Y, 1, 1, out_file);
    fwrite(&in_img->pixels[index].Y, 1, 1, out_file);
    fwrite(&in_img->pixels[index].Y, 1, 1, out_file);
}

void write_cb(YCC_image_t *in_img, FILE *out_file, uint32_t index)
{
    uint8_t base_color = 128;
    fwrite(&in_img->pixels[index].Cb, 1, 1, out_file);
    fwrite(&base_color, 1, 1, out_file);
    fwrite(&base_color, 1, 1, out_file);
}

void write_cr(YCC_image_t *in_img, FILE *out_file, uint32_t index)
{
    uint8_t base_color = 128;
    fwrite(&base_color, 1, 1, out_file);
    fwrite(&base_color, 1, 1, out_file);
    fwrite(&in_img->pixels[index].Cr, 1, 1, out_file);
}

void rgb_pixels_file_handler(RGB_image_t *img, FILE *file, void (*io_func)(RGB_image_t *, FILE *, uint32_t))
{
    exit_on_error(fseek(file, img->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(img->width);
    uint32_t row, column;
    for (row = 0; row < img->height; row++)
    {
        for (column = 0; column < img->width; column++)
        {
            uint32_t index = row * img->width + column;
            io_func(img, file, index);
            if (column == img->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++img->pixel_count;
        }
    }
}

void ycc_pixels_file_handler(YCC_image_t *img, FILE *file, void (*io_func)(YCC_image_t *, FILE *, uint32_t))
{
    exit_on_error(fseek(file, img->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(img->width);
    uint32_t row, column;
    for (row = 0; row < img->height; row++)
    {
        for (column = 0; column < img->width; column++)
        {
            uint32_t index = row * img->width + column;
            io_func(img, file, index);
            if (column == img->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++img->pixel_count;
        }
    }
}

// void file_to_rgb();
// void rgb_to_file();
// void file_to_ycc();
// void ycc_to_file();

#endif