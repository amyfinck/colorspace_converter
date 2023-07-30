#ifndef CONSTANTS
#define CONSTANTS

#include <stdlib.h>
#include <stdio.h>

typedef struct RGB_pixel_t
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGB_pixel_t;

typedef struct RGB_image_t
{
    RGB_pixel_t *pixels;
} RGB_image_t;

typedef struct YCC_pixel_t
{
    uint8_t Y; // luma
    uint8_t Cb; // blue-difference
    uint8_t Cr; // red-difference
} YCC_pixel_t;

typedef struct YCC_image_t
{
    YCC_pixel_t *pixels;
} YCC_image_t;

typedef struct header_t
{
    uint32_t height;
    uint32_t width;
    uint32_t offset;
    uint32_t padding;
    uint32_t pixel_count;
    uint32_t file_size;
} header_t;

void exit_on_error(int condition, char *msg)
{
    if (condition)
    {
        printf("Error: %s\n", msg);
        exit(1);
    }
}

#endif //CONSTANTS