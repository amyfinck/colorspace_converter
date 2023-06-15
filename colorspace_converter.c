#include "colorspace_converter.h"

#include <stdio.h>
#include <stdlib.h>

// struct for BMP header info
typedef struct BMP_header_t
{
    // TODO fill with information contained in a BMP header
    int height;
    int width;
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
    // TODO extract all the information and pass back a pointer to the filled in header
    return header;
}

int main( int argc, char* argv[] )
{
    if(argc != 2)
    {
        // TODO add usage statement
        printf("Not a valid command\n");
        exit(1);
    }
    // TODO check bitmap format

    // open input image
    FILE * input;
    input = fopen(argv[1], "r");
    if (input==NULL)
    {
        printf("Input File error");
        exit(1);
    }

    // get struct for header of the image
    BMP_header_t* BMPHeader = malloc(sizeof(BMP_header_t));
    BMPHeader = getHeaderBMP(input, BMPHeader);
    // TODO maybe check header is valid

    // TODO determine whether image is RGB or YCC so we can work both ways. For now assume start with RGB.

    // TODO get array of pixels, and convert to correct colorspace
}

