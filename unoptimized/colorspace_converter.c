#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include<sys/mman.h>
#include<string.h>
#include <sys/stat.h>

typedef struct RGB_pixel_t
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_pixel_t;

typedef struct RGB_image_t
{
    uint32_t height;
    uint32_t width;
    uint32_t offset;
    RGB_pixel_t *pixels;
    uint32_t pixel_count;
    uint32_t file_size;
    uint32_t data_size;
}RGB_image_t;

typedef struct YCC_pixel_t
{
    uint8_t Y; // luma
    uint8_t Cb; // blue-difference
    uint8_t Cr; // red-difference
}YCC_pixel_t;

typedef struct YCC_image_t
{
    uint32_t height;
    uint32_t width;
    uint32_t offset;
    YCC_pixel_t *pixels;
    uint32_t pixel_count;
    uint32_t padding;
}YCC_image_t;

RGB_image_t *input_rgb_img;
YCC_image_t *output_ycc_img;
RGB_image_t *output_rgb_img;

/*
 * y = 16.0 + 0.257 R + 0.504 G + 0.098 B
 * 2^8 * 0.257 = 66
 * 2^8 * 0.504 = 129
 * 2^8 * 0.098 = 25
 */
void get_luma()
{
    uint8_t K = 8;
    uint32_t i;
    for(i = 0; i < input_rgb_img->pixel_count; i++)
    {
        uint8_t R = input_rgb_img->pixels[i].R;
        uint8_t G = input_rgb_img->pixels[i].G;
        uint8_t B = input_rgb_img->pixels[i].B;
        uint32_t Y_temp = (16 << K) + (66 * R) + (129 * G) + (25 * B);
        uint8_t Y = (uint8_t)(( Y_temp + (1 << (K-1))) >> K);

        output_ycc_img->pixels[i].Y = Y;
    }
}

/*
 * cb = 128.0 - 0.148 R - 0.291 G + 0.439 B
 * cr = 128.0 + 0.439 R - 0.368 G - 0.071 B
 * Both Cb and Cr are unsigned with value is unsigned from 0 to 255
 *
 * 0.148 * 2^8 = 38
 * 0.291 * 2^8 = 74
 * 0.439 * 2^8 = 112
 * 0.368 * 2^8 = 94
 * 0.071 * 2^8 = 18
 */
void get_chroma()
{
    uint32_t i;
    for(i = 0; i < input_rgb_img->pixel_count; i++)
    {
        uint8_t K = 8;

        // Cb and Cr are signed from -128 to 127
        uint8_t R = input_rgb_img->pixels[i].R;
        uint8_t G = input_rgb_img->pixels[i].G;
        uint8_t B = input_rgb_img->pixels[i].B;

        uint32_t Cb_temp = (128 << K) - (38 * R) - (74 * G) + (112 * B);
        uint32_t Cr_temp = (128 << K) + (112 * R) - (94 * G) - (18 * B);

        uint8_t Cb = (uint8_t)(( Cb_temp + (1 << (K -1))) >> K);
        uint8_t Cr = (uint8_t)(( Cr_temp + (1 << (K -1))) >> K);

        if(Cb < 0) Cb = 0;
        if(Cb > 255) Cb = 255;

        output_ycc_img->pixels[i].Cb = (uint8_t)Cb;
        output_ycc_img->pixels[i].Cr = (uint8_t)Cr;
    }
}

