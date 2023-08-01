#include "converters.h"

void rgb_to_ycc(uint32_t pixel_count, RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img)
{
    uint32_t i;
    for(i = pixel_count - 1; i != 0; i-=2)
    {
        uint8_t R = input_rgb_img->pixels[i].R;
        uint8_t G = input_rgb_img->pixels[i].G;
        uint8_t B = input_rgb_img->pixels[i].B;

        output_ycc_img->pixels[i].Y = compute_ycc_y(R, G, B);
        output_ycc_img->pixels[i].Cb = compute_ycc_cb(R, G, B);
        output_ycc_img->pixels[i].Cr = compute_ycc_cr(R, G, B);

        R = input_rgb_img->pixels[i - 1].R;
        G = input_rgb_img->pixels[i - 1].G;
        B = input_rgb_img->pixels[i - 1].B;

        output_ycc_img->pixels[i - 1].Y = compute_ycc_y(R, G, B);
        output_ycc_img->pixels[i - 1].Cb = compute_ycc_cb(R, G, B);
        output_ycc_img->pixels[i - 1].Cr = compute_ycc_cr(R, G, B);
    }
}


/*
 * R′ = 1.164(Y′−16) + 1.596(Cr−128)
 * G′ = 1.164(Y′−16) − 0.813(Cr-128) − 0.391(Cb−128)
 * B′ = 1.164(Y′−16) + 2.018(Cb−b128)
 *
 * 2^6 * 1.164 = 74
 * 2^6 * 1.596 = 102
 * 2^6 * 0.813 = 52
 * 2^6 * 0.391 = 25
 * 2^6 * 2.018 = 129
 */
void ycc_to_rgb(uint32_t pixel_count, RGB_image_t *output_rgb_img, YCC_image_t *output_ycc_img)
{
    uint32_t i;
    for(i = 0; i < pixel_count; i++)
    {
        uint8_t Y = output_ycc_img->pixels[i].Y;
        uint8_t Cb = output_ycc_img->pixels[i].Cb;
        uint8_t Cr = output_ycc_img->pixels[i].Cr;

        output_rgb_img->pixels[i].B = compute_rgb_b(Y, Cb, Cr);
        output_rgb_img->pixels[i].G = compute_rgb_g(Y, Cb, Cr);
        output_rgb_img->pixels[i].R = compute_rgb_r(Y, Cb, Cr);

        Y = output_ycc_img->pixels[i - 1].Y;
        Cb = output_ycc_img->pixels[i - 1].Cb;
        Cr = output_ycc_img->pixels[i - 1].Cr;

        output_rgb_img->pixels[i - 1].B = compute_rgb_b(Y, Cb, Cr);
        output_rgb_img->pixels[i - 1].G = compute_rgb_g(Y, Cb, Cr);
        output_rgb_img->pixels[i - 1].R = compute_rgb_r(Y, Cb, Cr);
    }
}

void downsample_chroma(uint32_t height, uint32_t width, YCC_image_t *output_ycc_img)
{
    // TODO this should probably be a scaled down image
    uint32_t y;
    for(y = 0; y < height; y+=2)
    {
        uint32_t x;
        for(x = 0; x < width; x+=4)
        {
            // Unroll 1
            uint32_t bottom_left = y * width + x;
            uint32_t bottom_right = bottom_left + 1;
            uint32_t top_left = bottom_left + width;
            uint32_t top_right = top_left + 1;

            int32_t Cb_total = (int32_t)output_ycc_img->pixels[bottom_left].Cb + 
            (int32_t)output_ycc_img->pixels[bottom_right].Cb + 
            (int32_t)output_ycc_img->pixels[top_left].Cb + 
            (int32_t)output_ycc_img->pixels[top_right].Cb;

            int8_t Cb_avg = (int8_t)(Cb_total / 4);

            int32_t Cr_total = (int32_t)output_ycc_img->pixels[bottom_left].Cr + 
            (int32_t)output_ycc_img->pixels[bottom_right].Cr + 
            (int32_t)output_ycc_img->pixels[top_left].Cr + 
            (int32_t)output_ycc_img->pixels[top_right].Cr;

            int8_t Cr_avg = (int8_t)(Cr_total / 4);

            output_ycc_img->pixels[bottom_left].Cb = Cb_avg;
            output_ycc_img->pixels[bottom_right].Cb = Cb_avg;
            output_ycc_img->pixels[top_left].Cb = Cb_avg;
            output_ycc_img->pixels[top_right].Cb = Cb_avg;

            output_ycc_img->pixels[bottom_left].Cr = Cr_avg;
            output_ycc_img->pixels[bottom_right].Cr = Cr_avg;
            output_ycc_img->pixels[top_left].Cr = Cr_avg;
            output_ycc_img->pixels[top_right].Cr = Cr_avg;

            // Unroll 2
            bottom_left = y * width + (x + 2);
            bottom_right = bottom_left + 1;
            top_left = bottom_left + width;
            top_right = top_left + 1;

            int32_t Cb_total = (int32_t)output_ycc_img->pixels[bottom_left].Cb + 
            (int32_t)output_ycc_img->pixels[bottom_right].Cb + 
            (int32_t)output_ycc_img->pixels[top_left].Cb + 
            (int32_t)output_ycc_img->pixels[top_right].Cb;

            Cb_avg = (int8_t)(Cb_total / 4);
            
            Cr_total = (int32_t)output_ycc_img->pixels[bottom_left].Cr + 
            (int32_t)output_ycc_img->pixels[bottom_right].Cr + 
            (int32_t)output_ycc_img->pixels[top_left].Cr + 
            (int32_t)output_ycc_img->pixels[top_right].Cr;

            Cr_avg = (int8_t)(Cr_total / 4);

            output_ycc_img->pixels[bottom_left].Cb = Cb_avg;
            output_ycc_img->pixels[bottom_right].Cb = Cb_avg;
            output_ycc_img->pixels[top_left].Cb = Cb_avg;
            output_ycc_img->pixels[top_right].Cb = Cb_avg;

            output_ycc_img->pixels[bottom_left].Cr = Cr_avg;
            output_ycc_img->pixels[bottom_right].Cr = Cr_avg;
            output_ycc_img->pixels[top_left].Cr = Cr_avg;
            output_ycc_img->pixels[top_right].Cr = Cr_avg;
        }
    }
}