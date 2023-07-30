#include "constants.h"

/*
 * y = 16.0 + 0.257 R + 0.504 G + 0.098 B
 * 2^8 * 0.257 = 66
 * 2^8 * 0.504 = 129
 * 2^8 * 0.098 = 25
 */

//void get_ycc_pixels(uint32_t pixel_count, RGB_pixel_t *input_rgb_pixels, YCC_pixel_t *output_ycc_pixels)
//{
//    uint32_t i;
//    for(i = 0; i < pixel_count; i++)
//    {
//        uint8_t R = input_rgb_pixels[i].R;
//        uint8_t G = input_rgb_pixels[i].G;
//        uint8_t B = input_rgb_pixels[i].B;
//        output_ycc_pixels[i].Y = compute_ycc_y(R, G, B);
//        output_ycc_pixels[i].Cb = compute_ycc_cb(R, G, B);
//        output_ycc_pixels[i].Cr = compute_ycc_cr(R, G, B);
//    }
//}

// TODO this is a copy of get_row_byte_count
uint32_t get_row_bytes(uint32_t img_width)
{
    uint32_t row_bytes = img_width * 3;
    if (row_bytes % 4 != 0)
    {
        uint32_t padding = 4 - (row_bytes % 4);
        row_bytes += padding;
    }
    return row_bytes;
}

void get_ycc_pixels(FILE* in_file, header_t* header, YCC_pixel_t * ycc_pixels)
{
    // allocate memory for pixels
    header->pixel_count = 0;
    uint32_t bytes_per_row = get_row_bytes(header->width);
    uint32_t pixel_offset;
    uint32_t pixel_index = 0;

    uint8_t R;
    uint8_t G;
    uint8_t B;

    uint32_t y;
    for (y = 0; y < header->height; y++)
    {
        uint32_t x;
        for (x = 0; x < header->width; x++)
        {
            pixel_offset = (y * bytes_per_row) + (x * 3);

            // Pixels are stored in BGR order
            if (fseek(in_file, (long) header->offset + pixel_offset, SEEK_SET) != 0)
            {
                printf("Error seeking to pixel\n");
                fclose(in_file);
                exit(1);
            }
            fread(&B, 1, 1, in_file);
            fread(&G, 1, 1, in_file);
            fread(&R, 1, 1, in_file);

            ycc_pixels[pixel_index].Y = compute_ycc_y(R, G, B);
            ycc_pixels[pixel_index].Cb = compute_ycc_cb(R, G, B);
            ycc_pixels[pixel_index].Cr = compute_ycc_cr(R, G, B);

            pixel_index++;
            header->pixel_count++;
        }
    }
}

void ycc_to_rgb(header_t * header, RGB_pixel_t* output_rgb_pixels, YCC_pixel_t *output_ycc_pixels, FILE* out_fp)
{
    uint32_t i;
    for(i = 0; i < header->pixel_count; i++)
    {
        uint8_t Y = output_ycc_pixels[i].Y;
        uint8_t Cb = output_ycc_pixels[i].Cb;
        uint8_t Cr = output_ycc_pixels[i].Cr;

        output_rgb_pixels[i].R = compute_rgb_r(Y, Cb, Cr);
        output_rgb_pixels[i].G = compute_rgb_g(Y, Cb, Cr);
        output_rgb_pixels[i].B = compute_rgb_b(Y, Cb, Cr);
    }

    // calculate padding
    uint32_t bytes_per_row = header->width * 3; // 3 bytes per pixel
    if (bytes_per_row % 4 != 0)
    {
        uint32_t padding = 4 - (bytes_per_row % 4);
        bytes_per_row += padding;
        header->padding = padding;
    }

    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < header->height; y++)
    {
        uint32_t x;
        for (x = 0; x < header->width; x++)
        {
            // Pixels are stored in BGR order
            uint32_t pixel_offset = (y * bytes_per_row) + (x * 3);

            // Write RBG file
            if (fseek(out_fp, header->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); exit(1);
            }
            fwrite(&output_rgb_pixels[pixel_index].B, 1, 1, out_fp);
            fwrite(&output_rgb_pixels[pixel_index].G, 1, 1, out_fp);
            fwrite(&output_rgb_pixels[pixel_index].R, 1, 1, out_fp);

            pixel_index++;
        }
    }
    uint8_t zero = 0;
    fwrite(&zero, 1, header->padding, out_fp);

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