#ifndef COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
#define COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H

#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include "constants.h"
#include "transformers.h"
#include "converters.h"
#include "utilities.h"

void get_image_info(FILE* file);
void read_pixels(FILE* file);
void write_rgb_file(FILE* rgb_file);
void write_header(FILE* file_to_write, FILE* reference_file);
void resize_file(FILE* file, uint32_t width, uint32_t height);
void write_ycc_components(FILE* luma_fp, FILE* cb_fp, FILE* cr_fp);


#endif //COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
