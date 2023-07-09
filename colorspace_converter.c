#include "colorspace_converter.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/mman.h>
#include<string.h>

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
    uint32_t padding;
}RGB_image_t;

typedef struct YCC_pixel_t
{
    uint8_t Y; // luma
    int8_t Cb; // blue-difference
    int8_t Cr; // red-difference
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

RGB_image_t *inputRGBImage;
YCC_image_t *outputYCCImage;
RGB_image_t *outputRGBImage;

void getLuma()
{
    for(uint32_t i = 0; i < inputRGBImage->pixel_count; i++)
    {
        double R = (double)(inputRGBImage->pixels[i].R);
        double G = (double)(inputRGBImage->pixels[i].G);
        double B = (double)(inputRGBImage->pixels[i].B);
        double Y = 0.299*R + 0.587*G + 0.114*B;

        outputYCCImage->pixels[i].Y = (uint8_t)Y;
    }
}

void getChroma()
{
    for(uint32_t i = 0; i < inputRGBImage->pixel_count; i++)
    {
        // Cb and Cr are signed from -128 to 127
        int32_t Cb = inputRGBImage->pixels[i].B - outputYCCImage->pixels[i].Y;
        int32_t Cr = inputRGBImage->pixels[i].R - outputYCCImage->pixels[i].Y;
        if(Cb > 127) Cb = 127;
        if(Cb < -128) Cb = -128;
        if(Cr > 127) Cr = 127;
        if(Cr < -128) Cr = -128;

        outputYCCImage->pixels[i].Cb = (int8_t)Cb;
        outputYCCImage->pixels[i].Cr = (int8_t)Cr;
    }
}

void downsampleChroma()
{
    // TODO this should probably be a scaled down image
    for(uint32_t y = 0; y < inputRGBImage->height; y+=2)
    {
        for(uint32_t x = 0; x < inputRGBImage->width; x+=2)
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

void YCCToRGB()
{
    printf("pixel count: %d\n", inputRGBImage->pixel_count);
    for(uint32_t i = 0; i < inputRGBImage->pixel_count; i++)
    {
        double Y = (double)(outputYCCImage->pixels[i].Y);
        double Cb = (double)(outputYCCImage->pixels[i].Cb);
        double Cr = (double)(outputYCCImage->pixels[i].Cr);

        double R = Y + 1.402 * Cr;
        double G = Y - 0.344136 * Cb - 0.714136 * Cr;
        double B = Y + 1.772 * Cb;

        if(R > 255) R = 255;
        if(R < 0) R = 0;
        if(G > 255) G = 255;
        if(G < 0) G = 0;
        if(B > 255) B = 255;
        if(B < 0) B = 0;

        outputRGBImage->pixels[i].R = (uint8_t)R;
        outputRGBImage->pixels[i].G = (uint8_t)G;
        outputRGBImage->pixels[i].B = (uint8_t)B;
    }
}

void print_pixel( uint32_t x, uint32_t y)
{
    uint32_t pixel_index = y * inputRGBImage->width + x;
    printf("Pixel %d (%d, %d) - R: %d, G: %d, B: %d\n", pixel_index, x, y, inputRGBImage->pixels[pixel_index].R,
           inputRGBImage->pixels[pixel_index].G, inputRGBImage->pixels[pixel_index].B);
}

int main(int argc, char* argv[] )
{
    // TODO clear some stuff out of main
    // TODO check bitmap format
    if(argc != 2)
    {
        printf("Not a valid command. Usage: ./colorspace_converter <img_name>.bmp\n"); exit(1);
    }

    // open input image
    int in_fd;
    in_fd = open(argv[1], O_RDWR);
    if (in_fd == -1)
    {
        close(in_fd); printf("Could not open %s\n", argv[1]); exit(1);
    }
    struct stat sb;
    fstat(in_fd, &sb);

    // map contents of file to memory pointer p
    char * p = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, in_fd, 0);
    if (fstat(in_fd, &sb) == -1 | p == MAP_FAILED)
    {
        printf("Error: failed to map memory\n"); munmap(p, sb.st_size); close(in_fd); exit(1);
    }
    size_t file_size = sb.st_size;

    // create input image struct and fill in info from header
    inputRGBImage = malloc(sizeof(RGB_image_t));
    if(inputRGBImage == NULL)
    {
        printf("error - malloc failed\n"); close(in_fd); munmap(p, sb.st_size); exit(1);
    }
    memcpy(&inputRGBImage->offset, (p + 10), 4);
    memcpy(&inputRGBImage->width, (p + 18), 4);
    memcpy(&inputRGBImage->height, (p + 22), 4);
    inputRGBImage->pixel_count = 0;
    inputRGBImage->pixels = malloc(sizeof(RGB_pixel_t) * inputRGBImage->height * inputRGBImage->width);
    if(inputRGBImage->pixels == NULL)
    {
        printf("error - malloc failed\n"); close(in_fd); munmap(p, sb.st_size); exit(1);
    }

    // calculate padding
    uint32_t bytesPerRow = inputRGBImage->width * 3; // 3 bytes per pixel
    uint32_t padding = 0;
    if (bytesPerRow % 4 != 0)
    {
        padding = 4 - (bytesPerRow % 4);
        bytesPerRow += padding;
    }

    // read in pixels
    uint32_t pixel_offset = 0;
    uint32_t pixel_index = 0;
    for(uint32_t y = 0; y < inputRGBImage->height; y++)
    {
        for (uint32_t x = 0; x < inputRGBImage->width; x++)
        {
            pixel_offset = (y * bytesPerRow) + (x * 3);

            // Pixels are stored in BGR order
            memcpy(&inputRGBImage->pixels[pixel_index].B, (p + inputRGBImage->offset + pixel_offset ), sizeof(uint8_t));
            memcpy(&inputRGBImage->pixels[pixel_index].G, (p + inputRGBImage->offset + pixel_offset + 1), sizeof(uint8_t));
            memcpy(&inputRGBImage->pixels[pixel_index].R, (p + inputRGBImage->offset + pixel_offset + 2), sizeof(uint8_t));

            pixel_index++;
            inputRGBImage->pixel_count++;
        }
    }

    // create a file for all 4 output types
    int luma_fd;
    int cb_fd;
    int cr_fd;
    int o_fd;
    chdir("luma");
    luma_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (luma_fd == -1)
    {
        close(in_fd); close(luma_fd); munmap(p, sb.st_size); printf("error creating luma/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("cb");
    cb_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (cb_fd == -1)
    {
        close(in_fd); close(luma_fd); close(cb_fd) ;munmap(p, sb.st_size); printf("error creating cb/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("cr");
    cr_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (cr_fd == -1)
    {
        close(in_fd); close(luma_fd); close(cb_fd); close(cr_fd); munmap(p, sb.st_size); printf("error creating cr/%s\n", argv[1]); exit(1);
    }
    chdir(".."); chdir("output");
    o_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (o_fd == -1)
    {
        close(in_fd); close(luma_fd); close(cb_fd); close(cr_fd); close(o_fd); munmap(p, sb.st_size); printf("error creating output/%s\n", argv[1]); exit(1);
    }
    chdir("..");


    // copy input file to output locations
    char buffer[file_size];
    ssize_t bytesRead, bytesWritten;
    while ((bytesRead = read(in_fd, buffer, file_size)) > 0) {
        bytesWritten = write(luma_fd, buffer, bytesRead);
        write(cb_fd, buffer, bytesRead);
        write(cr_fd, buffer, bytesRead);
        write(o_fd, buffer, bytesRead);
        if (bytesWritten == -1) {
            printf("Error writing to output file.\n");
            close(in_fd);
            return 1;
        }
    }

    // create output image struct and fill in info
    outputYCCImage = malloc(sizeof(YCC_image_t));
    outputRGBImage = malloc(sizeof(RGB_image_t));
    if(outputYCCImage == NULL)
    {
        printf("Error - Output malloc failed\n"); close(in_fd); close(luma_fd); close(cb_fd); close(cr_fd); munmap(p, sb.st_size); exit(1);
    }
    outputYCCImage->pixel_count = 0;
    outputRGBImage->pixel_count = 0;
    outputYCCImage->offset = inputRGBImage->offset;
    outputRGBImage->offset = inputRGBImage->offset;
    outputYCCImage->width = inputRGBImage->width;
    outputRGBImage->width = inputRGBImage->width;
    outputYCCImage->height = inputRGBImage->height;
    outputRGBImage->height = inputRGBImage->height;
    outputYCCImage->pixels = malloc(sizeof(YCC_pixel_t) * outputYCCImage->height * outputYCCImage->width);
    if(outputYCCImage->pixels == NULL)
    {
        printf("error - Output pix malloc failed\n"); close(in_fd); close(luma_fd); close(cb_fd); close(cr_fd); munmap(p, sb.st_size); exit(1);
    }
    outputRGBImage->pixels = malloc(sizeof(RGB_pixel_t) * outputRGBImage->height * outputRGBImage->width);
    if(outputRGBImage->pixels == NULL)
    {
        printf("error - Output pix malloc failed\n"); close(in_fd); close(luma_fd); close(cb_fd); close(cr_fd); munmap(p, sb.st_size); exit(1);
    }

    // Calculate YCC values for OutputImage
    getLuma();
    getChroma();
    downsampleChroma();

    // use downsampled YCC image to create new RGB image
    YCCToRGB();

    // create mappings for output files
    char * p_l = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, luma_fd, 0);
    char * p_cb = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, cb_fd, 0);
    char * p_cr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, cr_fd, 0);
    char * p_o = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, o_fd, 0);

    // write YCC values to output files
    pixel_offset = 0;
    pixel_index = 0;
    for(uint32_t y = 0; y < outputYCCImage->height; y++)
    {
        for (uint32_t x = 0; x < outputYCCImage->width; x++)
        {
            // Pixels are stored in BGR order
            pixel_offset = (y * bytesPerRow) + (x * 3);

            // Write luma output file (grayscale)
            memcpy((p_l + outputYCCImage->offset + pixel_offset), &outputYCCImage->pixels[pixel_index].Y, sizeof(uint8_t));
            memcpy((p_l + outputYCCImage->offset + pixel_offset + 1), &outputYCCImage->pixels[pixel_index].Y, sizeof(uint8_t));
            memcpy((p_l + outputYCCImage->offset + pixel_offset + 2), &outputYCCImage->pixels[pixel_index].Y, sizeof(uint8_t));

            // Convert Cb and Cr to unsigned for use in RGB representation
            uint8_t baseColor = 128;
            uint8_t Cb = outputYCCImage->pixels[pixel_index].Cb + 128;
            uint8_t Cr = outputYCCImage->pixels[pixel_index].Cr + 128;

            // Write blue difference output file
            memcpy((p_cb + outputYCCImage->offset + pixel_offset), &Cb, sizeof(uint8_t));
            memcpy((p_cb + outputYCCImage->offset + pixel_offset + 1), &baseColor, sizeof(uint8_t));
            memcpy((p_cb + outputYCCImage->offset + pixel_offset + 2), &baseColor, sizeof(uint8_t));

            // Write red difference output file
            memcpy((p_cr + outputYCCImage->offset + pixel_offset), &baseColor, sizeof(uint8_t));
            memcpy((p_cr + outputYCCImage->offset + pixel_offset + 1), &baseColor, sizeof(uint8_t));
            memcpy((p_cr + outputYCCImage->offset + pixel_offset + 2), &Cr, sizeof(uint8_t));

            // Write output file
            memcpy((p_o + outputRGBImage->offset + pixel_offset), &outputRGBImage->pixels[pixel_index].B, sizeof(uint8_t));
            memcpy((p_o + outputRGBImage->offset + pixel_offset + 1), &outputRGBImage->pixels[pixel_index].G, sizeof(uint8_t));
            memcpy((p_o + outputRGBImage->offset + pixel_offset + 2), &outputRGBImage->pixels[pixel_index].R, sizeof(uint8_t));

            pixel_index++;
            outputYCCImage->pixel_count++;
        }
    }

    // free memory
    free(inputRGBImage->pixels);
    free(inputRGBImage);
    free(outputYCCImage->pixels);
    free(outputYCCImage);
    free(outputRGBImage->pixels);
    free(outputRGBImage);

    // unmap
    munmap(p, sb.st_size);
    munmap(p_l, sb.st_size);
    munmap(p_cb, sb.st_size);
    munmap(p_cr, sb.st_size);
    munmap(p_o, sb.st_size);

    // close all files
    close(in_fd);
    close(luma_fd);
    close(cb_fd);
    close(cr_fd);
    close(o_fd);

    printf("Luma component: /luma/%s\nCb component: /cb/%s\nCr component: /cr/%s\n", argv[1], argv[1], argv[1]);

    return 0;
}