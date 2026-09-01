#include "filters/Binarizer_Filter.hpp"
#include <arm_neon.h>

namespace core {

    //Constructor: Asigna el umbral a la variable privada trheshold_
    BinarizeFilter::BinarizeFilter (uint8_t threshold)
    : threshold_(threshold) {}

    //Método process: Matemática NEON
    void BinarizeFilter::process (const uint8_t* src, uint8_t* dst, std::size_t total_pixels) {
        std::size_t i = 0;

        //Clonar el umbral privado privado a un registro de 128 bits (16 bytes)
        uint8x16_t vec_threshold = vdupq_n_u8 (threshold_);

        //Bucle principal: procesar 16 pixeles por ciclo de reloj 
        for (; i + 16 <= total_pixels; i += 16) {
            uint8x16_t pixels = vld1q_u8 (src + i);
            uint8x16_t mask = vcgtq_u8 (pixels, vec_threshold);

            //Carga devuelta de los pixeles
            vst1q_u8 (dst + i, mask);
        }

        //Bucle escalar para limpiar los pixeles sobrantes
        for (; i < total_pixels; ++i) {
            dst[i] = (src[i] > threshold_) ? 255 : 0;
        }
    }
}