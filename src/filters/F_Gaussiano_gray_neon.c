#include <arm_neon.h>
#include <stdint.h>

void Gaussian_3x3_Grey_neon (const uint8_t *src, uint8_t *dst, int width, int height, int stride ) {

    //Recorriendo las filas desde 1 hasta height -2 (evitamos bordes verticales)
    for (int y = 1; y < height - 1; y++) {

        // Recorriendo las columnas de 8 en 8 pixeles
        for (int x = 1; x <= width - 9; x +=8) {

            //1. Fila superior (y -1)
            uint8x8_t r0_p0 = vld1_u8 (&src[(y - 1) * stride + (x - 1)]);  //Izquierda
            uint8x8_t r0_p1 = vld1_u8 (&src[(y - 1) * stride + x]);  //Centro
            uint8x8_t r0_p2 = vld1_u8 (&src[(y - 1) * stride + (x + 1)]); // Derecha

            //Operando con el filtro (1, 2, 1)
            uint16x8_t sum_top = vmovl_u8 (r0_p0);
            sum_top = vaddq_u16 (sum_top, vshll_n_u8(r0_p1, 1));
            sum_top = vaddq_u16 (sum_top, vmovl_u8(r0_p2));


            //Fila central (y)
            uint8x8_t r1_p0 = vld1_u8 (&src[y * stride + (x - 1)]); // Izquierda
            uint8x8_t r1_p1 = vld1_u8 (&src[y * stride + x]); //Centro
            uint8x8_t r1_p2 = vld1_u8 (&src[y * stride + (x + 1)]); //Derecha

            //Operando con el filtro [2, 4, 2]
            uint16x8_t sum_mid = vshll_n_u8 (r1_p0, 1); 
            sum_mid = vaddq_u16 (sum_mid, vshll_n_u8(r1_p1, 2));
            sum_mid = vaddq_u16 (sum_mid, vshll_n_u8(r1_p2, 1));


            //Fila inferior (y + 1)
            uint8x8_t r2_p0 = vld1_u8 (&src[(y + 1) * stride + (x - 1)]); // Izquierda 
            uint8x8_t r2_p1 = vld1_u8 (&src[(y + 1) * stride + x]); //Centro
            uint8x8_t r2_p2 = vld1_u8 (&src[(y + 1) * stride + (x + 1)]); // Derecha

            //Operando con el filtro (1, 2, 1)
            uint16x8_t sum_bot = vmovl_u8 (r2_p0);
            sum_bot = vaddq_u16 (sum_bot, vshll_n_u8(r2_p1, 1));
            sum_bot = vaddq_u16 (sum_bot, vmovl_u8(r2_p2));


            //Suma de las 3 filas en un solo acumulador
            uint16x8_t sum_total = vaddq_u16 (sum_top, sum_mid);
            sum_total = vaddq_u16 (sum_total, sum_bot);

            //Divicion entre 16 mediante desplazamiento de 4 bits (>> 4)
            //Empaquetado devuelta a 8 bits (uint8x8_t)
            uint16x8_t res_u16 = vshrq_n_u16 (sum_total, 4);
            uint8x8_t res_u8 = vmovn_u16 (res_u16);
            

            //Regresando los 8 pixeles procesados en la imagen de destino (dts)
            vst1_u8 (&dst[y * stride + x], res_u8);

        }
    }
}