void downsample_chroma()
{
    // TODO this should probably be a scaled down image
    uint32_t y;
    for(y = 0; y < input_rgb_img->height; y+=2)
    {
        uint32_t x;
        for(x = 0; x < input_rgb_img->width; x+=2)
        {
            uint32_t bottom_left = y * input_rgb_img->width + x;
            uint32_t bottom_right = bottom_left + 1;
            uint32_t top_left = bottom_left + input_rgb_img->width;
            uint32_t top_right = top_left + 1;

            int32_t Cb_total = (int32_t)output_ycc_img->pixels[bottom_left].Cb + (int32_t)output_ycc_img->pixels[bottom_right].Cb + (int32_t)output_ycc_img->pixels[top_left].Cb + (int32_t)output_ycc_img->pixels[top_right].Cb;
            int8_t Cb_avg = (int8_t)(Cb_total / 4);
            int32_t Cr_total = (int32_t)output_ycc_img->pixels[bottom_left].Cr + (int32_t)output_ycc_img->pixels[bottom_right].Cr + (int32_t)output_ycc_img->pixels[top_left].Cr + (int32_t)output_ycc_img->pixels[top_right].Cr;
            int8_t Cr_avg = (int8_t)(Cr_total / 4);

            output_ycc_img->pixels[bottom_left].Cb = Cb_avg;
            output_ycc_img->pixels[bottom_right].Cb = Cb_avg;
            output_ycc_img->pixels[top_left].Cb = Cb_avg;
            output_ycc_img->pixels[top_right].Cb = Cb_avg;

            output_ycc_img->pixels[bottom_left].Cr = Cr_avg;
            output_ycc_img->pixels[bottom_right].Cr = Cr_avg;
            output_ycc_img->pixels[top_left].Cr = Cr_avg;
            output_ycc_img->pixels[top_right].Cr = Cr_avg;
        }
    }
}

/*
 * R′ = 1.164(Y′−16) + 1.596(Cr−128)
 * G′ = 1.164(Y′−16) − 0.813(Cr-128) − 0.391(Cb−128)
 * B′ = 1.164(Y′−16) + 2.018(Cb−b128)
 *
 * 2^6 * 1.164 = 74
 * 2^6 * 1.596 = 102
 * 2^6 * 0.813 = 52
 * 2^6 * 0.391 = 25
 * 2^6 * 2.018 = 129
 */
void ycc_to_rgb()
{
    uint32_t i;
    for(i = 0; i < input_rgb_img->pixel_count; i++)
    {
        uint8_t K = 6;

        uint8_t Y = output_ycc_img->pixels[i].Y;
        uint8_t Cb = output_ycc_img->pixels[i].Cb;
        uint8_t Cr = output_ycc_img->pixels[i].Cr;

        int32_t R_temp = (74 * (Y-16)) + (102 * (Cr-128));
        int32_t G_temp = (74 * (Y-16)) - (52 * (Cr-128)) - (25 * (Cb-128));
        int32_t B_temp = (74 * (Y-16)) + (129 * (Cb-128));

        R_temp = (( R_temp + (1 << (6-1))) >> 6);
        G_temp = (( G_temp + (1 << (6-1))) >> 6);
        B_temp = (( B_temp + (1 << (6-1))) >> 6);

        // Saturating Arithmetic
        if(R_temp < 0) R_temp = 0;
        if(R_temp > 255) R_temp = 255;
        if(G_temp < 0) G_temp = 0;
        if(G_temp > 255) G_temp = 255;
        if(B_temp < 0) B_temp = 0;
        if(B_temp > 255) B_temp = 255;

        uint8_t R = (uint8_t)R_temp;
        uint8_t G = (uint8_t)G_temp;
        uint8_t B = (uint8_t)B_temp;

        output_rgb_img->pixels[i].R = (uint8_t)R;
        output_rgb_img->pixels[i].G = (uint8_t)G;
        output_rgb_img->pixels[i].B = (uint8_t)B;
    }
}

void read_pixels(FILE* file)
{
    // allocate memory for pixels
    input_rgb_img->pixel_count = 0;
    input_rgb_img->pixels = malloc(sizeof(RGB_pixel_t) * input_rgb_img->height * input_rgb_img->width);
    if(input_rgb_img->pixels == NULL) {
        printf("error - pixel malloc failed\n"); exit(1);
    }

    // calculate padding
    uint32_t bytesPerRow = input_rgb_img->width * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }
    input_rgb_img->data_size = bytesPerRow * input_rgb_img->height;

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

void resize_file(FILE* file, uint32_t width, uint32_t height)
{
    if (fseek(file, 18, SEEK_SET) != 0){
        printf("Error seeking to width position\n"); ; exit(1);
    }
    fwrite(&width, 4, 1, file);
    if (fseek(file, 22, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); ; exit(1);
    }
    fwrite(&height, 4, 1, file);
}

