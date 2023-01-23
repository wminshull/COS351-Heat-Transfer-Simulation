/*
 *  starter code
 *  create an image from data arriving on stdin
 *  single argument is size of (square) data and
 *  thus image:
 *  starterImg  500
 *  will read 500 data points at a time for 500 times,
 *  building a 500x500 image.
 */
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "readData.h"

int main(int argc, char **argv)
{
    int i, j;
    int width, height;
    unsigned char *img, *p;
    long ra_size;
    long img_dims;
    int channels = 3; // R G B
    int rowsize;
    unsigned char val;
    int *bufr;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s imageheight\n", argv[0]);
        return 2;
    }
    width = height = atoi(argv[1]); // this is a square image
    img_dims = width * height;      // # of pixels
    rowsize = width * channels;     // bytes per row (3 per pixel)
    ra_size = height * rowsize;     // total # of bytes needed
    img = malloc(ra_size);

    bufr = malloc((width + 1) * sizeof(int));

    for (p = img, i = 0; i <= height; i++)
    {

        readData(stdin, bufr, width);
        for (j = 0; j <= width; j++)
        {
            // TODO: make some interesting colors based on bufr[j]
            if (bufr[j] <= 15 && bufr[j] > 9)
            {
                *p = 216;       // R
                *(p + 1) = 243; // G
                *(p + 2) = 220; // B
                p += channels;
            }
            else if (bufr[j] <= 22 && bufr[j] > 15)
            {
                *p = 197;       // R
                *(p + 1) = 235; // G
                *(p + 2) = 210; // B
                p += channels;
            }
            else if (bufr[j] <= 30 && bufr[j] > 22)
            {
                *p = 183;       // R
                *(p + 1) = 228; // G
                *(p + 2) = 199; // B
                p += channels;
            }
            else if (bufr[j] <= 38 && bufr[j] > 30)
            {
                *p = 149;       // R
                *(p + 1) = 213; // G
                *(p + 2) = 178; // B
                p += channels;
            }
            else if (bufr[j] <= 46 && bufr[j] > 38)
            {
                *p = 116;       // R
                *(p + 1) = 198; // G
                *(p + 2) = 157; // B
                p += channels;
            }
            else if (bufr[j] <= 54 && bufr[j] > 46)
            {
                *p = 82;        // R
                *(p + 1) = 183; // G
                *(p + 2) = 136; // B
                p += channels;
            }
            else if (bufr[j] <= 62)
            {
                *p = 64;        // R
                *(p + 1) = 145; // G
                *(p + 2) = 108; // B
                p += channels;
            }
            else if (bufr[j] <= 70)
            {
                *p = 45;        // R
                *(p + 1) = 106; // G
                *(p + 2) = 79;  // B
                p += channels;
            }
            else if (bufr[j] <= 80)
            {
                *p = 27;       // R
                *(p + 1) = 67; // G
                *(p + 2) = 50; // B
                p += channels;
            }
            else if (bufr[j] <= 90)
            {
                *p = 8;        // R
                *(p + 1) = 28; // G
                *(p + 2) = 21; // B
                p += channels;
            }
        }

    } // next line

    /*
     * Write a PNG image:
     */
    stbi_write_png("sample.png", width, height, channels, img, rowsize);

    return 0;
} // main
