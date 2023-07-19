#include <stdio.h>
#include <stdlib.h>
#include "converters.h"
#include "bmp_parser.h"
#include "bmp_handler.h"
#include "utilities.h"

int main(int argc, char *argv[])
{
    // Declarations
    RGB_image_t *rgb_in_img;
    RGB_image_t *rgb_out_img;
    YCC_image_t *ycc_out_img;

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

    // Run processes
    set_img_header_from_file(rgb_in_img, in_file_rgb);
    allocate_rgb_pixels_memory(rgb_in_img);
    set_img_pixels_from_file(rgb_in_img, in_file_rgb);

    // Copy info from input file to output files
    copy_file(in_file_rgb, out_file_cb);
    copy_file(in_file_rgb, out_file_cr);
    copy_file(in_file_rgb, out_file_luma);
    copy_file(in_file_rgb, out_file_rgb);

    // Copy info from input image to output imagess
    copy_rgb_to_yyc(rgb_in_img, ycc_out_img);
    copy_rgb_to_rgb(rgb_in_img, rgb_out_img);

    // Compute and allocate conversions
    compute_ycc_from_rgb(rgb_in_img, ycc_out_img);
    compute_rgb_from_ycc(ycc_out_img, rgb_out_img);

    // Free memory
    free(rgb_in_img->pixels);
    free(ycc_out_img->pixels);
    free(rgb_in_img);
    free(rgb_out_img);
    free(ycc_out_img);

    // Close files
    fclose(in_file_rgb);
    fclose(out_file_cb);
    fclose(out_file_cr);
    fclose(out_file_luma);
    fclose(out_file_rgb);
}
