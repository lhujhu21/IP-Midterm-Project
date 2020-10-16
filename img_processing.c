#include "img_processing.h"

// Grayscale function
Image* grayscale(Image* im) {
  Image* out = malloc(sizeof(Image));
  out->rows = im->rows;
  out->cols = im->cols;
  out->data = malloc(sizeof(Pixel) * out->rows * out->cols);

  for (int i = 0; i < (im->rows * im->cols); i++) {
    Pixel p = im->data[i];
    float gray = 0.3 * p->r + 0.59 * p->g + 0.11 * p->b;
    p->r = gray;
    p->g = gray;
    p->b = gray;
    out->data = p;
  }
  return out;
}
