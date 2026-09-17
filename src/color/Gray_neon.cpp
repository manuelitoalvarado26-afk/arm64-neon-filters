#include "static/Gray_static.hpp"
#include <cstdint>
#include <arm_neon.h>

namespace core {

    void GrayscalerStatic::process (const uint8_t* src, uint8_t* dst, std::size_t total_pixels) {

        //Declaracion de variables
        std::size_t total_bytes = total_pixels * 3;
        std::size_t i = 0;

        //Procesamiento vectorial por bloques con ARM NEON
        for (; i + 24 <= total_bytes; i+= 24) {

            //Cargar y procesar 8 pixeles RGB ( 24 bytes)
            uint8x8x3_t rgb = vld3_u8 (&src[i]);

            //Sumar R, G y B (se amplia temporalmente a 16 bits evitando desvordamiento)
            uint16x8_t sum = vaddl_u8 (rgb.val[0], rgb.val[1]);
            sum = vaddw_u8 (sum, rgb.val[2]);

            //Multiplicar por 85 y dezplazar a la derecha 8 bits a la derecha (>> 8)
            uint16x8_t gray_16 = vmulq_n_u16 (sum, 85);
            uint8x8_t gray = vshrn_n_u16 (gray_16, 8);

            //Reorganizando los 3 canales
            uint8x8x3_t gray_rgb;
            gray_rgb.val[0] = gray;
            gray_rgb.val[1] = gray;
            gray_rgb.val[2] = gray;

            //Enttrelazar los 24 bytes en el buffer de salida
            vst3_u8 (&dst[i], gray_rgb);

            //Bucle residual 1 + 1
            for (; i < total_bytes; i += 3) {

                uint16_t sum_scalar = src[i] + src[i + 1] + src[i + 2];
                uint8_t gray_scalar = static_cast<uint8_t>((sum_scalar * 85) >> 8);

                dst[i] = gray_scalar;
                dst[i + 1] = gray_scalar;
                dst[i + 2] = gray_scalar;
            }

        }
        
    }
}