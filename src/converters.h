#include "constants.h"

/*
 * y = 16.0 + 0.257 R + 0.504 G + 0.098 B
 * 2^8 * 0.257 = 66
 * 2^8 * 0.504 = 129
 * 2^8 * 0.098 = 25
 */
void get_luma(uint32_t pixel_count, RGB_pixel_t *input_rgb_pixels, YCC_pixel_t *output_ycc_pixels)
{
    uint32_t i;
    for(i = 0; i < pixel_count; i++)
    {
        uint8_t R = input_rgb_pixels[i].R;
        uint8_t G = input_rgb_pixels[i].G;
        uint8_t B = input_rgb_pixels[i].B;
        output_ycc_pixels[i].Y = compute_ycc_y(R, G, B);
    }
}

void get_chroma(uint32_t pixel_count, RGB_pixel_t *input_rgb_pixels, YCC_pixel_t *output_ycc_pixels)
{
    uint32_t i;
    for(i = 0; i < pixel_count; i++)
    {
        uint8_t R = input_rgb_pixels[i].R;
        uint8_t G = input_rgb_pixels[i].G;
        uint8_t B = input_rgb_pixels[i].B;
        output_ycc_pixels[i].Cb = compute_ycc_cb(R, G, B);
        output_ycc_pixels[i].Cr = compute_ycc_cr(R, G, B);
    }
}

void ycc_to_rgb(uint32_t pixel_count, RGB_pixel_t *output_rgb_pixels, YCC_pixel_t *output_ycc_pixels)
{
    uint32_t i;
    for(i = 0; i < pixel_count; i++)
    {
        uint8_t Y = output_ycc_pixels[i].Y;
        uint8_t Cb = output_ycc_pixels[i].Cb;
        uint8_t Cr = output_ycc_pixels[i].Cr;

        output_rgb_pixels[i].R = compute_rgb_r(Y, Cb, Cr);
        output_rgb_pixels[i].G = compute_rgb_g(Y, Cb, Cr);
        output_rgb_pixels[i].B = compute_rgb_b(Y, Cb, Cr);
    }
}

void downsample_chroma(uint32_t height, uint32_t width, YCC_pixel_t *output_ycc_pixels)
{
    uint32_t y;
    for(y = 0; y < height; y+=2)
    {
        uint32_t x;
        for(x = 0; x < width; x+=2)
        {
            uint32_t bottom_left = y * width + x;
            uint32_t bottom_right = bottom_left + 1;
            uint32_t top_left = bottom_left + width;
            uint32_t top_right = top_left + 1;

            int32_t Cb_total = (int32_t)output_ycc_pixels[bottom_left].Cb + (int32_t)output_ycc_pixels[bottom_right].Cb + (int32_t)output_ycc_pixels[top_left].Cb + (int32_t)output_ycc_pixels[top_right].Cb;
            int8_t Cb_avg = (int8_t)(Cb_total / 4);
            int32_t Cr_total = (int32_t)output_ycc_pixels[bottom_left].Cr + (int32_t)output_ycc_pixels[bottom_right].Cr + (int32_t)output_ycc_pixels[top_left].Cr + (int32_t)output_ycc_pixels[top_right].Cr;
            int8_t Cr_avg = (int8_t)(Cr_total / 4);

            output_ycc_pixels[bottom_left].Cb = Cb_avg;
            output_ycc_pixels[bottom_right].Cb = Cb_avg;
            output_ycc_pixels[top_left].Cb = Cb_avg;
            output_ycc_pixels[top_right].Cb = Cb_avg;

            output_ycc_pixels[bottom_left].Cr = Cr_avg;
            output_ycc_pixels[bottom_right].Cr = Cr_avg;
            output_ycc_pixels[top_left].Cr = Cr_avg;
            output_ycc_pixels[top_right].Cr = Cr_avg;
        }
    }
}