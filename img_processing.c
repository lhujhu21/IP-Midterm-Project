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
    if (argc != 5) {
      fprintf(stderr, "Error: incorrect number of arguments supplied for Binarize function\n");
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
  if (strcmp(op, "crop") == 0) {
    // Incorrect number of values
    if (argc != 8) {
      fprintf(stderr, "Error: incorrect number of arguments supplied for crop function\n");
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
    if (argc != 6) {
      fprintf(stderr, "Error: incorrect number of arguments supplied for crop function\n");
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
  for (char* i = op; *i != '\0'; i++) {
    *i = tolower(*i);
  }
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
  // Create properly sized out image
  Image* out = CreateImage(rrow - lrow + 1, rcol - lcol + 1);
  int out_idx = 0;
  // Loop through rows
  for (int i = lrow; i < rrow; i++) {
    // Set y-value for current point, will be same for all points in the row
    Point current; current.y = i;
    // Loop through columns
    for (int j = lcol; j < rcol; j++) {
      current.x = j;
      Pixel current_pix = * GetPixel(current, im);
      out->data[out_idx].r = current_pix.r;
      out->data[out_idx].g = current_pix.g;
      out->data[out_idx].b = current_pix.b;
      out_idx++;

      //out->data[out_idx] = *GetPixel(current, im);
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
  
  // Calculate how many columns will be in the final seam-carved image
  int final_cols = col_sf * im->cols;
  if (final_cols < 2) final_cols = 2; // cap final column count at a minimum of 2
  // Calculate d, the number of seams to be carved from the original image
  int d = im->cols - final_cols;

  // Pass image through Gradient function
  Image * out = Gradient(im);

  // For every iteration of this loop, carve out one seam and realloc memory for the seam-carved image until d seams are carved out
  for (int seam = 1; seam <= d; seam++) {
    // Each potential seam is one row, with out->rows number of pixels in each seam
    Pixel * seams[out->cols][out->rows]; 
    seams = GetPotentialSeams(out, seams);

    // TO-DOs FOR FUTURE SELVES: LOOK AT COMMENTS
    // 1) Now that seams array contains all potential seams, sum over each row to get the gradient energy of each seam
    // 2) Identify seam with the lowest gradient energy
    // 3) Allocate new image with one less column representing the carved out seam
    // 4) Copy each pixel over to new image, unless pixel pointer == a seam pixel pointer. 
    //    Probably best to increment pointer through the identified seam to be carved out.
    // 5) Set out image to this new image
    // 6) Repeat the loop until all seams are carved out
    // 7) Transpose and do the same with rows

    // IDEA: not recursion, but a helper function to perform the actual Seam Carve within the SeamCarving Function

    // NOTE TO SELVES: don't forget to free memory after allocating new image memory
    // i.e. if new image (with one less column for the carved out seam) is malloc-ed to Image * new, 
    // free(out), then set Image * out = new
  }
}

int GetPotentialSeams(out, seams) {
  // For every iteration of this loop, start at each column index 'col' to map a potential seam
    for (int i = 0; i < out->cols; i++) {
      // Create a 2D array of pixel pointers to store seams, each starting with a pixel in unique column index and row 0
      int col = i; // index variable to keep track of column position of seam as it is carved out. 
      
      // For every iteration of this loop, do two things: 
      // 1) Store the current pixel into 2D array for the current seam
      // 2) Compare gradient values in neighboring pixels in the next row and move to the pixel with the least gradient magnitude
      //    Update 'col' index accordingly, and end iteration to move to next row
      for (int j = 0; j < out->rows; j++) {
        // Store current pixel in 2D array, at index [i][j]
        Point pt; pt.x = col; pt.y = j;
        Pixel *p = GetPixel(pt, out);
        seams[i][j] = p;
  
        // Before getting gradient magnitude of neighbors, consider eligibility of the next pixel. 
        // Do not get gradient magnitudes of boundary pixels.
        unsigned char least_gradient = 255;
        // If current pixel is in first or last column (boundary), default to right or left neighbor respectively. 
        // ****(not necessary due to way the else statements are set up, but may make the code run faster as won't have to check)****
        if (pt.x == 0) col += 1;
        else if (pt.x == out->rows) col -= 1;
        // If second-to-last row, connect automatically to below pixel
        else if (pt.y == out->rows - 1) col = col;
        // Else, consider three neighbors (two for columns adjacent to boundary, since boundary pixels not considered)
        else {
          int temp_col = col; // temporary variable to keep track of column index of neighbor with lowest gradient
          // Look at left neighbor
          Point left_pt; left_pt.x = col-1; left_pt.y = j+1;
          Pixel *left_pix = GetPixel(left_pt, out);
          if (left_pt.x != 0) { // checking not a boundary pixel
            if (left_pix->r < least_gradient) {
              least_gradient = left_pix->r;
              temp_col = col + 1;
            }
          }
          // Look at below neighbor
          Point below_pt; below_pt.x = col; below_pt.y = j+1;
          Pixel *below_pix = GetPixel(below_pt, out);
          if (below_pt.x != 0 || below_pt.x != out->rows) {
            if (below_pix->r < least_gradient) {
              least_gradient = below_pix->r;
              temp_col = col;
            }
          }
          // Look at right neighbor
          Point right_pt; right_pt.x = col+1; below_pt.y = j+1;
          Pixel *right_pix = GetPixel(right_pt, out);
          if (right_pt.x != out->rows) {
            if (right_pix->r < least_gradient) {
              least_gradient = right_pix->r;
              temp_col = col + 1;
            }
          }
          // Update 'col' index to index stored in temp_col
          col = temp_col;
        }
      }
    }
  return &seams; // DOUBLE-CHECK AMPERSAND
}

*/
