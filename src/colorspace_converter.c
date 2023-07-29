#include "colorspace_converter.h"

void get_image_info(RGB_image_t *input_rgb_img, FILE* file)
{
    // get file size
    fseek(file, 0, SEEK_END);
    input_rgb_img->file_size = ftell(file);

    // get offset, width, and height
    if (fseek(file, 10, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); ; exit(1);
    }
    fread(&input_rgb_img->offset, 4, 1, file);
    if (fseek(file, 18, SEEK_SET) != 0){
        printf("Error seeking to width position\n"); ; exit(1);
    }
    fread(&input_rgb_img->width, 4, 1, file);
    if (fseek(file, 22, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); ; exit(1);
    }
    fread(&input_rgb_img->height, 4, 1, file);
}

void read_pixels(RGB_image_t *input_rgb_img, FILE* file)
{
    // allocate memory for pixels
    input_rgb_img->pixel_count = 0;
    allocate_rgb_pixels_memory(input_rgb_img);

    // calculate padding
    uint32_t bytesPerRow = get_row_byte_count(input_rgb_img->width);
    uint32_t pixel_offset;
    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < input_rgb_img->height; y++)
    {
        uint32_t x;
        for(x = 0; x < input_rgb_img->width; x++)
        {
            pixel_offset = (y * bytesPerRow) + (x * 3);

            // Pixels are stored in BGR order
            if (fseek(file, (long)input_rgb_img->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); fclose(file); exit(1);
            }
            fread(&input_rgb_img->pixels[pixel_index].B, 1, 1, file);
            fread(&input_rgb_img->pixels[pixel_index].G, 1, 1, file);
            fread(&input_rgb_img->pixels[pixel_index].R, 1, 1, file);

            pixel_index++;
            input_rgb_img->pixel_count++;
        }
    }
}

void write_header(RGB_image_t *input_rgb_img, FILE* file_to_write, FILE* reference_file)
{
    // copy input file to RBG locations
    uint32_t header_length = input_rgb_img->offset;
    char buffer[header_length];
    size_t bytesRead;
    size_t totalBytesRead = 0;
    fseek(reference_file, 0, SEEK_SET);

    // copy over header
    while (totalBytesRead < header_length && (bytesRead = fread(buffer, 1, header_length - totalBytesRead, reference_file)) > 0)
    {
        if( fwrite(buffer, 1, bytesRead, file_to_write) == -1) {
            printf("Error writing to luma file\n"); exit(1);
        }
        totalBytesRead += bytesRead;
    }
}

void write_rgb(RGB_image_t *input_rgb_img, RGB_image_t *output_rgb_img, FILE* rgb_file)
{
    // calculate padding
    uint32_t bytesPerRow = input_rgb_img->width * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }

    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < input_rgb_img->height; y++)
    {
        uint32_t x;
        for (x = 0; x < input_rgb_img->width; x++)
        {
            // Pixels are stored in BGR order
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Write RBG file
            if (fseek(rgb_file, input_rgb_img->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); ; exit(1);
            }
            fwrite(&output_rgb_img->pixels[pixel_index].B, 1, 1, rgb_file);
            fwrite(&output_rgb_img->pixels[pixel_index].G, 1, 1, rgb_file);
            fwrite(&output_rgb_img->pixels[pixel_index].R, 1, 1, rgb_file);

            pixel_index++;
        }
    }
}

void write_ycc_components(RGB_image_t *input_rgb_img, YCC_image_t *output_ycc_img, FILE* luma_fp, FILE* cb_fp, FILE* cr_fp)
{
    // calculate padding
    uint32_t bytesPerRow = get_row_byte_count(input_rgb_img->width);
    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < input_rgb_img->height; y++)
    {
        uint32_t x;
        for (x = 0; x < input_rgb_img->width; x++)
        {
            // Pixels are stored in BGR order
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Write luma RBG file (grayscale)
            if (fseek(luma_fp, input_rgb_img->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); ; exit(1);
            }
            fwrite(&output_ycc_img->pixels[pixel_index].Y, 1, 1, luma_fp);
            fwrite(&output_ycc_img->pixels[pixel_index].Y, 1, 1, luma_fp);
            fwrite(&output_ycc_img->pixels[pixel_index].Y, 1, 1, luma_fp);

            pixel_index++;
        }
    }

    bytesPerRow = get_row_byte_count(input_rgb_img->width / 2);
    pixel_index = 0;
    for(y = 0; y < input_rgb_img->height/2; y++)
    {
        uint32_t x;
        for (x = 0; x < input_rgb_img->width/2; x++)
        {
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Convert Cb and Cr to unsigned for use in RGB representation
            uint8_t baseColor = 128;
            if (fseek(cb_fp, input_rgb_img->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); ; exit(1);
            }
            fwrite(&output_ycc_img->pixels[pixel_index].Cb, 1, 1, cb_fp);
            fwrite(&baseColor, 1, 1, cb_fp);
            fwrite(&baseColor, 1, 1, cb_fp);

            // Write red difference RBG file
            if (fseek(cr_fp, input_rgb_img->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); ; exit(1);
            }
            fwrite(&baseColor, 1, 1, cr_fp);
            fwrite(&baseColor, 1, 1, cr_fp);
            fwrite(&output_ycc_img->pixels[pixel_index].Cr, 1, 1, cr_fp);

            pixel_index+=2;
        }
        pixel_index += input_rgb_img->width;
    }
}

