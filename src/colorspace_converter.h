#ifndef COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
#define COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include<sys/mman.h>
#include<string.h>
#include <sys/stat.h>
#include <arm_neon.h>

typedef struct RGB_pixel_t
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_pixel_t;

typedef struct RGB_image_t
{
    RGB_pixel_t *pixels;
}RGB_image_t;

typedef struct YCC_pixel_t
{
    uint8_t Y; // luma
    uint8_t Cb; // blue-difference
    uint8_t Cr; // red-difference
}YCC_pixel_t;

typedef struct YCC_image_t
{
    YCC_pixel_t *pixels;
}YCC_image_t;

typedef struct header_t
{
    uint32_t height;
    uint32_t width;
    uint32_t offset;
    uint32_t pixel_count;
    uint32_t padding;
    uint32_t file_size;
}header_t;

void get_image_info(FILE* file);
void read_pixels(FILE* file);
void write_header(FILE* file_to_write, FILE* reference_file);
void resize_file(FILE* file, uint32_t width, uint32_t height);
void getLuma();
void getChroma();
void downsampleChroma();
void YCCToRGB();
void write_RGB(FILE* rgb_file);
void write_YCC_components(FILE* luma_fp, FILE* cb_fp, FILE* cr_fp);


#endif //COLORSPACE_CONVERTER_COLORSPACE_CONVERTER_H
