#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "prog_consts.h"
#include "transformers.h"

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

void compute_ycc_from_rgb(RGB_image_t *rgb_in_img, YCC_image_t *yyc_out_img)
{
    uint32_t index;
    for (index = 0; index < rgb_in_img->pixel_count; index++)
    {
        uint8_t R = rgb_in_img->pixels[index].R;
        uint8_t G = rgb_in_img->pixels[index].G;
        uint8_t B = rgb_in_img->pixels[index].B;

        yyc_out_img->pixels[index].Y = compute_ycc_y(R, G, B);
        yyc_out_img->pixels[index].Cb = compute_ycc_cb(R, G, B);
        yyc_out_img->pixels[index].Cr = compute_ycc_cr(R, G, B);
    }
}

void compute_rgb_from_ycc(YCC_image_t *yyc_out_img, RGB_image_t *rgb_in_img)
{
    uint32_t index;
    for (index = 0; index < rgb_in_img->pixel_count; index++)
    {
        uint8_t Y = yyc_out_img->pixels[index].Y;
        uint8_t Cb = yyc_out_img->pixels[index].Cb;
        uint8_t Cr = yyc_out_img->pixels[index].Cr;

        rgb_in_img->pixels[index].R = compute_rgb_r(Y, Cb, Cr);
        rgb_in_img->pixels[index].G = compute_rgb_g(Y, Cb, Cr);
        rgb_in_img->pixels[index].B = compute_rgb_b(Y, Cb, Cr);
    }
}

void downsample_ycc_chroma()
{

}


void rgb_to_ycc();
void ycc_to_ycc();

void downsample_ycc();

#endif