int main(int argc, char* argv[] )
{
    int32_t outputComponents = 0;
    RGB_image_t *input_rgb_img;
    YCC_image_t *output_ycc_img;
    RGB_image_t *output_rgb_img;

    FILE* in_fp;
    FILE* luma_fp;
    FILE* cb_fp;
    FILE* cr_fp;
    FILE* out_fp;

    if (argc < 2) {
        printf("Error: Missing mandatory input .BMP file.\n");
        printf("Usage: %s <input_file.BMP> [-c]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++)
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
            fclose(in_fp); fclose(luma_fp); printf("error creating luma/%s\n", argv[1]); exit(1);
        }
        chdir(".."); chdir("cb");
        cb_fp = fopen(argv[1], "w+");
        if (cb_fp == NULL)
        {
            printf("error creating cb/%s\n", argv[1]); exit(1);
        }
        chdir(".."); chdir("cr");
        cr_fp = fopen(argv[1], "w+");
        if (cr_fp == NULL)
        {
            printf("error creating cr/%s\n", argv[1]); exit(1);
        }
        chdir("..");
    }
    chdir("rgb");
    out_fp = fopen(argv[1], "w+");
    if (out_fp == NULL) {
        printf("error creating RBG/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("..");

    // create image structs
    input_rgb_img = malloc(sizeof(RGB_image_t));
    if(input_rgb_img == NULL)
    {
        printf("error - input malloc failed\n"); exit(1);
    }
    output_ycc_img = malloc(sizeof(YCC_image_t));
    if(output_ycc_img == NULL)
    {
        printf("error - YCC output malloc failed\n"); exit(1);
    }
    output_rgb_img = malloc(sizeof(RGB_image_t));
    if(output_ycc_img == NULL)
    {
        printf("error - RGB output malloc failed\n"); exit(1);
    }

    // get relevant information from header
    get_image_info(input_rgb_img, in_fp);

    // read pixels into inputRGBImage pixels
    read_pixels(input_rgb_img, in_fp);

    // Write the headers of the output files
    write_header(input_rgb_img, out_fp, in_fp);
    if(outputComponents == 1)
    {
        write_header(input_rgb_img, luma_fp, in_fp);
        write_header(input_rgb_img, cr_fp, in_fp);
        write_header(input_rgb_img, cb_fp, in_fp);
        resize_file(cb_fp, input_rgb_img->width/2, input_rgb_img->height/2);
        resize_file(cr_fp, input_rgb_img->width/2, input_rgb_img->height/2);
    }

    output_ycc_img->pixels = malloc(sizeof(YCC_pixel_t) * input_rgb_img->height * input_rgb_img->width);
    if(output_ycc_img->pixels == NULL){
        printf("error - Output pix malloc failed\n"); exit(1);
    }
    output_rgb_img->pixels = malloc(sizeof(RGB_pixel_t) * input_rgb_img->height * input_rgb_img->width);
    if(output_rgb_img->pixels == NULL){
        printf("error - Output pix malloc failed\n"); exit(1);
    }

    // Calculate YCC values for OutputImage
    get_luma(input_rgb_img, output_ycc_img);
    get_chroma(input_rgb_img, output_ycc_img);
    downsample_chroma(input_rgb_img, output_ycc_img);

    // use downsampled YCC image to create new RGB image
    ycc_to_rgb(input_rgb_img, output_rgb_img, output_ycc_img);

    // write YCC values to RBG files
    if(outputComponents == 1)
    {
        write_ycc_components(input_rgb_img, output_ycc_img, luma_fp, cb_fp, cr_fp);
    }
    write_rgb(input_rgb_img, output_rgb_img, out_fp);

    // free memory
    free(input_rgb_img->pixels);
    free(output_rgb_img->pixels);
    free(output_ycc_img->pixels);
    free(input_rgb_img);
    free(output_ycc_img);
    free(output_rgb_img);

    // close all files
    fclose(in_fp);
    if(outputComponents == 1)
    {
        fclose(luma_fp);
        fclose(cb_fp);
        fclose(cr_fp);
    }
    fclose(out_fp);

    printf("Luma component: /luma/%s\nCb component: /cb/%s\nCr component: /cr/%s\n", argv[1], argv[1], argv[1]);

    return 0;
}
