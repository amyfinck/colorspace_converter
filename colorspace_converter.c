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

RGB_image_t *inputImage;
YCC_image_t *outputImage;

void getLuma()
{
    for(uint32_t i = 0; i < inputImage->pixel_count; i++)
    {
        double R = (double)(inputImage->pixels[i].R);
        double G = (double)(inputImage->pixels[i].G);
        double B = (double)(inputImage->pixels[i].B);
        double Y = 0.299*R + 0.587*G + 0.114*B;

        outputImage->pixels[i].Y = (uint8_t)Y;
    }
}

void getChroma()
{
    for(uint32_t i = 0; i < inputImage->pixel_count; i++)
    {
        outputImage->pixels[i].Cb = inputImage->pixels[i].B - outputImage->pixels[i].Y;
        outputImage->pixels[i].Cr = inputImage->pixels[i].R - outputImage->pixels[i].Y;
    }
}

void print_pixel( uint32_t x, uint32_t y)
{
    uint32_t pixel_index = y * inputImage->width + x;
    printf("Pixel %d (%d, %d) - R: %d, G: %d, B: %d\n", pixel_index, x, y, inputImage->pixels[pixel_index].R, inputImage->pixels[pixel_index].G, inputImage->pixels[pixel_index].B);
}

int main( int argc, char* argv[] )
{
    // TODO clear some stuff out of main
    // TODO check bitmap format

    if(argc != 2)
    {
        printf("Not a valid command. Usage: ./colorspace_converter <img_name>.bmp\n");
        exit(1);
    }

    // open input image
    int in_fd;
    in_fd = open(argv[1], O_RDWR);
    if (in_fd == -1)
    {
        close(in_fd);
        printf("input File error\n");
        exit(1);
    }

    // fstat gets the file status, and puts all the relevant information in sb
    struct stat statbuf;
    fstat(in_fd, &statbuf);

    // mmap the file to pointer p
    char * p = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, in_fd, 0);
    if (fstat(in_fd, &statbuf) == -1 | p == MAP_FAILED)
    {
        printf("Error: failed to map memory\n");
        close(in_fd);
        exit(1);
    }
    size_t file_size = statbuf.st_size;

    printf("file size: %ld\n", file_size);

    char buffer[file_size];
    ssize_t bytesRead, bytesWritten;

    inputImage = malloc(sizeof(RGB_image_t));

    memcpy(&inputImage->offset, (p + 10), 4);
    memcpy(&inputImage->width, (p + 18), 4);
    memcpy(&inputImage->height, (p + 22), 4);

    inputImage->pixel_count = 0;
    if(inputImage == NULL)
    {
        printf("error - malloc failed\n");
        close(in_fd);
        munmap(p, statbuf.st_size);
        exit(1);
    }

    inputImage->pixels = malloc(sizeof(RGB_pixel_t) * inputImage->height * inputImage->width);
    if(inputImage->pixels == NULL)
    {
        printf("error - malloc failed\n");
        close(in_fd);
        munmap(p, statbuf.st_size);
        exit(1);
    }

    // TODO check header is valid
    uint32_t bytesPerRow = inputImage->width * 3; // there are 3 bytes per pixel
    uint32_t padding = 0;
    if (bytesPerRow % 4 != 0)
    {
        padding = 4 - (bytesPerRow % 4);
    }
    bytesPerRow += padding;
    inputImage->pixel_count = 0;

    uint32_t pixel_offset = 0;
    uint32_t pixel_index = 0;
    for(uint32_t y = 0; y < inputImage->height; y++)
    {
        for (uint32_t x = 0; x < inputImage->width; x++)
        {
            pixel_offset = (y * bytesPerRow) + (x * 3);

            // Pixels are stored in BGR order
            memcpy(&inputImage->pixels[pixel_index].B, (p + inputImage->offset + pixel_offset ), sizeof(uint8_t));
            memcpy(&inputImage->pixels[pixel_index].G, (p + inputImage->offset + pixel_offset + 1), sizeof(uint8_t));
            memcpy(&inputImage->pixels[pixel_index].R, (p + inputImage->offset + pixel_offset + 2), sizeof(uint8_t));

            pixel_index++;
            inputImage->pixel_count++;
        }
    }
    munmap(p, statbuf.st_size);

    int luma_fd;
    int cb_fd;
    int cr_fd;

    chdir("luma");
    luma_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (luma_fd == -1)
    {
        close(luma_fd);
        printf("output File error\n");
        exit(1);
    }
    chdir("..");
    chdir("cb");
    cb_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (cb_fd == -1)
    {
        close(cb_fd);
        printf("output File error\n");
        exit(1);
    }
    chdir("..");
    chdir("cr");
    cr_fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if (cr_fd == -1)
    {
        close(cb_fd);
        printf("output File error\n");
        exit(1);
    }
    chdir("..");

    while ((bytesRead = read(in_fd, buffer, file_size)) > 0) {
        bytesWritten = write(luma_fd, buffer, bytesRead);
        write(cb_fd, buffer, bytesRead);
        write(cr_fd, buffer, bytesRead);
        if (bytesWritten == -1) {
            printf("Error writing to output file.\n");
            close(in_fd);
            close(in_fd);
            return 1;
        }
    }

    outputImage = malloc(sizeof(YCC_image_t));
    outputImage->pixel_count = 0;
    if(outputImage == NULL)
    {
        printf("error - malloc failed\n");
        close(in_fd);
        munmap(p, statbuf.st_size);
        exit(1);
    }

    outputImage->offset = inputImage->offset;
    outputImage->width = inputImage->width;
    outputImage->height = inputImage->height;

    outputImage->pixels = malloc(sizeof(YCC_pixel_t) * outputImage->height * outputImage->width);
    if(outputImage->pixels == NULL)
    {
        printf("error - malloc failed\n");
        close(in_fd);
        close(luma_fd);
        munmap(p, statbuf.st_size);
        exit(1);
    }

    getLuma();
    getChroma();

    char * p_l = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, luma_fd, 0);
    char * p_cb = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, cb_fd, 0);
    char * p_cr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, cr_fd, 0);

    pixel_offset = 0;
    pixel_index = 0;

    for(uint32_t y = 0; y < outputImage->height; y++)
    {
        for (uint32_t x = 0; x < outputImage->width; x++)
        {
            pixel_offset = (y * bytesPerRow) + (x * 3);

            // Pixels are stored in BGR order
            memcpy((p_l + outputImage->offset + pixel_offset), &outputImage->pixels[pixel_index].Y, sizeof(uint8_t));
            memcpy((p_l + outputImage->offset + pixel_offset + 1), &outputImage->pixels[pixel_index].Y, sizeof(uint8_t));
            memcpy((p_l + outputImage->offset + pixel_offset + 2), &outputImage->pixels[pixel_index].Y, sizeof(uint8_t));

            uint8_t baseColor = 128;

            memcpy((p_cb + outputImage->offset + pixel_offset), &outputImage->pixels[pixel_index].Cb, sizeof(uint8_t));
            memcpy((p_cb + outputImage->offset + pixel_offset + 1), &baseColor, sizeof(uint8_t));
            memcpy((p_cb + outputImage->offset + pixel_offset + 2), &baseColor, sizeof(uint8_t));

            memcpy((p_cr + outputImage->offset + pixel_offset), &baseColor, sizeof(uint8_t));
            memcpy((p_cr + outputImage->offset + pixel_offset + 1), &baseColor, sizeof(uint8_t));
            memcpy((p_cr + outputImage->offset + pixel_offset + 2), &outputImage->pixels[pixel_index].Cr, sizeof(uint8_t));

            pixel_index++;
            inputImage->pixel_count++;
        }
    }

    munmap(p_l, statbuf.st_size);
    msync(p_l, statbuf.st_size, MS_SYNC);

    close(in_fd);
    close(luma_fd);
    close(cb_fd);
    close(cr_fd);

    return 0;

}

