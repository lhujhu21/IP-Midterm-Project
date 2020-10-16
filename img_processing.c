// Lucy Hu, lhu15
// Morgan Wu, mwu69

#include "img_processing.h"
#include "ppm_io.h"

/*
int CheckArgs(char *op, int argc, char **argv) {
  // Check Binarize function
  if (strcmp(op, "binarize") == 0) {
    // Incorrect number of values -- returns 6 in main
    if (argc < 5) {
      fprintf(stderr, "Error: not enough arguments supplied for Binarize function\n");
      return -6; 
    }
    // Invalid threshold values -- returns 7 in main
    
    
    }  
  }
}
*/

// Create a new Image struct
Image* CreateImage(int rows, int cols) {
  // Allocate memory for new struct
  Image* new = malloc(sizeof(Image));
  new->rows = rows;
  new->cols = cols;
  new->data = malloc(sizeof(Pixel) * rows * cols);
  return new;
}

// Grayscale function
Image* Grayscale(Image* im) {
  // Allocate memory for output
  Image* out = CreateImage(im->rows, im->cols);

  for (int i = 0; i < (im->rows * im->cols); i++) {
    Pixel p = im->data[i];
    float gray = 0.3 * p.r + 0.59 * p.g + 0.11 * p.b;
    out->data[i].r = gray;
    out->data[i].g = gray;
    out->data[i].b = gray;
  }
  return out;
}

Image* Binarize(Image* im, int threshold) {
  // Check validity of threshold
  if (threshold <= 0 || threshold >= 255) {
    fprintf(stderr, "Error: invalid threshold value %d\n", threshold);
    return NULL;
  }
  Image* out = Grayscale(im);
  for (int i = 0; i < (out->rows * out->cols); i++) {
    float gray = out->data[i].r;
    if (gray < threshold) {
      out->data[i].r = 0;
      out->data[i].g = 0;
      out->data[i].b = 0;
    } else {
      out->data[i].r = 255;
      out->data[i].g = 255;
      out->data[i].b = 255;
    }
  }
  return out;
}

Image* Crop(Image*im, int lcol, int lrow, int rcol, int rrow) {
  // Check if corner values are valid
  if (rcol > im->cols || rrow > im->rows || lcol > rcol || lrow > rrow ||
      lcol < 0 || lrow < 0 ) {
    fprintf(stderr, "Error: invalid corner values %d %d %d %d\n", lcol, lrow, rcol, rrow);
  }
  Image* out = CreateImage(rrow - lrow + 1, rcol - lcol + 1);
  for (int i = lrow; i <= rrow; i++) {
    int new_row = i - lrow;
    for (int j = lcol; i <= rcol; j++) {
      // [row][col] indexing is [i * out->cols + j] 
      int new_col = j - lcol;
      out->data[new_row * out->cols + new_col] = im->data[i * out->cols + j];
    }
  } 
  return out;
}

Image* Transpose(Image* im) {
  Image* out = CreateImage(im->cols, im->rows);
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      out->data[j * out->cols + i] = im->data[i * im->cols + j];
    }
  }
  return out;
}
