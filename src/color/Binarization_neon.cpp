#include "static/Binarizer_static.hpp"
#include <arm_neon.h>

namespace core {

    //Método process: Matemática NEON
    void BinarizeStatic::process (const uint8_t* src, uint8_t* dst, std::size_t total_pixels, uint8_t threshold) {
        std::size_t i = 0;

        //Clonar el umbral privado privado a un registro de 128 bits (16 bytes)
        uint8x16_t vec_threshold = vdupq_n_u8 (threshold);
        
        //Bucle principal: procesar 16 pixeles por ciclo de reloj 
        for (; i + 16 <= total_pixels; i += 16) {
            uint8x16_t pixels = vld1q_u8 (src + i);
            uint8x16_t mask = vcgtq_u8 (pixels, vec_threshold);

            //Carga devuelta de los pixeles
            vst1q_u8 (dst + i, mask);
        }

        //Bucle escalar para limpiar los pixeles sobrantes
        for (; i < total_pixels; ++i) {
            dst[i] = (src[i] > threshold) ? 255 : 0;
        }
    }
}