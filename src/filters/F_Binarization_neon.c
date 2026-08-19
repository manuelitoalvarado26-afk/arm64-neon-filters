#include <stdint.h>
#include <stddef.h>
#include <arm_neon.h>

void Binarization_neon (const uint8_t * src, uint8_t *dst, size_t total_pixeles, uint8_t umbral) {

    //Inicio del puntero 
    size_t i = 0;

    //Duplicamos el umbral en los 16 carriles en un registro de 128 bits
    uint8x16_t vec_umbral = vdupq_n_u8 (umbral);

    //Procesamiento vectorial en bloques de 16 pixeles (16 bytes continuos)
    for (; i + 15 < total_pixeles; i+=16) {

        //Cargar 16 pixeles continuos desde la memoria 
        uint8x16_t pixels = vld1q_u8 (src);

        //comparar si Pixel < Umbral (retorna 255 o 0 por carril)
        uint8x16_t mascara = vcgtq_u8 (pixels, vec_umbral);

        //Ecsribir el resultado de 16 bytes en la memoria de destino
        vst1q_u8 (dst, mascara);

        //Avanzar los punteros 
        src += 16;
        dst += 16;

        //Manejo de pixeles remanentes (sobrantes de 1 en 1)
        for (; i < total_pixeles; i++) {
            *dst = (*src > umbral) ? 255 : 0;
            src ++;
            dst ++;
        }
    }
}