#ifndef COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
#define COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H

//#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include "constants.h"
#include "transformers.h"
#include "converters.h"
#include "utilities.h"

void get_image_info(FILE* file, header_t *header);
//void write_rgb_file(FILE* rgb_file, RGB_pixel_t* output_rgb_pixels, header_t* header);
void write_header(FILE* file_to_write, FILE* reference_file, uint32_t header_length);
void resize_file(FILE* file, uint32_t width, uint32_t height);
void write_ycc_components(FILE* luma_fp, FILE* cb_fp, FILE* cr_fp, header_t* header, YCC_pixel_t* output_ycc_pixels);

#endif //COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
