#include "colorspace_converter.h"

int main(int argc, char* argv[] )
{
    header_t *header;

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
    header = (header_t *)malloc(sizeof(header_t));
    if(header == NULL)
    {
        printf("Error: Malloc for structs failed\n");
        exit(1);
    }

    // get relevant information from header
    get_image_info(header, in_fp);
    uint32_t width = header->width;
    uint32_t height = header->height;
    uint32_t offset = header->offset;
    check_height_width(width, height);

    // Allocate pixel memory
    YCC_pixel_t * output_ycc = (YCC_pixel_t *)malloc(sizeof(YCC_pixel_t) * width * height);
    RGB_pixel_t * output_rgb = (RGB_pixel_t *)malloc(sizeof(RGB_pixel_t) * width * height);
    if( output_ycc == NULL || output_rgb == NULL)
    {
        printf("Error: Malloc for structs failed\n");
        exit(1);
    }

    // Write the headers of the output files
    write_header(offset, out_fp, in_fp);
    if(outputComponents == 1)
    {
        write_header(offset, luma_fp, in_fp);
        write_header(offset, cr_fp, in_fp);
        write_header(offset, cb_fp, in_fp);
        resize_file(cb_fp, width/2, height/2);
        resize_file(cr_fp, width/2, height/2);
    }

    // Calculate YCC values for OutputImage
    rgb_to_ycc(header,  output_ycc, in_fp);
    downsample_chroma(height, width, output_ycc);
    ycc_to_rgb(width * height, output_rgb, output_ycc);

    // write YCC values to RBG files
    write_pixels_rgb(header, output_rgb, out_fp);
    if(outputComponents == 1)
    {
        // write_ycc_components(header, output_ycc_img, luma_fp, cb_fp, cr_fp);
        write_pixels_luma(header, output_ycc, luma_fp);
        write_pixels_cb(header, output_ycc, cb_fp);
        write_pixels_cr(header, output_ycc, cr_fp);
    }

    // free memory
    free(header);
    free(output_ycc);
    free(output_rgb);

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
