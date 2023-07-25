#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include "converters.h"
#include "bmp_parser.h"
#include "bmp_handler.h"
#include "utilities.h"

int main(int argc, char *argv[])
{
    // Image declarations
    RGB_image_t *rgb_in_img;
    RGB_image_t *rgb_out_img;
    YCC_image_t *ycc_out_img;

    // File declarations
    FILE *in_file_rgb;
    FILE *out_file_cb;
    FILE *out_file_cr;
    FILE *out_file_luma;
    FILE *out_file_rgb;

    // char *if_rgb_path = get_abs_path("input/parrot500.bmp");
    // char *of_cb_path = get_abs_path("output/cb/parrot10.bmp");
    // char *of_cr_path = get_abs_path("output/cr/parrot10.bmp");
    // char *of_lum_path = get_abs_path("output/luma/parrot10.bmp");
    // char *of_rgb_path = get_abs_path("output/rgb/parrot10.bmp");

    // Open files
    chdir("input");
    in_file_rgb = fopen(argv[1], "r");

    // Allocate memory
    allocate_rgb_memory(rgb_in_img);
    allocate_ycc_memory(ycc_out_img);
    allocate_rgb_memory(rgb_out_img);

    // Read rgb input file
    set_img_header_from_file(rgb_in_img, in_file_rgb);
    allocate_rgb_pixels_memory(rgb_in_img);
    rgb_pixels_file_handler(rgb_in_img, in_file_rgb, read_rgb);

    chdir(".."); chdir("output/cb");
    out_file_cb = fopen(argv[1], "w+");
    chdir(".."); chdir("cr");
    out_file_cr = fopen(argv[1], "w+");
    chdir(".."); chdir("luma");
    out_file_luma = fopen(argv[1], "w+");
    chdir(".."); chdir("rgb");
    out_file_rgb = fopen(argv[1], "w+");

    // Copy info from input file to output files
    copy_file(in_file_rgb, out_file_cb);
    copy_file(in_file_rgb, out_file_cr);
    copy_file(in_file_rgb, out_file_luma);
    copy_file(in_file_rgb, out_file_rgb);

    // Copy info from input image to output images
    copy_rgb_to_yyc(rgb_in_img, ycc_out_img);
    // copy_rgb_to_rgb(rgb_in_img, rgb_out_img);

    // // Compute and allocate conversions
    // compute_ycc_from_rgb(rgb_in_img, ycc_out_img);
    // downsample_ycc_chroma(ycc_out_img);
    // compute_rgb_from_ycc(ycc_out_img, rgb_out_img);

    // // Write the images to output files
    // ycc_pixels_file_handler(ycc_out_img, out_file_luma, write_luma);
    // ycc_pixels_file_handler(ycc_out_img, out_file_cb, write_cb);
    // ycc_pixels_file_handler(ycc_out_img, out_file_cr, write_cr);
    // rgb_pixels_file_handler(rgb_out_img, out_file_rgb, write_rgb);

    // Free memory
    // free(rgb_in_img->pixels);
    // free(ycc_out_img->pixels);
    // free(rgb_out_img->pixels);
    // free(rgb_in_img);
    // free(rgb_out_img);
    // free(ycc_out_img);

    // Close files
    fclose(in_file_rgb);
    fclose(out_file_cb);
    fclose(out_file_cr);
    fclose(out_file_luma);
    fclose(out_file_rgb);

    return 0;
}
