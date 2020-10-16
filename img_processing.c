// Lucy Hu, lhu15
// Morgan Wu, mwu69

#include "img_processing.h"

// Create a new Image struct
Image* CreateImage(int rows; int cols) {
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
  Image* out = CreateImage(im->rows; im->cols);

  for (int i = 0; i < (im->rows * im->cols); i++) {
    Pixel p = im->data[i];
    float gray = 0.3 * p->r + 0.59 * p->g + 0.11 * p->b;
    out->data[i]->r = gray;
    out->data[i]->g = gray;
    out->data[i]->b = gray;
  }
  return out;
}

Image* Binarize(Image* im, int threshold) {
  // Check if threshold value is valid
  if (threshold <= 0 || threshold >= 255) {
    return NULL; 
  }  
  Image* out = Grayscale(input);
  for (int i = 0; i < (out->rows * out->cols); i++) {
    float gray = out->data[i]->r;
    if (gray < threshold) {
      out->data[i]->r = 0;
      out->data[i]->g = 0;
      out->data[i]->b = 0;
    } else {
      out->data[i]->r = 255;
      out->data[i]->g = 255;
      out->data[i]->b = 255;
    }
  }
  return out;
}
