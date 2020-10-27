// Lucy Hu, lhu15
// Morgan Wu, mwu69

/*****************************************************************************
 * Midterm Project - A program to run the image processing operations
 * Note: for naming convention, we try to follow Google C++ style guide:
 *       https://google.github.io/styleguide/cppguide.html
 * It is not compulsory, but you are highly encouraged to follow a convention.
 *
 * Summary: This file implements a program for image processing operations.
 *          Different operations take different input arguments. In general,
 *            ./project <input> <output> <operation name> [operation params]
 *          The program will return 0 and write an output file if successful.
 *          Otherwise, the below error codes should be returned:
 *            1: Wrong usage (i.e. mandatory arguments are not provided)
 *            2: Input file I/O error
 *            3: Output file I/O error
 *            4: The Input file cannot be read as a PPM file
 *            5: Unsupported image processing operations
 *            6: Incorrect number of arguments for the specified operation
 *            7: Invalid arguments for the specified operation
 *            8: Other errors 
 *****************************************************************************/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ppm_io.h" // PPM I/O header
#include "img_processing.h" // image processing function headers

int main(int argc, char **argv) {
  // Make sure there is at least 4 arguments
  if (argc < 4) {
    fprintf(stderr, "Error: usage should be: ./project input.ppm output.ppm operation_type (optional additional arguments)\n");
    return 6;
  }

  // Open input and output files for binary reading/writing
  // Check for errors 
  FILE* input = fopen(argv[1], "rb");
  if (!input) {
    fprintf(stderr, "Error: couldn't open input file: %s\n", argv[1]);
    return 2;
  }
  
  FILE* output = fopen(argv[2], "wb");
  if (!output) {
    fprintf(stderr, "Error: couldn't open output file: %s\n", argv[2]);
    fclose(input);
    return 3;
  }

  // Create Image struct for input file and for output photo
  Image* im; 
  im = ReadPPM(input);
  if (im == NULL) {
    fprintf(stderr, "Error: input file cannot be read as PPM file\n");
    fclose(input);
    fclose(output);
    return 4;
  }
  Image* out; // Struct to store processed photo

  // Create Args struct for image processing arguments, check arguments for validity
  Args values;
  int check = CheckArgs(im, argc, argv, &values);
  if (check != 0) return check; // If function does not return 0, return error code

  // Match image processing operation
  char *op = LowerCase(argv[3]); // used helper function defined in img_processing.h
  // call Grayscale
  if (strcmp(op, "grayscale") == 0) {
    out = Grayscale(im);
  }
  // call Binarize
  else if (strcmp(op, "binarize") == 0) {
    out = Binarize(im, values.threshold); 
  }
  // call Crop
  else if (strcmp(op, "crop") == 0) {
    out = Crop(im, values.lcol, values.lrow, values.rcol, values.rrow); 
  }
  // call Transpose
  else if (strcmp(op, "transpose") == 0) {
    out = Transpose(im);
  }
  // call Gradient
  else if (strcmp(op, "gradient") == 0) {
    out = Gradient(im);
  }
  // call SeamCarve
  else if (strcmp(op, "seam") == 0) {
    out = Seam(im, values.col_sf, values.row_sf);
  }
  else {
    fprintf(stderr, "Error: unsupported image processing command: %s\n", argv[4]);
    free(im->data);
    free(im);
    fclose(input);
    fclose(output);
    return 5;
  }

  if (out == NULL) {
    // NULL pointer returned because unable to complete operation due to lack of memory for output image
    free(im->data);
    free(im);
    fclose(input);
    fclose(output);
    return 8;
  }
  
  // Write image to file
  int num = WritePPM(output, out); // Number of pixels successfully written

  if (num != out->rows * out->cols) {
    fprintf(stderr, "Error: did not successfully write processed image to output\n");
    free(im->data);
    free(im);
    free(out->data);
    free(out);
    fclose(input);
    fclose(output);
    return 3;
  }

  fclose(input);
  fclose(output);
  if (strcmp(op, "seam") != 0) {
    free(im->data);
    free(im); 
  }
  free(out->data);
  free(out);
  
  return 0;
}
