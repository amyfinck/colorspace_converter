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

void get_image_info(header_t *header, FILE* file);
void resize_file(FILE* file, uint32_t width, uint32_t height);
void read_pixels(header_t *header, RGB_image_t *input_rgb_img, FILE* file);
void write_rgb_file(header_t *header, RGB_image_t *output_rgb_img, FILE* rgb_file);
void write_header(uint32_t offset, FILE* file_to_write, FILE* reference_file);
void write_ycc_components(header_t *header, YCC_image_t *output_ycc_img, FILE* luma_fp, FILE* cb_fp, FILE* cr_fp);


#endif //COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
