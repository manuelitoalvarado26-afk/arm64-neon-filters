#include <stddef.h>
#include <stdint.h>
#include <arm_neon.h>

void Gray_neon (const uint8_t *src, uint8_t *dst, size_t total_pixeles) {

    //Ponderacion de color gris (0.288 x r) (0.587 x g) (0.114 x b)
    uint8x8_t r_weight = vdup_n_u8 (77);
    uint8x8_t g_weight = vdup_n_u8 (150);
    uint8x8_t b_weight = vdup_n_u8 (29);

    //Bucle SIMD y Destrenlazado por hardware
    size_t i = 0;

    for (;i + 7 < total_pixeles; i +=8 ) {
        uint8x8x3_t rgb = vld3_u8 (src);

        //Multiplicacion y acumulacion con expancion de 8 a 16 bits
        uint16x8_t acc = vmull_u8 (rgb.val[0], r_weight);
        acc = vmlal_u8 (acc, rgb.val[1], g_weight);
        acc = vmlal_u8 (acc, rgb.val[2], b_weight);

        //Convercion de 16 a 8 bits y divicion con desplazamiento de bits (>> 8)
        uint8x8_t gray = vshrn_n_u16 (acc, 8);
        
        //Guardar en la memoria 
        vst1_u8 (dst, gray);

        //Avanzar punteros
        src += 24;
        dst += 8;
    }
}