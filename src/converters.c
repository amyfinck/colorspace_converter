#include "converters.h"

void rgb_to_ycc(header_t* header, YCC_pixel_t *output_ycc_img, FILE* in_file)
{
    if(fseek(in_file, header->offset, SEEK_SET) != 0)
    {
        printf("Error: Seeking pixel start position failed\n");
        exit(1);
    }

    uint8_t R;
    uint8_t G;
    uint8_t B;

    uint32_t buffer_row_bytes = header->padding;
    uint32_t row, column;

    register uint32_t width = header->width;
    register uint32_t height = header->height;
    for (row = 0; row < height; row++)
    {
        for (column = 0; column < width; column+=2)
        {
            uint32_t index = row * header->width + column;
            uint32_t index2 = index + 1;

            fread(&B, 1, 1, in_file);
            fread(&G, 1, 1, in_file);
            fread(&R, 1, 1, in_file);

            output_ycc_img[index].Y = compute_ycc_y(R, G, B);
            output_ycc_img[index].Cb = compute_ycc_cb(R, G, B);
            output_ycc_img[index].Cr = compute_ycc_cr(R, G, B);

            fread(&B, 1, 1, in_file);
            fread(&G, 1, 1, in_file);
            fread(&R, 1, 1, in_file);

            output_ycc_img[index2].Y = compute_ycc_y(R, G, B);
            output_ycc_img[index2].Cb = compute_ycc_cb(R, G, B);
            output_ycc_img[index2].Cr = compute_ycc_cr(R, G, B);

        }
        if (buffer_row_bytes != 0)
        {
            fseek(in_file, buffer_row_bytes, SEEK_CUR);
        }
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
void ycc_to_rgb(uint32_t pixel_count, RGB_pixel_t *output_rgb_img, YCC_pixel_t *output_ycc_img)
{
    uint32_t i;
    for(i = pixel_count - 1; i != 0; --i)
    {
        uint8_t Y = output_ycc_img[i].Y;
        uint8_t Cb = output_ycc_img[i].Cb;
        uint8_t Cr = output_ycc_img[i].Cr;

        output_rgb_img[i].B = compute_rgb_b(Y, Cb, Cr);
        output_rgb_img[i].G = compute_rgb_g(Y, Cb, Cr);
        output_rgb_img[i].R = compute_rgb_r(Y, Cb, Cr);
    }
}

void downsample_chroma(uint32_t height, uint32_t width, YCC_pixel_t *output_ycc_img)
{
    uint32_t y, x;
    for(y = 0; y < height; y+=2)
    {
        for(x = 0; x < width; x+=2)
        {
            uint32_t bottom_left = y * width + x;
            uint32_t bottom_right = bottom_left + 1;
            uint32_t top_left = bottom_left + width;
            uint32_t top_right = top_left + 1;

            int32_t Cb_total = (int32_t)output_ycc_img[bottom_left].Cb + (int32_t)output_ycc_img[bottom_right].Cb + (int32_t)output_ycc_img[top_left].Cb + (int32_t)output_ycc_img[top_right].Cb;
            int8_t Cb_avg = (int8_t)((Cb_total >> 2) & 0xFF);
            int32_t Cr_total = (int32_t)output_ycc_img[bottom_left].Cr + (int32_t)output_ycc_img[bottom_right].Cr + (int32_t)output_ycc_img[top_left].Cr + (int32_t)output_ycc_img[top_right].Cr;
            int8_t Cr_avg = (int8_t)((Cr_total >> 2) & 0xFF);

            output_ycc_img[bottom_left].Cb = Cb_avg;
            output_ycc_img[bottom_right].Cb = Cb_avg;
            output_ycc_img[top_left].Cb = Cb_avg;
            output_ycc_img[top_right].Cb = Cb_avg;

            output_ycc_img[bottom_left].Cr = Cr_avg;
            output_ycc_img[bottom_right].Cr = Cr_avg;
            output_ycc_img[top_left].Cr = Cr_avg;
            output_ycc_img[top_right].Cr = Cr_avg;
        }
    }
}