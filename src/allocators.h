#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include "constants.h"

void allocate_header_memory(header_t **header)
{
    *header = (header_t *)malloc(sizeof(header_t));
    exit_on_error(*header == NULL, "Malloc for yyc image failed");
}

void allocate_rgb_memory(RGB_image_t **rgb_img)
{
    *rgb_img = (RGB_image_t *)malloc(sizeof(RGB_image_t));
    exit_on_error(*rgb_img == NULL, "Malloc for rgb image failed");
}

void allocate_ycc_memory(YCC_image_t **ycc_img)
{
    *ycc_img = (YCC_image_t *)malloc(sizeof(YCC_image_t));
    exit_on_error(*ycc_img == NULL, "Malloc for yyc image failed");
}

void allocate_rgb_pixels_memory(uint32_t height, uint32_t width, RGB_image_t *rgb_img)
{
    rgb_img->pixels = (RGB_pixel_t *)malloc(sizeof(RGB_pixel_t) * height * width);
    exit_on_error(rgb_img->pixels == NULL, "Malloc for RGB pixels failed");
}

void allocate_ycc_pixels_memory(uint32_t height, uint32_t width, YCC_image_t *ycc_img)
{
    ycc_img->pixels = (YCC_pixel_t *)malloc(sizeof(YCC_pixel_t) * height * width);
    exit_on_error(ycc_img->pixels == NULL, "Malloc for YCC pixels failed");
}

#endif //ALLOCATORS_H