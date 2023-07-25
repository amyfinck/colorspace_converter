#ifndef TRANSFORMERS_H
#define TRANSFORMERS_H

#include <stdint.h>

#define K 8

uint8_t compute_ycc_y(uint8_t R, uint8_t G, uint8_t B)
{
    int32_t temp = (16 << K) + (66 * R) + (129 * G) + (25 * B);
    return (uint8_t)((temp + (1 << (K - 1))) >> K);
}

uint8_t compute_ycc_cb(uint8_t R, uint8_t G, uint8_t B)
{
    int32_t temp = (128 << K) - (38 * R) - (74 * G) + (112 * B);
    int32_t cb = (int32_t)((temp + (1 << (K - 1))) >> K);
    return (uint8_t)(cb < 0 ? 0 : (cb > 255 ? 255 : cb));
}

uint8_t compute_ycc_cr(uint8_t R, uint8_t G, uint8_t B)
{
    int32_t temp = (128 << K) + (112 * R) - (94 * G) - (18 * B);
    return (uint8_t)((temp + (1 << (K - 1))) >> K);
}

uint8_t compute_rgb_r(int16_t Y, int16_t Cb, int16_t Cr)
{
    int16_t red = (298 * Y + 409 * Cr + 128) >> K;
    return (uint8_t)(red < 0 ? 0 : (red > 255 ? 255 : red));
}

uint8_t compute_rgb_g(int16_t Y, int16_t Cb, int16_t Cr)
{
    int16_t green = (298 * Y - 100 * Cb - 208 * Cr + 128) >> K;
    return (uint8_t)(green < 0 ? 0 : (green > 255 ? 255 : green));
}

uint8_t compute_rgb_b(int16_t Y, int16_t Cb, int16_t Cr)
{
    int16_t blue = (298 * Y + 516 * Cb + 128) >> K;
    return (uint8_t)(blue < 0 ? 0 : (blue > 255 ? 255 : blue));
}

#endif