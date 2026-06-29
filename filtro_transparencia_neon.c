#include <stdio.h>
#include <arm_neon.h>

typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} PixelRGBA;

void filtro_transparencia_neon (PixelRGBA* imagen, int num_pixeles, float factor) {
    uint8_t factor_fijo = (uint8_t) (factor * 255.0f);
    uint8x8_t v_factor = vdup_n_u8 (factor_fijo);
    int i = 0;
    
    for (; i <= num_pixeles - 8; i += 8) {
        
        uint8x8x4_t v_rgba = vld4_u8 ((uint8_t*)&imagen[i]);
        uint16x8_t v_alfa_expandido = vmull_u8 (v_rgba.val[3], v_factor);
        v_rgba.val[3] = vshrn_n_u16 (v_alfa_expandido, 8);
        vst4_u8((uint8_t*)&imagen[i], v_rgba);
    }
    
    for (; i < num_pixeles; i++) {
        imagen[i].a = (int) (imagen[i].a * factor);
    }
}