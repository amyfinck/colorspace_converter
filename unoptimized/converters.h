#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "constants.h"
#include "transformers.h"

void compute_ycc_from_rgb(RGB_image_t *rgb_in_img, YCC_image_t *ycc_out_img)
{
    uint32_t index;
    for (index = 0; index < rgb_in_img->pixel_count; index++)
    {
        uint8_t R = rgb_in_img->pixels[index].R;
        uint8_t G = rgb_in_img->pixels[index].G;
        uint8_t B = rgb_in_img->pixels[index].B;

        ycc_out_img->pixels[index].Y = compute_ycc_y(R, G, B);
        ycc_out_img->pixels[index].Cb = compute_ycc_cb(R, G, B);
        ycc_out_img->pixels[index].Cr = compute_ycc_cr(R, G, B);
    }
}

void compute_rgb_from_ycc(YCC_image_t *ycc_in_img, RGB_image_t *rgb_out_img)
{
    printf("BOOOOOAAAA");
    uint32_t index;
    for (index = 0; index < ycc_in_img->pixel_count; index++)
    {
        uint8_t Y = ycc_in_img->pixels[index].Y;
        uint8_t Cb = ycc_in_img->pixels[index].Cb;
        uint8_t Cr = ycc_in_img->pixels[index].Cr;

        printf("YOOOOO");

        rgb_out_img->pixels[index].R = compute_rgb_r(Y, Cb, Cr);
        rgb_out_img->pixels[index].G = compute_rgb_g(Y, Cb, Cr);
        rgb_out_img->pixels[index].B = compute_rgb_b(Y, Cb, Cr);
    }
}

void downsample_ycc_chroma(YCC_image_t *ycc_out_img)
{
    uint32_t row, column, i;
    uint8_t sample_size = 4;
    for (row = 0; row < ycc_out_img->height; row += (sample_size / 2))
    {
        for (column = 0; column < ycc_out_img->width; column += (sample_size / 2))
        {
            uint32_t index = row * ycc_out_img->width + column;
            uint32_t pixels[] = {
                index,
                index + 1,
                index + ycc_out_img->width,
                index + ycc_out_img->width + 1};

            int32_t cb_total = 0;
            int32_t cr_total = 0;

            for (i = 0; i < sample_size; ++i)
            {
                cb_total += ycc_out_img->pixels[pixels[i]].Cb;
                cr_total += ycc_out_img->pixels[pixels[i]].Cr;
            }

            int8_t cb_avg = (int8_t)(cb_total / sample_size);
            int8_t cr_avg = (int8_t)(cr_total / sample_size);

            for (i = 0; i < sample_size; ++i)
            {
                ycc_out_img->pixels[pixels[i]].Cb = cb_avg;
                ycc_out_img->pixels[pixels[i]].Cr = cr_avg;
            }
        }
    }
}

void rgb_to_ycc();
void ycc_to_ycc();

#endif