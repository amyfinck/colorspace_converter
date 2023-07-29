#include "colorspace_converter.h"

header_t *header;
RGB_image_t *input_rgb_img;
YCC_image_t *output_ycc_img;
RGB_image_t *output_rgb_img;

void get_image_info(FILE* file)
{
    // get file size
    fseek(file, 0, SEEK_END);
    header->file_size = ftell(file);

    // get offset, width, and height
    if (fseek(file, 10, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); exit(1);
    }
    fread(&header->offset, 4, 1, file);
    if (fseek(file, 18, SEEK_SET) != 0){
        printf("Error seeking to width position\n"); exit(1);
    }
    fread(&header->width, 4, 1, file);
    if (fseek(file, 22, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); exit(1);
    }
    fread(&header->height, 4, 1, file);

    uint32_t bytes_per_row = header->width * 3; // 3 bytes per pixel
    if (bytes_per_row % 4 != 0)
    {
        header->padding= 4 - (bytes_per_row % 4);
    }
    else
    {
        header->padding = 0;
    }
}

void read_pixels(FILE* file)
{
    // allocate memory for pixels
    header->pixel_count = 0;
    input_rgb_img->pixels = malloc(sizeof(RGB_pixel_t) * header->height * header->width);
    if(input_rgb_img->pixels == NULL) {
        printf("error - pixel malloc failed\n"); exit(1);
    }

    // calculate padding
    uint32_t bytesPerRow = header->width * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }

    uint32_t pixel_offset;
    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < header->height; y++)
    {
        uint32_t x;
        for(x = 0; x < header->width; x++)
        {
            pixel_offset = (y * bytesPerRow) + (x * 3);

            // Pixels are stored in BGR order
            if (fseek(file, (long)header->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); fclose(file); exit(1);
            }
            fread(&input_rgb_img->pixels[pixel_index].B, 1, 1, file);
            fread(&input_rgb_img->pixels[pixel_index].G, 1, 1, file);
            fread(&input_rgb_img->pixels[pixel_index].R, 1, 1, file);

            pixel_index++;
            header->pixel_count++;
        }
    }
}

void write_header(FILE* file_to_write, FILE* reference_file)
{
    // copy input file to RBG locations
    uint32_t header_length = header->offset;
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

void write_rgb_file(FILE* rgb_file)
{
    // calculate padding
    uint32_t bytesPerRow = header->width * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
        header->padding = padding;
    }

    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < header->height; y++)
    {
        uint32_t x;
        for (x = 0; x < header->width; x++)
        {
            // Pixels are stored in BGR order
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Write RBG file
            if (fseek(rgb_file, header->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); exit(1);
            }
            fwrite(&output_rgb_img->pixels[pixel_index].B, 1, 1, rgb_file);
            fwrite(&output_rgb_img->pixels[pixel_index].G, 1, 1, rgb_file);
            fwrite(&output_rgb_img->pixels[pixel_index].R, 1, 1, rgb_file);

            pixel_index++;
        }
    }
    uint8_t zero = 0;
    fwrite(&zero, 1, header->padding, rgb_file);
}

