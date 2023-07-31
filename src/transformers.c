#include "transformers.h"

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
    int32_t red = (74 * (Y - 16)) + (102 * (Cr - 128));
    red = ((red + (1 << (H - 1))) >> H);
    return (uint8_t)(red < 0 ? 0 : (red > 255 ? 255 : red));
}

uint8_t compute_rgb_g(int16_t Y, int16_t Cb, int16_t Cr)
{
    int32_t green = (74 * (Y - 16)) - (52 * (Cr - 128)) - (25 * (Cb - 128));
    green = ((green + (1 << (H - 1))) >> H);
    return (uint8_t)(green < 0 ? 0 : (green > 255 ? 255 : green));
}

uint8_t compute_rgb_b(int16_t Y, int16_t Cb, int16_t Cr)
{
    int32_t blue = (74 * (Y - 16)) + (129 * (Cb - 128));
    blue = ((blue + (1 << (H - 1))) >> H);
    return (uint8_t)(blue < 0 ? 0 : (blue > 255 ? 255 : blue));
}