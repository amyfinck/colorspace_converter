#include "colorspace_converter.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/mman.h>
#include<string.h>

// struct for BMP header info
typedef struct BMP_header_t
{
    uint32_t height;
    uint32_t width;
    uint32_t offset;
}BMP_header_t;

typedef struct RGB_pixel_t
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_pixel_t;

typedef struct RGB_image_t
{
    RGB_pixel_t *pixels;
    BMP_header_t header;
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
    YCC_pixel_t *pixels;
    BMP_header_t header;
}YCC_image_t;

RGB_image_t *inputImage;

BMP_header_t* getHeaderBMP(FILE* fp, BMP_header_t* header)
{
    // TODO implement here instead of in main
    return header;
}

void print_pixel( uint32_t x, uint32_t y)
{
    uint32_t pixel_index = y * inputImage->header.width + x;
    printf("Pixel %d (%d, %d) - R: %d, G: %d, B: %d\n", pixel_index, x, y, inputImage->pixels[pixel_index].R, inputImage->pixels[pixel_index].G, inputImage->pixels[pixel_index].B);
}

int main( int argc, char* argv[] )
{
    if(argc != 2)
    {
        printf("Not a valid command. Usage: ./colorspace_converter <img_name>.bmp\n");
        exit(1);
    }

    // TODO check bitmap format

    // open input image
    int file_descriptor;
    file_descriptor = open(argv[1], O_RDWR);
    if (file_descriptor == -1)
    {
        close(file_descriptor);
        printf("Input File error\n");
        exit(1);
    }

    // fstat gets the file status, and puts all the relevant information in sb
    struct stat statbuf;
    fstat(file_descriptor, &statbuf);

    // mmap the file to pointer p
    char * p = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    struct stat sb;
    if (p == MAP_FAILED)
    {
        printf("Error: failed to map memory\n");
        close(file_descriptor);
        exit(1);
    }
    if (fstat(file_descriptor, &sb) == -1) {
        perror("Error getting file information");
        close(file_descriptor);
        return 1;
    }
    size_t file_size = sb.st_size;


    BMP_header_t* BMPHeader = malloc(sizeof(BMP_header_t));
    if(BMPHeader == NULL)
    {
        printf("error - malloc failed\n");
        close(file_descriptor);
        munmap(p, statbuf.st_size);
        exit(1);
    }

    memcpy(&BMPHeader->offset, (p + 10), 4);
    memcpy(&BMPHeader->width, (p + 18), 4);
    memcpy(&BMPHeader->height, (p + 22), 4);

    inputImage = malloc(sizeof(RGB_image_t));
    inputImage->pixel_count = 0;
    if(inputImage == NULL)
    {
        printf("error - malloc failed\n");
        close(file_descriptor);
        munmap(p, statbuf.st_size);
        exit(1);
    }
    inputImage->header = *BMPHeader;

    inputImage->pixels = malloc(sizeof(RGB_pixel_t) * BMPHeader->height * BMPHeader->width);
    if(inputImage->pixels == NULL)
    {
        printf("error - malloc failed\n");
        close(file_descriptor);
        munmap(p, statbuf.st_size);
        exit(1);
    }

    printf("Header size is %d\n", BMPHeader->offset);

    uint32_t bytesPerRow = inputImage->header.width * 3; // there are 3 bytes per pixel
    if (bytesPerRow % 4 != 0)
    {
        inputImage->padding = 4 - (bytesPerRow % 4);
    }
    else
    {
        inputImage->padding = 0;
    }

    uint32_t bytesPerIteration = inputImage->padding + inputImage->padding;
    int totalBytes = inputImage->header.width * bytesPerIteration;
    inputImage->pixel_count = 0;

    for(int i = 0; i < (inputImage->header.width + inputImage->padding) * inputImage->header.height; i++)
    {

            // TODO should this be a 2D array?
            memcpy(&inputImage->pixels[i].R, (p + BMPHeader->offset + (i * 3)), sizeof(uint8_t));
            memcpy(&inputImage->pixels[i].G, (p + BMPHeader->offset + (i * 3) + 1), sizeof(uint8_t));
            memcpy(&inputImage->pixels[i].B, (p + BMPHeader->offset + (i * 3) + 2), sizeof(uint8_t));
            inputImage->pixel_count++;

    }

    printf("I received a picture with a height of %d and a width of %d\n", BMPHeader->height, BMPHeader->width);
    printf("There are %d pixels total\n", inputImage->pixel_count);

    printf("The file size is %d, header size is %d, so there are %d pixels\n", (int)file_size, inputImage->header.offset, (int)file_size - inputImage->header.offset);

    /* To get the pixel number from the x and y coordinates, use the formula:
     * pixel_number = y * width + x
     * To get the x and y coordinates from the pixel number, use the formula:
     * x = pixel_number % width
     * y = pixel_number / width
     */

    int i, j;
    for(i = 0; i < inputImage->header.height; i++)
    {
        for(j = 0; j < inputImage->header.width; j++)
        {
            print_pixel(j, i);
        }
    }

    /*
     * 0 - 127 is green
     * 128 - 255 is black
     * 256 - 383 is green
     * 384 - 511 is black
     * ect...
     *
     * 65408 is red
     * 65535 is blue
     */

    // todo make this a function call
    // BMPHeader = getHeaderBMP(file_descriptor, BMPHeader);
    // TODO maybe check header is valid
    // TODO determine whether image is RGB or YCC so we can work both ways. For now assume start with RGB.
    // TODO get array of pixels, and convert to correct colorspace
}

