#ifndef _CONVERTERS_H
#define _CONVERTERS_H

#include "constants.h"
#include "transformers.h"

void ycc_to_rgb(uint32_t pixel_count, RGB_pixel_t *output_rgb_img, YCC_pixel_t *output_ycc_img);
void rgb_to_ycc(header_t * header, YCC_pixel_t *output_ycc_img, FILE* in_fp);
void downsample_chroma(uint32_t height, uint32_t width, YCC_pixel_t *output_ycc_img);

#endif