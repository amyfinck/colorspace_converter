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
    // TODO fill with information contained in a BMP header
    uint32_t height;
    uint32_t width;
}BMP_header_t;

typedef struct RGB_pixel_t
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_pixel_t;

typedef struct YCC_pixel_t
{
    uint8_t Y; // luma
    uint8_t Cb; // blue-difference
    uint8_t Cr; // red-difference
}YCC_pixel_t;


BMP_header_t* getHeaderBMP(FILE* fp, BMP_header_t* header)
{
    return header;
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
        printf("Input File error");
        exit(1);
    }

    // fstat gets the file status, and puts all the relevant information in sb
    struct stat statbuf;
    fstat(file_descriptor, &statbuf);

    // mmap the file to pointer p
    char * p = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (p == MAP_FAILED)
    {
        printf("Error: failed to map memory\n");
        close(file_descriptor);
        exit(1);
    }

    BMP_header_t* BMPHeader = malloc(sizeof(BMP_header_t));
    if(BMPHeader == NULL)
    {
        printf("error - malloc failed\n");
        close(file_descriptor);
        munmap(p, statbuf.st_size);
        exit(1);
    }

    memcpy(&BMPHeader->width, (p + 18), 4);
    memcpy(&BMPHeader->height, (p + 22), 4);

    printf("I received a picture with a height of %d and a width of %d\n", BMPHeader->height, BMPHeader->width);

    // todo make this a function call
   // BMPHeader = getHeaderBMP(file_descriptor, BMPHeader);
    // TODO maybe check header is valid
    // TODO determine whether image is RGB or YCC so we can work both ways. For now assume start with RGB.
    // TODO get array of pixels, and convert to correct colorspace
}

