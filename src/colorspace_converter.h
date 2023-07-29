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
#include "allocators.h"
#include "utilities.h"

void get_image_info(RGB_image_t *input_rgb_img, FILE* file);
void read_pixels(RGB_image_t *input_rgb_img, FILE* file);
void write_header(RGB_image_t *input_rgb_img, FILE* file_to_write, FILE* reference_file);
void resize_file(FILE* file, uint32_t width, uint32_t height);
void get_luma(RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img);
void get_chroma(RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img);
void downsample_chroma(RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img);
void ycc_to_rgb(RGB_image_t *input_rgb_img, RGB_image_t *output_rgb_img, YCC_image_t *output_ycc_img);
void write_rgb(RGB_image_t *input_rgb_img, RGB_image_t *output_rgb_img, FILE* rgb_file);
void write_ycc_components(RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img, FILE* luma_fp, FILE* cb_fp, FILE* cr_fp);


#endif //COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
