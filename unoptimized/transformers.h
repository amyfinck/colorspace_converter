#ifndef TRANSFORMERS_H
#define TRANSFORMERS_H

#include <stdint.h>

#define K 8

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
    int64_t temp = (74 * (Y - 16)) + (102 * (Cr - 128));
    if (temp < 0)
        temp = 0;
    return (uint8_t)((temp + (1 << (K - 1))) >> K);
}

uint8_t compute_rgb_g(uint8_t Y, uint8_t Cb, uint8_t Cr)
{
    int64_t temp = (74 * (Y - 16)) - (52 * (Cr - 128)) - (25 * (Cb - 128));
    if (temp < 0)
        temp = 0;
    return (uint8_t)((temp + (1 << (K - 1))) >> K);
}

uint8_t compute_rgb_b(uint8_t Y, uint8_t Cb, uint8_t Cr)
{
    int64_t temp = (74 * (Y - 16)) + (129 * (Cb - 128));
    if (temp < 0)
        temp = 0;
    return (uint8_t)((temp + (1 << (K - 1))) >> K);
}

#endif