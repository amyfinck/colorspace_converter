#ifndef BMP_PARSER_H
#define BMP_PARSER_H

#include <stdint.h>
#include <stdio.h>
#include "constants.h"

#define PIXEL_M 3

uint32_t get_row_byte_count(uint32_t img_width);
uint32_t get_buffer_row_bytes(uint32_t img_width);
void get_file_size(header_t *header, FILE *in_file);
void get_file_offset(header_t *header, FILE *file);
void get_file_width(header_t *header, FILE *file);
void get_file_height(header_t *header, FILE *file);
void get_image_info(header_t *header, FILE* file);
void read_pixels_rgb(header_t *header, RGB_pixel_t *img, FILE *file);
void write_pixels_rgb(header_t *header, RGB_pixel_t *img, FILE *file);
void write_pixels_luma(header_t *header, YCC_pixel_t *img, FILE *file);
void write_pixels_cb(header_t *header, YCC_pixel_t *img, FILE *file);
void write_pixels_cr(header_t *header, YCC_pixel_t *img, FILE *file);

#endif