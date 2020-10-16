#include "img_processing.h"

// Create a new Image struct
Image* createImage(int rows; int cols) {
  // Allocate memory for new struct
  Image* new = malloc(sizeof(Image));
  new->rows = rows;
  new->cols = cols;
  new->data = malloc(sizeof(Pixel) * rows * cols);
  return new;
}

// Grayscale function
Image* grayscale(Image* im) {
  // Allocate memory for output
  Image* out = createImage(im->rows; im->cols);

  for (int i = 0; i < (im->rows * im->cols); i++) {
    Pixel p = im->data[i];
    float gray = 0.3 * p->r + 0.59 * p->g + 0.11 * p->b;
    out->data[i]->r = p;
    out->data[i]->g = p;
    out->data[i]->b = p;
  }
  return out;
}
