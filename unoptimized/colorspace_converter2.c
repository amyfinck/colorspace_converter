#include <stdio.h>
#include <stdlib.h>
#include "converters.h"
#include "bmp_parser.h"
#include "utilities.h"

int main(int argc, char *argv[])
{
    // Declarations
    RGB_image_t *rgb_in_img;
    RGB_image_t *rgb_out_img;
    YCC_image_t *yyc_out_img;

    FILE *in_file_rgb;
    FILE *out_file_cb;
    FILE *out_file_cr;
    FILE *out_file_luma;
    FILE *out_file_rgb;

    // Open files
    in_file_rgb = fopen(argv[1], "r");
    out_file_cb = fopen(argv[1], "w+");
    out_file_cr = fopen(argv[1], "w+");
    out_file_luma = fopen(argv[1], "w+");
    out_file_rgb = fopen(argv[1], "w+");

    //----------------------------------
    set_img_header_from_file(rgb_in_img, in_file_rgb);
    allocate_rgb_pixels_memory(rgb_in_img);
    set_img_pixels_from_file(rgb_in_img, in_file_rgb);

    // Free memory
    free(rgb_in_img->pixels);
    free(yyc_out_img->pixels);
    free(rgb_in_img);
    free(rgb_out_img);
    free(yyc_out_img);

    // Close files
    fclose(in_file_rgb);
    fclose(out_file_cb);
    fclose(out_file_cr);
    fclose(out_file_luma);
    fclose(out_file_rgb);
}
