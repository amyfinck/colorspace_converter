#include "colorspace_converter.h"

int main(int argc, char* argv[] )
{
    header_t *header;
    RGB_image_t *input_rgb_img;
    YCC_image_t *output_ycc_img;
    RGB_image_t *output_rgb_img;

    int32_t outputComponents = 0;

    if (argc < 2) {
        printf("Error: Missing mandatory input .BMP file.\n");
        printf("Usage: %s <input_file.BMP> [-c]\n", argv[0]);
        return 1;
    }

    int i;
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-c") == 0)
            {
                outputComponents = 1;
            } else
            {
                printf("Error: Unrecognized flag %s\n", argv[i]);
                printf("Usage: %s <input_file.BMP> [-c]\n", argv[0]);
                return 1;
            }
        }
    }

    FILE* in_fp;
    FILE* luma_fp;
    FILE* cb_fp;
    FILE* cr_fp;
    FILE* out_fp;

    // open the input, luma, cb, cr, and output files
    chdir("input");
    in_fp = fopen(argv[1], "r");
    if (in_fp == NULL) {
        printf("Could not open %s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("output");
    if(outputComponents == 1)
    {
        chdir("luma");
        luma_fp = fopen(argv[1], "w+");
        if (luma_fp == NULL)
        {
            fclose(in_fp); fclose(luma_fp); printf("Error creating luma/%s\n", argv[1]); exit(1);
        }
        chdir(".."); chdir("cb");
        cb_fp = fopen(argv[1], "w+");
        if (cb_fp == NULL)
        {
            printf("Error creating cb/%s\n", argv[1]); exit(1);
        }
        chdir(".."); chdir("cr");
        cr_fp = fopen(argv[1], "w+");
        if (cr_fp == NULL)
        {
            printf("Error creating cr/%s\n", argv[1]); exit(1);
        }
        chdir("..");
    }
    chdir("rgb");
    out_fp = fopen(argv[1], "w+");
    if (out_fp == NULL) {
        printf("Error creating RBG/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("..");
    
    // create image structs
    allocate_rgb_memory(&input_rgb_img);
    allocate_rgb_memory(&output_rgb_img);
    allocate_ycc_memory(&output_ycc_img);
    allocate_header_memory(&header);

    // get relevant information from header
    get_image_info(header, in_fp);
    check_height_width(header->width, header->height);
    rgb_pixels_file_handler(header, input_rgb_img, in_fp, read_rgb);

    // Write the headers of the output files
    write_header(header->offset, out_fp, in_fp);
    if(outputComponents == 1)
    {
        write_header(header->offset, luma_fp, in_fp);
        write_header(header->offset, cr_fp, in_fp);
        write_header(header->offset, cb_fp, in_fp);
        resize_file(cb_fp, header->width/2, header->height/2);
        resize_file(cr_fp, header->width/2, header->height/2);
    }

    // Allocate pixel memory
    allocate_ycc_pixels_memory(header->height, header->width, output_ycc_img);
    allocate_rgb_pixels_memory(header->height, header->width, output_rgb_img);

    // Calculate YCC values for OutputImage
    get_luma(header->pixel_count, input_rgb_img, output_ycc_img);
    get_chroma(header->pixel_count, input_rgb_img, output_ycc_img);
    downsample_chroma(header->height, header->width, output_ycc_img);
    ycc_to_rgb(header->pixel_count, output_rgb_img, output_ycc_img);

    // write YCC values to RBG files
    rgb_pixels_file_handler(header, output_rgb_img, out_fp, write_rgb);
    if(outputComponents == 1)
    {
        ycc_pixels_file_handler(header, output_ycc_img, luma_fp, write_luma);
        ycc_pixels_file_handler(header, output_ycc_img, cb_fp, write_cb);
        ycc_pixels_file_handler(header, output_ycc_img, cr_fp, write_cr);
    }

    // free memory
    free(input_rgb_img->pixels);
    free(output_ycc_img->pixels);
    free(output_rgb_img->pixels);
    free(input_rgb_img);
    free(output_ycc_img);
    free(output_rgb_img);

    // close all files
    fclose(in_fp);
    fclose(out_fp);
    if(outputComponents == 1)
    {
        fclose(luma_fp);
        fclose(cb_fp);
        fclose(cr_fp);
    }
    return 0;
}
