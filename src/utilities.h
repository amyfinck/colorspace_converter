#ifndef UTILITIES_H
#define UTILITIES_H

#include "constants.h"

#define PIXEL_M 3

void resize_file(FILE* file, uint32_t width, uint32_t height);
void check_height_width(uint32_t width, uint32_t height);
void write_header(uint32_t offset, FILE* file_to_write, FILE* reference_file);

#endif