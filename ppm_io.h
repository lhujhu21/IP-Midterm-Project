// Lucy Hu, lhu15
// Morgan Wu, mwu69
/*****************************************************************************
 * Midterm Project - PPM file input/output header file
 * Note: for naming convention, we try to follow Google C++ style guide:
 *       https://google.github.io/styleguide/cppguide.html
 * It is not compulsory, but you are highly encouraged to follow a convention.
 * 
 * Summary: This file provides the utility functions to read/write PPM file
 *****************************************************************************/
#ifndef PPM_IO_H
#define PPM_IO_H

#include <stdio.h> // c file type: FILE

/* A struct to store a point (2D coordinate).
 */ 
typedef struct _point {
  int x;
  int y;
} Point;


/* A struct to store a single RGB pixel, one byte per color channel.
 */
typedef struct _pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

/* A struct to bundle together a pixel array with the other
 * image data we'll frequently want to pass around with it.
 * (This saves us from having to pass the same three 
 * variables to every function.) Note that no Pixels are
 * stored within this struct; the data field is a pointer.
 */
typedef struct _image {
  Pixel *data;  // pointer to array of Pixels
  int rows;     // number of rows of Pixels
  int cols;     // number of columns of Pixels
} Image;

/* GetPixel
 * A function to take the x,y coordinates stored in a Point struct and returns the Pixel at that coordinate in a given image
 * @parameter Point pt has the x and y coordinates of the desired pixel. Origin (0,0) is top left corner of image.
 * @parameter Image * im is a pointer to the image in which the pixel is contained
 * @returns pointer to the Pixel within the image at the given x-y coordinates
 */
Pixel * GetPixel(Point pt, Image * im);

/* ReadPPM
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image* ReadPPM(FILE *fp);

/* WritePPM
 * Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int WritePPM(FILE *fp, const Image *img);

#endif // MIDTERM_PPM_IO_H_
