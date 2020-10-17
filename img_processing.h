// Lucy Hu, lhu15
// Morgan Wu, mwu69

#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

/** 
 * A struct to store all possible image processing arguments.
 */
typedef struct _args {
  int threshold;
  int lcol;
  int lrow;
  int rcol;
  int rrow;
  // float csf; // column scale factor
  // float rsf; // row scale factor
} Args;

/*
 * Helper function to take user-specified operation and check for correct arguments.
 * @parameter *op the function name to check arguments for.
 * @parameter argc the number of items in argv.
 * @parameter argv array of command line arguments. 
 * @parameter values pointer to Args struct to store read values
 * @return 0 for no errors, 6 for incorrect # of args, 7 for invalid args
int CheckArgs(char *op, int argc, char **argv, Args* values);
*/

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

#endif
