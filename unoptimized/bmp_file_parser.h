#ifndef BMP_FILE_PARSER_H
#define BMP_FILE_PARSER_H

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "prog_consts.h"

#define CHUNK_SIZE 4096
#define PIXEL_M 3
#define BMP_HEADER_SIZE 54

uint32_t get_file_size(FILE *in_file)
{
    fseek(in_file, 0, SEEK_END);
    return ftell(in_file);
}

uint32_t *get_file_offset(FILE *in_file)
{
    uint32_t *buffer;
    assert(fseek(in_file, 10, SEEK_SET) == 0, "Error: Seeking offset position failed");
    assert(fread(buffer, 4, 1, in_file) != 0);
    return buffer;
}

uint32_t *get_file_width(FILE *in_file) 
{
    uint32_t *buffer;
    assert(fseek(in_file, 18, SEEK_SET) == 0, "Error: Seeking width position failed");
    assert(fread(buffer, 4, 1, in_file) != 0);
    return buffer;
}

uint32_t* get_file_height(FILE *in_file)
{
    uint32_t *buffer;
    assert(fseek(in_file, 22, SEEK_SET) == 0, "Error: Seeking height position failed");
    assert(fread(buffer, 4, 1, in_file) != 0);
    return buffer;
}

void set_file_header(RGB_image_t *in_img, FILE *in_file)
{
    in_img->offset = *get_file_offset(in_file);
    in_img->width = *get_file_width(in_file);
    in_img->height = *get_file_height(in_file);
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


void set_pixels(RGB_image_t *in_img, FILE *in_file)
{
    assert(fseek(in_file, in_img->offset, SEEK_SET) == 0, "Error: Seeking pixel start position failed");
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

    // Read and write data in chunks until the end of the input file
    while ((bytes_read = fread(buffer, sizeof(unsigned char), CHUNK_SIZE, src_file)) > 0)
        assert(fwrite(buffer, sizeof(unsigned char), bytes_read, dest_file) != bytes_read, "Error: Unable to write to file");
}

void file_to_rgb();
void rgb_to_file();
void file_to_ycc();
void ycc_to_file();

#endif