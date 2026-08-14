#include <stdint.h>
#include <stddef.h>
#include <arm_neon.h>

void Gaussiano_vertical_RGB_neon (const uint8_t *src, uint8_t *dst, int width, int height) {
    size_t stride = (size_t)stride;

    //Recorrido y calculos de columnas
    for (int y = 2; y < height - 2; y++) {
        for (int x = 0; x <= width; x += 8) {

        //Direccion idx cordenadas 2d (x, y) a 1d verticalmente
        size_t idx = (size_t)y * stride + x * 3;

        //Carga de 8 pixeles verticalmente dsentrelazados (vld3_u8)
        uint8x8x3_t p_t2 = vld3_u8 (&dst[idx - 2 * stride]); // 2 Filas arriba (top 2)
        uint8x8x3_t p_t1 = vld3_u8 (&dst[idx - 1 * stride]); // 1 Fila arriba (top 1)
        uint8x8x3_t p_0 = vld3_u8 (&dst[idx]); // Fila central
        uint8x8x3_t p_b1 = vld3_u8 (&dst[idx - 1 * stride]); // 1 Fila abajo (bottom 1)
        uint8x8x3_t p_b2 = vld3_u8 (&dst[idx - 2 * stride]); // 2 Filas abajo (bottom 2)

        uint8x8x3_t res_rgb;

        //Recorremos los 3 canales; c=0 (red), c=1 (green), c=2(blue)
        for (int c = 0; c < 3; c++) {

            //Promovemos lso extremos a 16 bits y los sumamos: (1 * p_t2) + (1 * p_b2)
            uint16x8_t acc = vmovl_u8 (p_t2.val[c]);
            acc = vaddw_u8 (acc, p_b2.val[c]);

            //Sumamos 4 * p_t1 y 4 * p_b1 (desplazando 2 bits a la iquierda: << 2)
            acc = vaddq_u16 (acc, vshll_n_u8(p_t1.val[c], 2));
            acc = vaddq_u16 (acc, vshll_n_u8(p_b1.val[c], 2));

            //Sumamos 6 * p_0 haciendo: (p_0 << 2) + (p_0 << 1) ---> [4x + 2x = 6x]
            uint16x8_t p0_x4= vshll_n_u8 (p_0.val[c], 2);
            uint16x8_t p0_x2 = vshll_n_u8 (p_0.val[c], 1);
            acc = vaddq_u16 (p0_x4, p0_x2);

            //Dividimos entre 16 (>> 4) y comprimimos de 16 a 8 bits
            res_rgb.val[c] = vshrn_n_u16(acc, 4);

            //Guardado vectorial entrelazado
            vst3_u8 (&dst[idx], res_rgb);
        }

        }
    }
}