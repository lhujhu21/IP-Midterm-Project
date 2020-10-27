// Lucy Hu, lhu15
// Morgan Wu, mwu69

#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

#include "ppm_io.h"

/** 
 * A struct to store all possible image processing arguments.
 */
typedef struct _args {
  int threshold; // Binarize
  int lcol; // Crop
  int lrow; // Crop
  int rcol; // Crop
  int rrow; // Crop
  float col_sf; // Seam carving
  float row_sf; // Seam carving
} Args;


/**
 * Helper function to take user-specified operation and check for correct arguments.
 * @parameter *im the image-to-be-processed for which arguments are checked against (necessary for Crop)
 * @parameter *op the function name to check arguments for.
 * @parameter argc the number of items in argv.
 * @parameter argv array of command line arguments. 
 * @parameter values pointer to Args struct to store read values
 * @return 0 for no errors, 6 for incorrect # of args, 7 for invalid args
 */
int CheckArgs(Image* im, int argc, char **argv, Args* values);


/**
 * Helper function to remove case-sensitivity of user-entered arguments
 * @parameter *op the function name to convert to lowercase
 * @return string where all letters are converted to lowercase
 */
char* LowerCase(char* op);

/**
 * Creates a new Image struct.
 * @parameter rows the number of rows in image.
 * @paramater cols the number of columns in image.
 * @return pointer to the new struct.
 */
Image* CreateImage(int rows, int cols);

/**
 * Converts image to grayscale.
 * @parameter im Image pointer to be converted.
 * @return pointer to the grayscale image struct.
 */
Image* Grayscale(Image* im);

/**
 * Binarizes image.
 * @parameter im Image pointer to be binarized.
 * @parameter threshold value to check against.
 * @return pointer to binarized image struct.
 */
Image* Binarize(Image* im, int threshold);

/**
 * Crops image.
 * @parameter im Image pointer to be cropped.
 * @parameter lcol is the x-value for the upper left corner
 * @parameter lrow is the y-value for the upper left corner
 * @parameter rcol is the x-value for the bottom right corner
 * @parameter rrow is the y-value for the bottom left corner
 * @return pointer to cropped image struct.
 */
Image* Crop(Image* im, int lcol, int lrow, int rcol, int rrow);

/**
 * Transposes image.
 * @parameter im Image pointer to be transposed.
 * @return pointer to transposed image struct.
 */
Image* Transpose(Image* im);

/**
 * Computes the magnitude of the gradient at each interior pixel.
 * Returns image with each pixel's color channels set to gradient magnitude.
 * @parameter im Image pointer for which gradient is computed
 * @return pointer to gradient image.
 */
Image* Gradient(Image *im);

/**
 * Seam carves image. 
 * @parameter im Image pointer for input image.
 * @parameter col_sf column sizescaling factor.
 * @parameter row_sf row size scaling factor 
 * @return pointer to seam carved image. 
 */
Image* Seam(Image* im, float col_sf, float row_sf);

/**
 * Helper function for Seam carving function.
 * @parameter out Image pointer for current iteration of image being seam carved
 * @parameter seams 2D array storing pointers to pixels mapped out as part of a seam
 * Does not return anything, modifies seams array as pass-by-reference, and fills seams 2D array with fully mapped seams
 */
void MapSeams(Image* out, Pixel*** seams);

#endif
