#ifndef PROG_CONSTS_H
#define PROG_CONSTS_H

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct RGB_pixel_t
{
    uint8_t R; // Red component
    uint8_t G; // Green component
    uint8_t B; // Blue component
} RGB_pixel_t;

typedef struct YCC_pixel_t
{
    uint8_t Y;  // Luma
    uint8_t Cb; // Blue-difference
    uint8_t Cr; // Red-difference
} YCC_pixel_t;

typedef struct RGB_image_t
{
    uint32_t height;
    uint32_t width;
    uint32_t offset;
    uint32_t pixel_count;
    uint32_t padding;
    uint32_t file_size;
    RGB_pixel_t *pixels;
} RGB_image_t;

typedef struct YCC_image_t
{
    uint32_t height;
    uint32_t width;
    uint32_t offset;
    uint32_t pixel_count;
    uint32_t padding;
    uint32_t file_size;
    YCC_pixel_t *pixels;
} YCC_image_t;

#endif