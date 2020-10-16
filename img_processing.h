// Lucy Hu, lhu15
// Morgan Wu, mwu69

/**
 * Creates a new Image struct.
 * @parameter rows the number of rows in image.
 * @paramater cols the number of columns in image.
 * @return pointer to the new struct.
 */
Image* CreateImage(int rows; int cols);

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
Image* Binarize(Image* im, int threshold) {
