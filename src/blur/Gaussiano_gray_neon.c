#include <stdint.h>
#include <stddef.h>
#include <arm_neon.h>

void Gaussian_3x3_Grey_neon (const uint8_t *src, uint8_t *dst, int width, int height, int stride ) {

    //Calculando el limite del vectorizador sin salirse de los bordes
    int width_aligned = (width - 1) & ~7;

    //Recorriendo las filas desde 1 hasta height -2 (evitamos bordes verticales)
    for (int y = 1; y < height - 1; y++) {
        const uint8_t *row_top = &src[(y - 1) * stride];
        const uint8_t *row_mid = &src[y * stride];
        const uint8_t *row_bot = &src[(y + 1) * stride];
        uint8_t *row_dst = &dst [y * stride];

        // Recorriendo las columnas de 8 en 8 pixeles
        for (int x = 1; x < width_aligned; x +=8) {

            //Fila superior (y - 1)
           uint8x8_t r0_L = vld1_u8 (row_top + x - 1);
           uint8x8_t r0_C = vld1_u8 (row_top + x);
           uint8x8_t r0_R = vld1_u8 (row_top + x + 1);

           //Fila central (y)
           uint8x8_t r1_L = vld1_u8 (row_mid + x - 1);
           uint8x8_t r1_C = vld1_u8 (row_mid + x);
           uint8x8_t r1_R = vld1_u8 (row_mid + x + 1);

           //Fila inferior (y + 1)
           uint8x8_t r2_L = vld1_u8 (row_bot + x - 1);
           uint8x8_t r2_C = vld1_u8 (row_bot + x);
           uint8x8_t r2_R = vld1_u8 (row_bot + x + 1);

           //Filtro para la fila superior (y - 1) (1, 2, 1)
           uint16x8_t sum_top = vmovl_u8 (r0_L);
           sum_top = vaddq_u16 (sum_top, vshll_n_u8 (r0_C, 1)); //r0_C
           sum_top = vaddw_u8 (sum_top, r0_R);

           //Filtro para la fila central (y) (2, 4, 2)
           uint16x8_t sum_mid = vshll_n_u8 (r1_L, 1); //r1_L * 2
           sum_mid = vaddq_u16 (sum_mid, vshll_n_u8 (r1_C, 2)); // r1_C * 4
           sum_mid = vaddq_u16 (sum_mid, vshll_n_u8 (r1_R, 1)); //r1_R * 2

           //Filtro para la fila inferior (y + 1) (1, 2, 1)
           uint16x8_t sum_bot = vmovl_u8 (r2_L);
           sum_bot = vaddq_u16 (sum_bot, vshll_n_u8 (r2_C, 1)); //r2_C
           sum_bot = vaddw_u8 (sum_bot, r2_R);

           //Sumando los valores en conjunto
           uint16x8_t sum_all = vaddq_u16 (sum_top, sum_mid);
           sum_all = vaddq_u16 (sum_all, sum_bot);

           //Dividir entre 16 ( >> 4) y reempaquetando a 8 bits
           uint8x8_t res_gray = vshrn_n_u16 (sum_all, 4);

           //Gurdadndo resultados en la memoria 
           vst1_u8 (row_dst + x, res_gray );
        }
    }
}