void write_header(FILE* file_to_write, FILE* reference_file)
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

void write_rgb(FILE* rgb_file)
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
    for(y = 0; y < output_ycc_img->height; y++)
    {
        uint32_t x;
        for (x = 0; x < output_ycc_img->width; x++)
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
            output_ycc_img->pixel_count++;
        }
    }

}

void write_ycc_components(FILE* luma_fp, FILE* cb_fp, FILE* cr_fp)
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
    for(y = 0; y < output_ycc_img->height; y++)
    {
        uint32_t x;
        for (x = 0; x < output_ycc_img->width; x++)
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
            output_ycc_img->pixel_count++;
        }
    }

    bytesPerRow = input_rgb_img->width / 2 * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }

    uint32_t datasize = bytesPerRow * input_rgb_img->height;
    printf("data size is %d\n", datasize);

    pixel_index = 0;
    for(y = 0; y < output_ycc_img->height/2; y++)
    {
        uint32_t x;
        for (x = 0; x < output_ycc_img->width/2; x++)
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

void get_image_info(FILE* file)
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

void print_pixel( uint32_t x, uint32_t y)
{
    uint32_t pixel_index = y * input_rgb_img->width + x;
    printf("Pixel %d (%d, %d) - R: %d, G: %d, B: %d\n", pixel_index, x, y, input_rgb_img->pixels[pixel_index].R,
           input_rgb_img->pixels[pixel_index].G, input_rgb_img->pixels[pixel_index].B);
}

int main(int argc, char* argv[] )
{
    int32_t outputComponents = 0;

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

    FILE* luma_fp;
    FILE* cb_fp;
    FILE* cr_fp;

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
    FILE* out_fp = fopen(argv[1], "w+");
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
    get_image_info(in_fp);

    // read pixels into inputRGBImage pixels
    read_pixels(in_fp);

    // Write the headers of the output files
    write_header(out_fp, in_fp);
    if(outputComponents == 1)
    {
        write_header(luma_fp, in_fp);
        write_header(cr_fp, in_fp);
        write_header(cb_fp, in_fp);
        resize_file(cb_fp, input_rgb_img->width/2, input_rgb_img->height/2);
        resize_file(cr_fp, input_rgb_img->width/2, input_rgb_img->height/2);
    }

    output_ycc_img->pixel_count = 0;
    output_rgb_img->pixel_count = 0;
    output_ycc_img->offset = input_rgb_img->offset;
    output_rgb_img->offset = input_rgb_img->offset;
    output_ycc_img->width = input_rgb_img->width;
    output_rgb_img->width = input_rgb_img->width;
    output_ycc_img->height = input_rgb_img->height;
    output_rgb_img->height = input_rgb_img->height;
    output_ycc_img->pixels = malloc(sizeof(YCC_pixel_t) * output_ycc_img->height * output_ycc_img->width);
    if(output_ycc_img->pixels == NULL){
        printf("error - Output pix malloc failed\n"); exit(1);
    }
    output_rgb_img->pixels = malloc(sizeof(RGB_pixel_t) * output_rgb_img->height * output_rgb_img->width);
    if(output_rgb_img->pixels == NULL){
        printf("error - Output pix malloc failed\n"); exit(1);
    }


    // Calculate YCC values for OutputImage
    get_luma();
    get_chroma();
    downsample_chroma();

    // use downsampled YCC image to create new RGB image
    ycc_to_rgb();

    // write YCC values to RBG files
    if(outputComponents == 1)
    {
        write_ycc_components(luma_fp, cb_fp, cr_fp);
    }
    write_rgb(out_fp);

    printf("rgb pixels: %d\n", output_rgb_img->pixel_count);
    printf("ycc pixels: %d\n", output_ycc_img->pixel_count);


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

    printf("Luma component: /luma/%s\nCb component: /cb/%s\nCr component: /cr/%s\n", argv[1], argv[1], argv[1]);

    return 0;
}
