#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <assert.h>
#include "prog_consts.h"

void copy_rgb_to_yyc(RGB_image_t *rgb_in_img, YCC_image_t *yyc_out_img)
{
    yyc_out_img->pixel_count = 0;
    yyc_out_img->offset = rgb_in_img->offset;
    yyc_out_img->width = rgb_in_img->width;
    yyc_out_img->height = rgb_in_img->height;
    yyc_out_img->pixels = (YCC_pixel_t *)malloc(sizeof(YCC_pixel_t) * yyc_out_img->height * yyc_out_img->width);
    assert(yyc_out_img->pixels != NULL, "Error: Malloc for YYC pixels failed");
}

void copy_rgb_to_rgb(RGB_image_t *rgb_in_img, RGB_image_t *rgb_out_img)
{
    rgb_out_img->pixel_count = 0;
    rgb_out_img->offset = rgb_in_img->offset;
    rgb_out_img->width = rgb_in_img->width;
    rgb_out_img->height = rgb_in_img->height;
    rgb_out_img->pixels = (RGB_pixel_t *)malloc(sizeof(RGB_pixel_t) * rgb_out_img->height * rgb_out_img->width);
    assert(rgb_out_img->pixels != NULL, "Error: Malloc for RGB pixels failed");
}

#endif