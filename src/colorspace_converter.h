#ifndef COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
#define COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H

#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include "constants.h"
#include "converters.h"
#include "allocators.h"
#include "utilities.h"
#include "bmp_parser.h"

void get_image_info(header_t *header, FILE* file);
void write_header(uint32_t offset, FILE* file_to_write, FILE* reference_file);


#endif //COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
