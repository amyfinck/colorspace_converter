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

void read_pixels_rgb(header_t *header, RGB_image_t *img, FILE *file)
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            fread(&img->pixels[index].B, 1, 1, file);
            fread(&img->pixels[index].G, 1, 1, file);
            fread(&img->pixels[index].R, 1, 1, file);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

void write_pixels_rgb(header_t *header, RGB_image_t *img, FILE *file)
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            fwrite(&img->pixels[index].B, 1, 1, file);
            fwrite(&img->pixels[index].G, 1, 1, file);
            fwrite(&img->pixels[index].R, 1, 1, file);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

void write_pixels_luma(header_t *header, YCC_image_t *img, FILE *file)
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            fwrite(&img->pixels[index].Y, 1, 1, file);
            fwrite(&img->pixels[index].Y, 1, 1, file);
            fwrite(&img->pixels[index].Y, 1, 1, file);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

void write_pixels_cb(header_t *header, YCC_image_t *img, FILE *file)
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    uint8_t base_color = 128;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            fwrite(&img->pixels[index].Cb, 1, 1, file);
            fwrite(&base_color, 1, 1, file);
            fwrite(&base_color, 1, 1, file);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

void write_pixels_cr(header_t *header, YCC_image_t *img, FILE *file)
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    uint8_t base_color = 128;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            fwrite(&base_color, 1, 1, file);
            fwrite(&base_color, 1, 1, file);
            fwrite(&img->pixels[index].Cr, 1, 1, file);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

void ycc_pixels_file_handler(header_t *header, YCC_image_t *img, FILE *file, void (*io_func)(YCC_image_t *, FILE *, uint32_t))
{
    exit_on_error(fseek(file, header->offset, SEEK_SET) != 0, "Error: Seeking pixel start position failed");
    uint32_t buffer_row_bytes = get_buffer_row_bytes(header->width);
    uint32_t row, column;
    for (row = 0; row < header->height; row++)
    {
        for (column = 0; column < header->width; column++)
        {
            uint32_t index = row * header->width + column;
            io_func(img, file, index);
            if (column == header->width - 1 && buffer_row_bytes != 0)
                fseek(file, buffer_row_bytes, SEEK_CUR);
            ++header->pixel_count;
        }
    }
}

#endif