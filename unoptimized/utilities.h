#ifndef UTILITIES_H
#define UTILITIES_H

#include "constants.h"

void allocate_rgb_memory(RGB_image_t *rgb_img)
{
    rgb_img = (RGB_image_t *)malloc(sizeof(RGB_image_t));
    exit_on_error(rgb_img == NULL, "Malloc for rgb image failed");
}

void allocate_ycc_memory(YCC_image_t *ycc_img)
{
    ycc_img = (YCC_image_t *)malloc(sizeof(YCC_image_t));
    exit_on_error(ycc_img == NULL, "Malloc for rgb image failed");
}

void allocate_rgb_pixels_memory(RGB_image_t *rgb_img)
{
    rgb_img->pixel_count = 0;
    rgb_img->pixels = (RGB_pixel_t *)malloc(sizeof(RGB_pixel_t) * rgb_img->height * rgb_img->width);
    exit_on_error(rgb_img->pixels == NULL, "Malloc for RGB pixels failed");
}

void allocate_ycc_pixels_memory(YCC_image_t *ycc_img)
{
    ycc_img->pixel_count = 0;
    ycc_img->pixels = (YCC_pixel_t *)malloc(sizeof(YCC_pixel_t) * ycc_img->height * ycc_img->width);
    exit_on_error(ycc_img->pixels == NULL, "Malloc for YCC pixels failed");
}

void copy_rgb_to_yyc(RGB_image_t *rgb_in_img, YCC_image_t *ycc_out_img)
{
    ycc_out_img->offset = rgb_in_img->offset;
    ycc_out_img->width = rgb_in_img->width;
    ycc_out_img->height = rgb_in_img->height;
    allocate_ycc_pixels_memory(ycc_out_img);
}

void copy_rgb_to_rgb(RGB_image_t *rgb_in_img, RGB_image_t *rgb_out_img)
{
    rgb_out_img->offset = rgb_in_img->offset;
    rgb_out_img->width = rgb_in_img->width;
    rgb_out_img->height = rgb_in_img->height;
    allocate_rgb_pixels_memory(rgb_out_img);
}

#endif