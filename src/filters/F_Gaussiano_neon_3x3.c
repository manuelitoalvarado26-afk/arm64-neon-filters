#include <stdint.h>
#include <arm_neon.h>

void Aplicar_Gaussiano_3x3_RGB_neon (const uint8_t *src, uint8_t *dst, int width, int height) {

    //Recorremos verticalmente evitando los bordees (y = 1 hasta height - 1)
    for (int y = 1; y < height - 1; y++) {

        //Recorremosde 8 en 8 pixeles. Limitado a 'width - 9' evitando salirse de la RAM
        for (int x = 1; x <= width - 9; x+=8) {

            //Offsets en BYTES para fila superior, central e inferior 
            //Multiplicamos x * 3 (RGB)
            size_t stride = width * 3;
            size_t off_top = (y - 1) * stride + (x * 3);
            size_t off_mid = y * stride + (x * 3);
            size_t off_bot = (y + 1) * stride + (x * 3);

            //Fila superior (top)
            uint8x8x3_t f0_left = vld3_u8 (&src[off_top - 3]);
            uint8x8x3_t f0_mid = vld3_u8 (&src[off_top]);
            uint8x8x3_t f0_right = vld3_u8 (&src[off_top + 3]);

            //Fila central (mid)
            uint8x8x3_t f1_left = vld3_u8 (&src[off_mid - 3]);
            uint8x8x3_t f1_mid = vld3_u8 (&src[off_mid]);
            uint8x8x3_t f1_right = vld3_u8 (&src[off_mid + 3]);

            //Fila inferior (bot)
            uint8x8x3_t f2_left = vld3_u8 (&src[off_bot - 3]);
            uint8x8x3_t f2_mid = vld3_u8 (&src[off_bot]);
            uint8x8x3_t f2_right = vld3_u8 (&src[off_bot + 3]);

            //Variable de almasenamiento para los 3 canales
            uint8x8x3_t res_rgb;

            
            //Recorremos los 3 canales; c=0 (red), c=1 (green), c=2(blue)
            for (int c = 0; c < 3; c++) {

                //Fila superior (bot) aplicando el filtro (1, 2, 1)
                uint16x8_t sum_top = vmovl_u8 (f0_left.val[c]);
                sum_top = vaddq_u16 (sum_top, vshll_n_u8(f0_mid.val[c],1));
                sum_top = vaddq_u16 (sum_top, vmovl_u8(f0_right.val[c]));

                //Fila central (mid) aplicando filtro (2, 4, 2)
                uint16x8_t sum_mid = vshll_n_u8 (f1_left.val[c],1);
                sum_mid = vaddq_u16 (sum_mid, vshll_n_u8(f1_mid.val[c],2));
                sum_mid = vaddq_u16 (sum_mid, vshll_n_u8(f1_right.val[c],1));

                //fila inferior (bot)aplicando filtro (1, 2, 1)
                uint16x8_t sum_bot = vmovl_u8 (f2_left.val[c]);
                sum_bot = vaddq_u16 (sum_bot, vshll_n_u8(f2_mid.val[c],1));
                sum_bot = vaddq_u16 (sum_bot, vmovl_u8(f2_right.val[c]));

                //Suma de las 3 filas (sum_top, sum_mid, sum_bot)
                uint16x8_t sum_total = vaddq_u16 (sum_top, sum_mid);
                sum_total = vaddq_u16 (sum_total, sum_bot);

                //Divicion entre 16 mediante desplazamiento de 4 bits (>> 4)
                //Empaquetado devuelta a 8 bits (uint8x8_t)
                uint16x8_t res_u16 = vshrq_n_u16 (sum_total,4);
                res_rgb.val[c] = vmovn_u16 (res_u16);
            }

                //Regresando los 8 pixeles procesados en la imagen de destino (dts)
                vst3_u8 (&dst[off_mid], res_rgb);
        }
    }
}