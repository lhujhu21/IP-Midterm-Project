// Lucy Hu, lhu15
// Morgan Wu, mwu69

#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

/**
 * Helper function to take user-specified operation and check for correct arguments:
 
int CheckArgs(char *op, int argc, char **argv);
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