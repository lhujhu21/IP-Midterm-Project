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
#include "ppm_io.h" // PPM I/O header
#include "img_processing.h"

// TODO: include required headers for your projects.  
// We recommend to put your image processing operations in 
//  img_processing.h for declarations and
//  img_processing.c for their defintions
// Then you should include the below header:
//#include "img_processing.h" // Image processing header

int main(int argc, char **argv) {
  if (argc > 8) { // Check if max number of args is exceeded
    fprintf(stderr, "Error: too many command line arguments\n");
    return 1;
  }

  // Open input and output files for binary reading/writing
  // Check for errors 
  FILE* input = fopen(argv[2], "rb");
  if (!input) {
    fprintf(stderr, "Error: couldn't open input file: %s\n", argv[2]);
  }
  FILE* output = fopen(argv[3], "wb");
  if (!output) {
    fprintf(stderr, "Error: couldn't open output file: %s\n", argv[3]);
  }

  // Create Image struct for input file and for output photo
  Image* im = ReadPPM(input);
  Image* out; // Struct to store processed photo

  // Match image processing operation
  // Add more cases later!
  switch (argv[4]) {
  case "grayscale":
    out = Grayscale(Im);
    break;
  case "binarize":
    out = Binarize(Im, (int)argv[5]);
    break;
  default:
    fprintf(stderr, "Error: invalid image processing command: %s\n", argv[4]);
  }
  
  // Write image to file
  int num = WritePPM(output, out); // Number of pixels successfully written
  
  return 0;
}
