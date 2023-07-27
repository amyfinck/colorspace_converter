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

FILE *in_fp;
FILE *luma_fp;
FILE *cb_fp;
FILE *cr_fp;
FILE *out_fp;

RGB_image_t *inputRGBImage;
YCC_image_t *outputYCCImage;
RGB_image_t *outputRGBImage;

void error_teardown()
{
    fclose(in_fp);
    fclose(luma_fp);
    fclose(cb_fp);
    fclose(cr_fp);
    fclose(out_fp);

    free(inputRGBImage);
    free(inputRGBImage->pixels);
    free(outputYCCImage);
    free(outputYCCImage->pixels);
    free(outputRGBImage);
    free(outputRGBImage->pixels);
}

/*
 * y = 16.0 + 0.257 R + 0.504 G + 0.098 B
 * 2^8 * 0.257 = 66
 * 2^8 * 0.504 = 129
 * 2^8 * 0.098 = 25
 */
void getLuma()
{
    uint8_t K = 8;
    uint32_t i;
    for(i = 0; i < inputRGBImage->pixel_count; i++)
    {
        uint8_t R = inputRGBImage->pixels[i].R;
        uint8_t G = inputRGBImage->pixels[i].G;
        uint8_t B = inputRGBImage->pixels[i].B;
        uint32_t Y_temp = (16 << K) + (66 * R) + (129 * G) + (25 * B);
        uint8_t Y = (uint8_t)(( Y_temp + (1 << (K-1))) >> K);

        outputYCCImage->pixels[i].Y = Y;
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
void getChroma()
{
    uint32_t i;
    for(i = 0; i < inputRGBImage->pixel_count; i++)
    {
        uint8_t K = 8;

        // Cb and Cr are signed from -128 to 127
        uint8_t R = inputRGBImage->pixels[i].R;
        uint8_t G = inputRGBImage->pixels[i].G;
        uint8_t B = inputRGBImage->pixels[i].B;

        uint32_t Cb_temp = (128 << K) - (38 * R) - (74 * G) + (112 * B);
        uint32_t Cr_temp = (128 << K) + (112 * R) - (94 * G) - (18 * B);

        uint8_t Cb = (uint8_t)(( Cb_temp + (1 << (K -1))) >> K);
        uint8_t Cr = (uint8_t)(( Cr_temp + (1 << (K -1))) >> K);

        if(Cb < 0) Cb = 0;
        if(Cb > 255) Cb = 255;

        outputYCCImage->pixels[i].Cb = (uint8_t)Cb;
        outputYCCImage->pixels[i].Cr = (uint8_t)Cr;
    }
}

void downsampleChroma()
{
    // TODO this should probably be a scaled down image
    uint32_t y;
    for(y = 0; y < inputRGBImage->height; y+=2)
    {
        uint32_t x;
        for(x = 0; x < inputRGBImage->width; x+=2)
        {
            uint32_t bottom_left = y * inputRGBImage->width + x;
            uint32_t bottom_right = bottom_left + 1;
            uint32_t top_left = bottom_left + inputRGBImage->width;
            uint32_t top_right = top_left + 1;

            int32_t Cb_total = (int32_t)outputYCCImage->pixels[bottom_left].Cb + (int32_t)outputYCCImage->pixels[bottom_right].Cb + (int32_t)outputYCCImage->pixels[top_left].Cb + (int32_t)outputYCCImage->pixels[top_right].Cb;
            int8_t Cb_avg = (int8_t)(Cb_total / 4);
            int32_t Cr_total = (int32_t)outputYCCImage->pixels[bottom_left].Cr + (int32_t)outputYCCImage->pixels[bottom_right].Cr + (int32_t)outputYCCImage->pixels[top_left].Cr + (int32_t)outputYCCImage->pixels[top_right].Cr;
            int8_t Cr_avg = (int8_t)(Cr_total / 4);

            outputYCCImage->pixels[bottom_left].Cb = Cb_avg;
            outputYCCImage->pixels[bottom_right].Cb = Cb_avg;
            outputYCCImage->pixels[top_left].Cb = Cb_avg;
            outputYCCImage->pixels[top_right].Cb = Cb_avg;

            outputYCCImage->pixels[bottom_left].Cr = Cr_avg;
            outputYCCImage->pixels[bottom_right].Cr = Cr_avg;
            outputYCCImage->pixels[top_left].Cr = Cr_avg;
            outputYCCImage->pixels[top_right].Cr = Cr_avg;
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
void YCCToRGB()
{
    uint32_t i;
    for(i = 0; i < inputRGBImage->pixel_count; i++)
    {
        uint8_t K = 6;

        uint8_t Y = outputYCCImage->pixels[i].Y;
        uint8_t Cb = outputYCCImage->pixels[i].Cb;
        uint8_t Cr = outputYCCImage->pixels[i].Cr;

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

        outputRGBImage->pixels[i].R = (uint8_t)R;
        outputRGBImage->pixels[i].G = (uint8_t)G;
        outputRGBImage->pixels[i].B = (uint8_t)B;
    }
}

void read_pixels()
{
    // allocate memory for pixels
    inputRGBImage->pixel_count = 0;
    inputRGBImage->pixels = malloc(sizeof(RGB_pixel_t) * inputRGBImage->height * inputRGBImage->width);
    if(inputRGBImage->pixels == NULL) {
        printf("error - pixel malloc failed\n"); fclose(in_fp); exit(1);
    }

    // calculate padding
    uint32_t bytesPerRow = inputRGBImage->width * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }
    inputRGBImage->data_size = bytesPerRow * inputRGBImage->height;
    printf("data size is %d\n", inputRGBImage->data_size);

    uint32_t pixel_offset;
    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < inputRGBImage->height; y++)
    {
        uint32_t x;
        for(x = 0; x < inputRGBImage->width; x++)
        {
            pixel_offset = (y * bytesPerRow) + (x * 3);

            // Pixels are stored in BGR order
            if (fseek(in_fp, (long)inputRGBImage->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); fclose(in_fp); exit(1);
            }
            fread(&inputRGBImage->pixels[pixel_index].B, 1, 1, in_fp);
            fread(&inputRGBImage->pixels[pixel_index].G, 1, 1, in_fp);
            fread(&inputRGBImage->pixels[pixel_index].R, 1, 1, in_fp);

            pixel_index++;
            inputRGBImage->pixel_count++;
        }
    }
}

void resize_file(FILE* file, uint32_t width, uint32_t height)
{
    if (fseek(file, 18, SEEK_SET) != 0){
        printf("Error seeking to width position\n"); error_teardown(); exit(1);
    }
    fwrite(&width, 4, 1, file);
    if (fseek(file, 22, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); error_teardown(); exit(1);
    }
    fwrite(&height, 4, 1, file);
}


/*
 *
 */
void write_headers()
{
    // copy input file to RBG locations
    uint32_t header_length = inputRGBImage->offset;
    char buffer[header_length];
    size_t bytesRead;
    size_t totalBytesRead = 0;
    fseek(in_fp, 0, SEEK_SET);
    while (totalBytesRead < header_length && (bytesRead = fread(buffer, 1, header_length - totalBytesRead, in_fp)) > 0)
    {
        if( fwrite(buffer, 1, bytesRead, luma_fp) == -1) {
            printf("Error writing to luma file\n"); error_teardown(); exit(1);
        }
        if( fwrite(buffer, 1, bytesRead, cb_fp) == -1) {
            printf("Error writing to cb file\n"); error_teardown(); exit(1);
        }
        if( fwrite(buffer, 1, bytesRead,cr_fp) == -1) {
            printf("Error writing to cr file\n"); error_teardown(); exit(1);
        }
        if(fwrite(buffer, 1, bytesRead, out_fp) == -1) {
            printf("Error writing to output file\n"); error_teardown(); exit(1);
        }
        totalBytesRead += bytesRead;
    }
    resize_file(cb_fp, inputRGBImage->width/2, inputRGBImage->height/2);
    resize_file(cr_fp, inputRGBImage->width/2, inputRGBImage->height/2);
}

void write_YCC_components()
{
    // calculate padding
    uint32_t bytesPerRow = inputRGBImage->width * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }

    uint32_t pixel_index = 0;
    uint32_t y;
    for(y = 0; y < outputYCCImage->height; y++)
    {
        uint32_t x;
        for (x = 0; x < outputYCCImage->width; x++)
        {
            // Pixels are stored in BGR order
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Write luma RBG file (grayscale)
            if (fseek(luma_fp, inputRGBImage->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); error_teardown(); exit(1);
            }
            fwrite(&outputYCCImage->pixels[pixel_index].Y, 1, 1, luma_fp);
            fwrite(&outputYCCImage->pixels[pixel_index].Y, 1, 1, luma_fp);
            fwrite(&outputYCCImage->pixels[pixel_index].Y, 1, 1, luma_fp);

            // Write RBG file
            if (fseek(out_fp, inputRGBImage->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); error_teardown(); exit(1);
            }
            fwrite(&outputRGBImage->pixels[pixel_index].B, 1, 1, out_fp);
            fwrite(&outputRGBImage->pixels[pixel_index].G, 1, 1, out_fp);
            fwrite(&outputRGBImage->pixels[pixel_index].R, 1, 1, out_fp);

            pixel_index++;
            outputYCCImage->pixel_count++;
        }
    }

    bytesPerRow = inputRGBImage->width / 2 * 3; // 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        uint32_t padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }

    uint32_t datasize = bytesPerRow * inputRGBImage->height;
    printf("data size is %d\n", datasize);

    pixel_index = 0;
    for(y = 0; y < outputYCCImage->height/2; y++)
    {
        uint32_t x;
        for (x = 0; x < outputYCCImage->width/2; x++)
        {
            uint32_t pixel_offset = (y * bytesPerRow) + (x * 3);

            // Convert Cb and Cr to unsigned for use in RGB representation
            uint8_t baseColor = 128;
            if (fseek(cb_fp, inputRGBImage->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); error_teardown(); exit(1);
            }
            fwrite(&outputYCCImage->pixels[pixel_index].Cb, 1, 1, cb_fp);
            fwrite(&baseColor, 1, 1, cb_fp);
            fwrite(&baseColor, 1, 1, cb_fp);

            // Write red difference RBG file
            if (fseek(cr_fp, inputRGBImage->offset + pixel_offset, SEEK_SET) != 0) {
                printf("Error seeking to pixel\n"); error_teardown(); exit(1);
            }
            fwrite(&baseColor, 1, 1, cr_fp);
            fwrite(&baseColor, 1, 1, cr_fp);
            fwrite(&outputYCCImage->pixels[pixel_index].Cr, 1, 1, cr_fp);

            pixel_index+=2;
        }
        pixel_index += inputRGBImage->width;
    }
}

void get_image_info()
{
    // get file size
    fseek(in_fp, 0, SEEK_END);
    inputRGBImage->file_size = ftell(in_fp);

    // get offset, width, and height
    if (fseek(in_fp, 10, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); error_teardown(); exit(1);
    }
    fread(&inputRGBImage->offset, 4, 1, in_fp);
    if (fseek(in_fp, 18, SEEK_SET) != 0){
        printf("Error seeking to width position\n"); error_teardown(); exit(1);
    }
    printf("offset: %d\n", inputRGBImage->offset);
    fread(&inputRGBImage->width, 4, 1, in_fp);
    if (fseek(in_fp, 22, SEEK_SET) != 0) {
        printf("Error seeking to offset position\n"); error_teardown(); exit(1);
    }
    fread(&inputRGBImage->height, 4, 1, in_fp);
}

void print_pixel( uint32_t x, uint32_t y)
{
    uint32_t pixel_index = y * inputRGBImage->width + x;
    printf("Pixel %d (%d, %d) - R: %d, G: %d, B: %d\n", pixel_index, x, y, inputRGBImage->pixels[pixel_index].R,
           inputRGBImage->pixels[pixel_index].G, inputRGBImage->pixels[pixel_index].B);
}

int main(int argc, char* argv[] )
{
    if(argc != 2)
    {
        printf("Not a valid command. Usage: ./cc.exe <img_name>.bmp\n"); exit(1);
    }

    // open the input, luma, cb, cr, and output files
    chdir("input");
    in_fp = fopen(argv[1], "r");
    if (in_fp == NULL) {
        printf("Could not open %s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("output"); chdir("luma");
    luma_fp = fopen(argv[1], "w+");
    if (luma_fp == NULL) {
        fclose(in_fp); fclose(luma_fp); printf("error creating luma/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("cb");
    cb_fp = fopen(argv[1], "w+");
    if (cb_fp == NULL) {
        fclose(in_fp); fclose(luma_fp); fclose(cb_fp); printf("error creating cb/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("cr");
    cr_fp = fopen(argv[1], "w+");
    if (cr_fp == NULL) {
        fclose(in_fp); fclose(luma_fp); fclose(cb_fp); fclose(cr_fp); printf("error creating cr/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("rgb");
    out_fp = fopen(argv[1], "w+");
    if (out_fp == NULL) {
        fclose(in_fp); fclose(luma_fp); fclose(cb_fp); fclose(cr_fp); fclose(out_fp); printf("error creating RBG/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("..");

    // create image structs
    inputRGBImage = malloc(sizeof(RGB_image_t));
    if(inputRGBImage == NULL)
    {
        fclose(in_fp); fclose(luma_fp); fclose(cb_fp); fclose(cr_fp); fclose(out_fp); free(inputRGBImage); printf("error - input malloc failed\n"); exit(1);
    }
    outputYCCImage = malloc(sizeof(YCC_image_t));
    if(outputYCCImage == NULL)
    {
        fclose(in_fp); fclose(luma_fp); fclose(cb_fp); fclose(cr_fp); fclose(out_fp); free(inputRGBImage); free(outputYCCImage); printf("error - YCC output malloc failed\n"); exit(1);
    }
    outputRGBImage = malloc(sizeof(RGB_image_t));
    if(outputYCCImage == NULL)
    {
        error_teardown(); printf("error - RGB output malloc failed\n"); exit(1);
    }

    get_image_info();

    read_pixels();

    write_headers();

    outputYCCImage->pixel_count = 0;
    outputRGBImage->pixel_count = 0;
    outputYCCImage->offset = inputRGBImage->offset;
    outputRGBImage->offset = inputRGBImage->offset;
    outputYCCImage->width = inputRGBImage->width;
    outputRGBImage->width = inputRGBImage->width;
    outputYCCImage->height = inputRGBImage->height;
    outputRGBImage->height = inputRGBImage->height;
    outputYCCImage->pixels = malloc(sizeof(YCC_pixel_t) * outputYCCImage->height * outputYCCImage->width);
    if(outputYCCImage->pixels == NULL){
        printf("error - Output pix malloc failed\n"); fclose(in_fp); fclose(luma_fp); fclose(cb_fp); fclose(cr_fp); exit(1);
    }
    outputRGBImage->pixels = malloc(sizeof(RGB_pixel_t) * outputRGBImage->height * outputRGBImage->width);
    if(outputRGBImage->pixels == NULL){
        printf("error - Output pix malloc failed\n"); fclose(in_fp); fclose(luma_fp); fclose(cb_fp); fclose(cr_fp); exit(1);
    }

    // Calculate YCC values for OutputImage
    getLuma();
    getChroma();
    downsampleChroma();

    // use downsampled YCC image to create new RGB image
    YCCToRGB();

    // write YCC values to RBG files
    write_YCC_components();

    // free memory
    free(inputRGBImage->pixels);
    free(inputRGBImage);
    free(outputYCCImage->pixels);
    free(outputYCCImage);
    free(outputRGBImage->pixels);
    free(outputRGBImage);

    // close all files
    fclose(in_fp);
    fclose(luma_fp);
    fclose(cb_fp);
    fclose(cr_fp);
    fclose(out_fp);

    printf("Luma component: /luma/%s\nCb component: /cb/%s\nCr component: /cr/%s\n", argv[1], argv[1], argv[1]);

    return 0;
}
