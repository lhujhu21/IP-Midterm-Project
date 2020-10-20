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
/*  if (argc > 8) { // Check if max number of args is exceeded
    fprintf(stderr, "Error: too many command line arguments\n");
    return 1;
  }
*/

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
    return 3;
  }

  // Create Image struct for input file and for output photo
  Image* im = malloc(sizeof(Image));
  if (!im) {
    fprintf(stderr, "Error: image allocation failed\n");
    return 8; // what error code does this fall under?
  }
  im = ReadPPM(input);
  if (im == NULL) {
    fprintf(stderr, "Error: input file cannot be read as PPM file\n");
    return 4;
  }
  Image* out; // Struct to store processed photo

  // Create Args struct for image processing arguments
  /*
  Args* values = malloc(sizeof(Args));
  if (!values) {
    fprintf(stderr, "Error: unable to allocate memory for arguments\n");
    return 8;
  }
  */
  
  // Match image processing operation
  // Add more cases later!
  char op[] = LowerCase(argv[3]); // used helper function defined in img_processing.h
  // call Grayscale
  if (strcmp(op, "grayscale") == 0) {
    out = Grayscale(im);
  }
  // call Binarize
  else if (strcmp(op, "binarize") == 0) {
    /*
    int check = CheckArgs(op, arc, argv, values);
    if (check != 0) return check;
    */
    int threshold; // Wouldn't need this anymore
    if (sscanf(argv[4], " %d ", &threshold) != 1) {
      fprintf(stderr, "Error: invalid argument for Binarize\n");
      return 7;
    }
    if (argc < 5) {
      fprintf(stderr, "Error: not enough arguments supplied for Binarize function\n");
      return 6; 
    }
    out = Binarize(im, threshold); // change to values->threshold
  }
  // call Crop
  else if (strcmp(op, "crop") == 0) {
    if (argc < 8) {
      fprintf(stderr, "Error: not enough arguments supplied for crop function\n");
      return 6;
    }
    int lcol, lrow, rcol, rrow;
    if (sscanf(argv[4], " %d ", &lcol) != 1 ||
	      sscanf(argv[5], " %d ", &lrow) != 1 ||
	      sscanf(argv[6], " %d ", &rcol) != 1 ||
	      sscanf(argv[7], " %d ", &rrow) != 1) {
      fprintf(stderr, "Error: invalid argument for Crop\n");
      return 7;
    }
    out = Crop(im, lcol, lrow, rcol, rrow); // change to values->lcol, values->lrow, etc.
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
  
  else {
    fprintf(stderr, "Error: unsupported image processing command: %s\n", argv[4]);
    return 5;
  }

  if (out == NULL) {
    // NULL pointer returned because unable to complete operation due to invalid arguments
    return 7;
  }
  
  // Write image to file
  int num = WritePPM(output, out); // Number of pixels successfully written
  
  return 0;
}
