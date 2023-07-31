#ifndef TRANSFORMERS_H
#define TRANSFORMERS_H

#include <stdint.h>

#define K 8
#define H 6

uint8_t compute_ycc_y(uint8_t R, uint8_t G, uint8_t B);
uint8_t compute_ycc_cb(uint8_t R, uint8_t G, uint8_t B);
uint8_t compute_ycc_cr(uint8_t R, uint8_t G, uint8_t B);
uint8_t compute_rgb_r(int16_t Y, int16_t Cb, int16_t Cr);
uint8_t compute_rgb_g(int16_t Y, int16_t Cb, int16_t Cr);
uint8_t compute_rgb_b(int16_t Y, int16_t Cb, int16_t Cr);

#endif