void write_ycc_components(FILE* luma_fp, FILE* cb_fp, FILE* cr_fp)
{
    // calculate padding
    uint32_t bytesPerRow = header->width * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }

    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < header->height; y++)
    {
        uint32_t x;
        for (x = 0; x < header->width; x++)
        {
            // Pixels are stored in BGR order
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Write luma RBG file (grayscale)
            if (fseek(luma_fp, header->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); exit(1);
            }
            fwrite(&output_ycc_img->pixels[pixel_index].Y, 1, 1, luma_fp);
            fwrite(&output_ycc_img->pixels[pixel_index].Y, 1, 1, luma_fp);
            fwrite(&output_ycc_img->pixels[pixel_index].Y, 1, 1, luma_fp);

            pixel_index++;
        }
    }
    fseek(luma_fp, header->padding, SEEK_CUR);

    bytesPerRow = header->width / 2 * 3; // 3 bytes per pixel
    uint32_t chroma_padding = 0;
    if (bytesPerRow % 4 != 0)
    {
        chroma_padding = 4 - (bytesPerRow % 4);
        bytesPerRow += chroma_padding;
    }

    pixel_index = 0;
    for(y = 0; y < header->height/2; y++)
    {
        uint32_t x;
        for (x = 0; x < header->width/2; x++)
        {
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Convert Cb and Cr to unsigned for use in RGB representation
            uint8_t baseColor = 128;
            if (fseek(cb_fp, header->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); exit(1);
            }
            fwrite(&output_ycc_img->pixels[pixel_index].Cb, 1, 1, cb_fp);
            fwrite(&baseColor, 1, 1, cb_fp);
            fwrite(&baseColor, 1, 1, cb_fp);

            // Write red difference RBG file
            if (fseek(cr_fp, header->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); exit(1);
            }
            fwrite(&baseColor, 1, 1, cr_fp);
            fwrite(&baseColor, 1, 1, cr_fp);
            fwrite(&output_ycc_img->pixels[pixel_index].Cr, 1, 1, cr_fp);

            pixel_index+=2;
        }
        pixel_index += header->width;
    }
    uint8_t zero = 0;
    fwrite(&zero, 1, chroma_padding, cr_fp);
    fwrite(&zero, 1, chroma_padding, cb_fp);
}

int main(int argc, char* argv[] )
{
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

    FILE* luma_fp;
    FILE* cb_fp;
    FILE* cr_fp;
    FILE* out_fp;

    // open the input, luma, cb, cr, and output files
    chdir("input");
    FILE* in_fp = fopen(argv[1], "r");
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
    input_rgb_img = malloc(sizeof(RGB_image_t));
    if(input_rgb_img == NULL)
    {
        printf("Error - input malloc failed\n"); exit(1);
    }
    output_ycc_img = malloc(sizeof(YCC_image_t));
    if(output_ycc_img == NULL)
    {
        printf("Error - YCC output malloc failed\n"); exit(1);
    }
    output_rgb_img = malloc(sizeof(RGB_image_t));
    if(output_ycc_img == NULL)
    {
        printf("Error - RGB output malloc failed\n"); exit(1);
    }

    header = malloc(sizeof(header_t));
    if(header == NULL)
    {
        printf("Error - header malloc failed\n"); exit(1);
    }

    // get relevant information from header
    get_image_info(in_fp);

    if(header->width % 2 != 0 || header->height % 2 != 0)
    {
        printf("Error - width and height must be divisible by 2\n");
    }

    // read pixels into inputRGBImage pixels
    read_pixels(in_fp);

    // Write the headers of the output files
    write_header(out_fp, in_fp);
    if(outputComponents == 1)
    {
        write_header(luma_fp, in_fp);
        write_header(cr_fp, in_fp);
        write_header(cb_fp, in_fp);
        resize_file(cb_fp, header->width/2, header->height/2);
        resize_file(cr_fp, header->width/2, header->height/2);
    }

    output_ycc_img->pixels = malloc(sizeof(YCC_pixel_t) * header->height * header->width);
    if(output_ycc_img->pixels == NULL){
        printf("Error - Output pix malloc failed\n"); exit(1);
    }
    output_rgb_img->pixels = malloc(sizeof(RGB_pixel_t) * header->height * header->width);
    if(output_rgb_img->pixels == NULL){
        printf("Error - Output pix malloc failed\n"); exit(1);
    }

    // Calculate YCC values for OutputImage
    get_luma(header->pixel_count, input_rgb_img, output_ycc_img);
    get_chroma(header->pixel_count, input_rgb_img, output_ycc_img);
    downsample_chroma(header->height, header->width, input_rgb_img, output_rgb_img);
    ycc_to_rgb(header->pixel_count, input_rgb_img, output_rgb_img, output_ycc_img);

    // write YCC values to RBG files
    write_rgb_file(out_fp);
    if(outputComponents == 1)
    {
        write_ycc_components(luma_fp, cb_fp, cr_fp);
    }

    // free memory
    free(input_rgb_img->pixels);
    free(input_rgb_img);
    free(output_ycc_img->pixels);
    free(output_ycc_img);
    free(output_rgb_img->pixels);
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
    return 0;
}
