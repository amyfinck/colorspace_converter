#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "converters.h"
#include "bmp_parser.h"
#include "bmp_handler.h"
#include "utilities.h"

#define PATH_MAX 256

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

    printf(".\\input\\parrot10.bmp\n");
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }

    printf("Concatenated String: %s\n", strcat(cwd, "/input/parrot10.bmp"));
    // // Open files
    in_file_rgb = fopen(strcat(cwd, "/input/parrot10.bmp"));

    if (in_file_rgb == NULL)
    {
        printf("Could not open %s\n", strcat(cwd, "/input/parrot10.bmp"));
        exit(1);
    }
    // out_file_cb = fopen(argv[1], "w+");
    // out_file_cr = fopen(argv[1], "w+");
    // out_file_luma = fopen(argv[1], "w+");
    // out_file_rgb = fopen(argv[1], "w+");

    // // Run processes
    // set_img_header_from_file(rgb_in_img, in_file_rgb);
    // allocate_rgb_pixels_memory(rgb_in_img);
    // rgb_pixels_file_handler(rgb_in_img, in_file_rgb, read_rgb);

    // // Copy info from input file to output files
    // copy_file(in_file_rgb, out_file_cb);
    // copy_file(in_file_rgb, out_file_cr);
    // copy_file(in_file_rgb, out_file_luma);
    // copy_file(in_file_rgb, out_file_rgb);

    // // Copy info from input image to output imagess
    // copy_rgb_to_yyc(rgb_in_img, ycc_out_img);
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
    // free(rgb_in_img);
    // free(rgb_out_img);
    // free(ycc_out_img);

    // Close files
    fclose(in_file_rgb);
    // fclose(out_file_cb);
    // fclose(out_file_cr);
    // fclose(out_file_luma);
    // fclose(out_file_rgb);

    return 0;
}
