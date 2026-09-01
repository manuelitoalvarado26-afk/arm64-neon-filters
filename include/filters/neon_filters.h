#ifndef NEON_FILTERS_H
#define NEON_FILTERS_H

#include <stdint.h>
#include <stddef.h>

//Color
typedef struct 
{ uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
}PixelRGBA;

void Binarization_neon (const uint8_t * src, uint8_t *dst, size_t total_pixeles, uint8_t umbral);
void Gray_neon (const uint8_t *src, uint8_t *dst, size_t total_pixeles);
void Mezcla_trans(const PixelRGBA* imgA, const PixelRGBA* imgB, PixelRGBA* imgDest, size_t numPixeles );
void Sepia_neon (uint8_t *datos ,int ancho, int alto, int canales);

//Blur
void Gaussian_3x3_Grey_neon (const uint8_t *src, uint8_t *dst, int width, int height, int stride);
void Gaussiano_3x3_RGB_neon (const uint8_t *src, uint8_t *dst, int width, int height);
void Gaussiano_horizontal_RGB_neon (const uint8_t *src, uint8_t *dst, int width, int height, size_t stride);
void Gaussiano_vertical_RGB_neon (const uint8_t *src, uint8_t *dst, int width, int height, size_t stride);

//Edges
void Sobel_neon(const uint8_t *src, uint8_t *dst, int width, int height);

#endif