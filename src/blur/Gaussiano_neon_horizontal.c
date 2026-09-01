#include <stdint.h>
#include <stddef.h>
#include <arm_neon.h>

void Gaussiano_horizontal_RGB_neon (const uint8_t *src, uint8_t *dst, int width, int height, size_t stride) {

    //Recorrido y caculo de filas
    for (int y = 0; y < height; y++) {
        for (int x = 2; x <= width - 10; x+=8) {

         //Direccion idx cordenadas 2d (x, y) a 1d horizontalmente
        size_t idx = (size_t)y * stride + (x * 3);

        //Carga de 8 pixeles horizontalmente dsentrelazados (vld3_u8)
        uint8x8x3_t p_l2 = vld3_u8 (&src[idx - 6]); // -2 Pixel left (-6 bytes)
        uint8x8x3_t p_l1 = vld3_u8 (&src[idx - 3]); // -1 Pixel left (-3 bytes)
        uint8x8x3_t p_0 = vld3_u8 (&src[idx]); // Pixel central
        uint8x8x3_t p_r1 = vld3_u8 (&src[idx + 3]); // +1 Pixel right (+3 bytes)
        uint8x8x3_t p_r2 = vld3_u8 (&src[idx + 6]); // +2 Pixel right (+6 bytes)

        //Variable de almasenamiento para los 3 canales
        uint8x8x3_t res_rgb;

        //Recorremos los 3 canales; c=0 (red), c=1 (green), c=2(blue)
        for (int c = 0; c < 3; c++) {

            //Promovemos lso extremos a 16 bits y los sumamos: (1 * p_l2) + (1 * pr_2)
            uint16x8_t acc = vmovl_u8 (p_l2.val[c]);
            acc = vaddw_u8 (acc, p_r2.val[c]);

            //Sumamos 4 * p_l1 y 4 * p_r1 (desplazando 2 bits a la iquierda: << 2)
            acc = vaddq_u16 (acc, vshll_n_u8(p_l1.val[c], 2));
            acc = vaddq_u16 (acc, vshll_n_u8(p_r1.val[c], 2));

            //Sumamos 6 * p_0 haciendo: (p_0 << 2) + (p_0 << 1) ---> [4x + 2x = 6x]
            uint16x8_t p0_x4 = vshll_n_u8 (p_0.val[c], 2);
            uint16x8_t p0_x2 = vshll_n_u8 (p_0.val[c], 1);
            acc = vaddq_u16 (acc, vaddq_u16 (p0_x4, p0_x2));

            //Dividimos entre 16 (>> 4) y comprimimos de 16 a 8 bits
            res_rgb.val[c] = vshrn_n_u16 (acc, 4);

            //Guardado vectorial entrelazado
            vst3_u8 (&dst[idx], res_rgb);
        }

        }
    }
}