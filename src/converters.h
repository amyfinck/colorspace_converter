#ifndef _CONVERTERS_H
#define _CONVERTERS_H

#include "constants.h"
#include "transformers.h"

void get_luma(uint32_t pixel_count, RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img);
void get_chroma(uint32_t pixel_count, RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img);
void ycc_to_rgb(uint32_t pixel_count, RGB_image_t *output_rgb_img, YCC_image_t *output_ycc_img);
void downsample_chroma(uint32_t height, uint32_t width, YCC_image_t *output_ycc_img);

#endif