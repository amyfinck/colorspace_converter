#ifndef TRANSFORMERS_H
#define TRANSFORMERS_H

#include <stdint.h>

uint8_t compute_ycc_y(uint8_t R, uint8_t G, uint8_t B)
{
    uint32_t temp = (16 << K) + (66 * R) + (129 * G) + (25 * B);
    return (uint8_t)((temp + (1 << (K - 1))) >> K);
}

uint8_t compute_ycc_cb(uint8_t R, uint8_t G, uint8_t B)
{
    uint32_t temp = (128 << K) - (38 * R) - (74 * G) + (112 * B);
    uint8_t cb = (uint8_t)((temp + (1 << (K - 1))) >> K);
    if (cb < 0)
        return 0;
    return cb > 255 ? 255 : cb;
}

uint8_t compute_ycc_cr(uint8_t R, uint8_t G, uint8_t B)
{
    uint32_t temp = (128 << K) + (112 * R) - (94 * G) - (18 * B);
    return (uint8_t)((temp + (1 << (K - 1))) >> K);
}

uint8_t compute_rgb_r(uint8_t Y, uint8_t Cb, uint8_t Cr)
{
    int16_t y = (int16_t)Y[i] - 16;
    int16_t cr = (int16_t)Cr[i] - 128;

    int16_t red = (298 * y + 409 * cr + 128) >> 8;
    return (uint8_t)(red < 0 ? 0 : (red > 255 ? 255 : red));
}

uint8_t compute_rgb_g(uint8_t Y, uint8_t Cb, uint8_t Cr)
{
    int16_t y = (int16_t)Y[i] - 16;
    int16_t cb = (int16_t)Cb[i] - 128;
    int16_t cr = (int16_t)Cr[i] - 128;

    int16_t green = (298 * y - 100 * cb - 208 * cr + 128) >> 8;
    return (uint8_t)(green < 0 ? 0 : (green > 255 ? 255 : green));
}

uint8_t compute_rgb_b(uint8_t Y, uint8_t Cb, uint8_t Cr)
{
    int16_t y = (int16_t)Y[i] - 16;
    int16_t cb = (int16_t)Cb[i] - 128;
    int16_t cr = (int16_t)Cr[i] - 128;

    int16_t blue = (298 * y + 516 * cb + 128) >> 8;
    return (uint8_t)(blue < 0 ? 0 : (blue > 255 ? 255 : blue));
}

#endif