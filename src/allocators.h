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

void allocate_rgb_pixels_memory(uint32_t height, uint32_t width, RGB_pixel_t **pixels)
{
    *pixels = (RGB_pixel_t *)malloc(sizeof(RGB_pixel_t) * height * width);
    exit_on_error(*pixels == NULL, "Malloc for RGB pixels failed");
}

void allocate_ycc_pixels_memory(uint32_t height, uint32_t width, YCC_pixel_t **pixels)
{
    *pixels = (YCC_pixel_t *)malloc(sizeof(YCC_pixel_t) * height * width);
    exit_on_error(*pixels == NULL, "Malloc for YCC pixels failed");
}

// void copy_rgb_to_yyc(RGB_image_t *rgb_in_img, YCC_image_t *ycc_out_img)
// {
//     ycc_out_img->pixel_count = rgb_in_img->pixel_count;
//     ycc_out_img->offset = rgb_in_img->offset;
//     ycc_out_img->width = rgb_in_img->width;
//     ycc_out_img->height = rgb_in_img->height;
//     allocate_ycc_pixels_memory(ycc_out_img);
// }

// void copy_rgb_to_rgb(RGB_image_t *rgb_in_img, RGB_image_t *rgb_out_img)
// {
//     rgb_out_img->pixel_count = rgb_in_img->pixel_count;
//     rgb_out_img->offset = rgb_in_img->offset;
//     rgb_out_img->width = rgb_in_img->width;
//     rgb_out_img->height = rgb_in_img->height;
//     allocate_rgb_pixels_memory(rgb_out_img);
// }

#endif //ALLOCATORS_H