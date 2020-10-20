// Lucy Hu, lhu15
// Morgan Wu, mwu69

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // use tolower() to remove case-sensitivity from user-input
#include "img_processing.h"
#include "ppm_io.h"

int CheckArgs(Image *im, char *op, int argc, char **argv, Args* values) {
  // Check arguments for Binarize function
  if (strcmp(op, "binarize") == 0) {
    // Incorrect number of values -- returns 6 in main
    if (argc < 5) {
      fprintf(stderr, "Error: not enough arguments supplied for Binarize function\n");
      return 6; 
    }
    // Invalid threshold values -- returns 7 in main
    int threshold;
    if (sscanf(argv[4], " %d ", &threshold) != 1) {
      fprintf(stderr, "Error: invalid argument for Binarize\n");
      return 7;
    } else if (threshold <= 0 || threshold >= 255) {
	fprintf(stderr, "Error: invalid threshold value %d\n", threshold);
      return 7;
    }
    // If there are no errors, assign to struct and return 0
    values->threshold = threshold;
    return 0; 
  }

  // Check arguments for Crop function
  if (strcmp(op, "binarize") == 0) {
    // Incorrect number of values
    if (argc < 8) {
      fprintf(stderr, "Error: not enough arguments supplied for crop function\n");
      return 6;
    }
    // Invalid corner values
    int lcol, lrow, rcol, rrow;
    if (sscanf(argv[4], " %d ", &lcol) != 1 ||
	      sscanf(argv[5], " %d ", &lrow) != 1 ||
	      sscanf(argv[6], " %d ", &rcol) != 1 ||
	      sscanf(argv[7], " %d ", &rrow) != 1) {
      fprintf(stderr, "Error: invalid argument for Crop\n");
      return 7;
    } else if (rcol > im->cols || rrow > im->rows || lcol > rcol || 
	             lrow > rrow || lcol < 0 || lrow < 0 ) { 
      fprintf(stderr, "Error: invalid corner values %d %d %d %d\n", lcol, lrow, rcol, rrow);
      return 7;
    }
    // If there are no errors, assign to struct and return 0
    values->lcol = lcol;
    values->lrow = lrow;
    values->rcol = rcol;
    values->rrow = rrow;
    return 0;
  }

  // Check arguments for Seam Carving function
  if (strcmp(op, "seam") == 0) {
    // Incorrect number of values
    if (argc < 6) {
      fprintf(stderr, "Error: not enough arguments supplied for crop function\n");
      return 6;
    }
    // Check for invalid values
    float col_sf;
    float row_sf;
    if (sscanf(argv[4], " %f ", &col_sf) != 1 ||
	      sscanf(argv[5], " %f ", &row_sf) != 1) {
      fprintf(stderr, "Error: invalid argument for Seam Carve\n");
      return 7;
    }
    if (col_sf < 0 || col_sf > 1 ||
	      row_sf < 0 || row_sf > 1) {
      fprintf(stderr, "Error: invalid scaling factor values %f %f\n", col_sf, row_sf);
      return 7;
    }
    // If there are no errors, assign to struct and return 0;
    values->col_sf = col_sf;
    values->row_sf = row_sf;
    return 0;
  }
  return 0;
}

// Convert string to lowercase
char* LowerCase(char* op) {
  for (char* i = op; *i != '\0'; op++) *i = tolower(*i);
  return op;
}

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
    unsigned char gray = 0.3 * p.r + 0.59 * p.g + 0.11 * p.b;
    out->data[i].r = gray;
    out->data[i].g = gray;
    out->data[i].b = gray;
  }
  return out;
}

// Binarize function
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

// Crop function
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

// Transpose function
Image* Transpose(Image* im) {
  Image* out = CreateImage(im->cols, im->rows);
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      out->data[j * out->cols + i] = im->data[i * im->cols + j];
    }
  }
  return out;
}

// Gradient function
Image* Gradient(Image *im){
  // Pass image through Grayscale function
  Image* out = Grayscale(im);
  // Compute magnitude of gradient at each pixel
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      // For pixels on the boundary, set gradient to 0
      unsigned char gradient = 0;
      // For pixels not on the boundary, calculate gradient
      if (i != 0 && i != out->rows && j != 0 && j != out->cols) {
        // Get grayscale values for adjacent pixels
        Pixel p_right = out->data[i * out->cols + j + 1];
        Pixel p_left = out->data[i * out->cols + j - 1];
        Pixel p_top = out->data[(i-1) * out->cols + j];
        Pixel p_bot = out->data[(i+1) * out->cols + j];
        // Calculate gradient in x and y direction
        float x_grad = 0.5*(p_right.r - p_left.r);
        float y_grad = 0.5*(p_top.r - p_bot.r);
        // Take absolute value
        if (x_grad < 0) x_grad *= -1;
        if (y_grad < 0) y_grad *= -1;
        // Sum together for final G(x,y) gradient value
        gradient = x_grad + y_grad;
      }
      // Set gradient value to color channels of the pixel
      Pixel *p = out->data + (i * out->cols + j);
      p->r = gradient;
      p->g = gradient;
      p->b = gradient;
    }
  }
  return out;
}

/*
// Seam carving function
Image* SeamCarving(Image* im, float col_sf, float row_sf) {
  Image* og = CreateImage(im->rows, im->cols); // Save a copy of the original Image struct
  int new_cols = col_sf * im->cols;
  if (new_cols < 2) new_cols = 2;
  int d = im->cols - new_cols;
  for (int seam = 1; seam <= d; seam++) {
    og = Gradient(og);
    for (int i = 0; i < og->cols; i++) {
      Pixel seams[og->cols][og->rows];
      int col = i;
      for (int j = 0; j < og->rows; j++) {
	seams[i][j] = og->data[j * og->cols + i];
	if (j + 1 == og->rows) {
	  seams[i][j] = og->data[]
	}
	
	seams[i][j] = og->data
      }
    }
  }

 




  int new_rows = row_sf * im->row;
  if (new_rows < 2) new_rows = 2;


}

*/
