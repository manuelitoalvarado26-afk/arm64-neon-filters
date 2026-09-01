#include <stdio.h>
#include <stdint.h>
#include "neon_filters.h"

int main() {
    printf ("--- Probando libreria ARM64 NEON ---\n");

    //Prueba de binarizacion
    size_t total_pixels = 16;
    uint8_t entrada [16] = {10, 50, 100, 120, 130, 200, 255, 0, 45, 150, 170, 5};
    uint8_t salida [16] = {0};
    uint8_t umbral = 128;

    Binarization_neon (entrada, salida, total_pixels, umbral);

    printf ("\nResultado Binarization (Umbral %d):\n", umbral);
    for (int i = 0; i < total_pixels; i++) {
        printf ("%d", salida[i]);
    }
    printf ("\n");

    //Prueba de mezcla transparencia RGBA
    PixelRGBA imgA [2] = {{255, 0, 0, 255}, {0, 255, 0, 128}};
    PixelRGBA imgB [2] = {{0, 0, 255, 255}, {255, 255, 0, 255}};
    PixelRGBA img_Res [2] = {0};

    Mezcla_trans (imgA, imgB, img_Res, 2);
    printf ("\nPrueba ejecutada con exito.\n");

    return 0;
}