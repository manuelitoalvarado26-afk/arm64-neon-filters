#include "static/Sobel_static.hpp"
#include <arm_neon.h>
#include <cstring>

namespace core {

    void SobelStatic::process(const uint8_t* src, uint8_t* dst, int width, int height) {

        if (!src || !dst || width < 3 || height < 3) {
            return;
        } 

        //Limpieza del buffer de destino (0) para evitar leer bordes externos de la imagen
        std::memset(dst, 0, static_cast<std::size_t>(width * height));

        //Definiendo los limites de procesamiento Izquierda(x - 1) Derecha(x + 1)
        int limit_y = height - 1;
        int limit_x = width - 1;

        //Bucle vertical comenzando desde y
        for (int y = 1; y < limit_y; y++) {
            
            //Punteros de las 3 filas que usara la ventana 3x3
            const uint8_t* row_top = src + (y - 1) * width;
            const uint8_t* row_mid = src + y * width;
            const uint8_t* row_bot = src + (y + 1) * width;
            uint8_t* row_dst = dst + y * width;


        //Bucle horizontal que avanza de 8 en 8 pixeles 
        int x = 1;
        for (; x <= limit_x - 8; x+= 8) {

            //Carga de la fila superior (row_top)
            uint8x8_t t_left  = vld1_u8 (row_top + x - 1);
            uint8x8_t t_mid   = vld1_u8 (row_top + x);
            uint8x8_t t_right = vld1_u8 (row_top + x + 1); 

            //Carga de la fila central (row_mid) 
            uint8x8_t m_left  = vld1_u8 (row_mid + x - 1);
            uint8x8_t m_right = vld1_u8 (row_mid + x + 1);
            //m_mid no se necesita la matriz Sobel tiene 0 en el centro.

            //Carga de la fila inferior (row_bot)
            uint8x8_t b_left  = vld1_u8 (row_bot + x - 1);
            uint8x8_t b_mid   = vld1_u8 (row_bot + x);
            uint8x8_t b_right = vld1_u8 (row_bot + x + 1); 


            //Promovemos cada vector de 8 bits (uint8x8_t) a 16 bits con signo (int16x8_t)
            int16x8_t t_left_16  = vreinterpretq_s16_u16 (vmovl_u8(t_left));
            int16x8_t t_mid_16   = vreinterpretq_s16_u16 (vmovl_u8(t_mid));
            int16x8_t t_right_16 = vreinterpretq_s16_u16 (vmovl_u8(t_right));

            int16x8_t m_left_16  = vreinterpretq_s16_u16(vmovl_u8(m_left));
            int16x8_t m_right_16 = vreinterpretq_s16_u16(vmovl_u8(m_right));

            int16x8_t b_left_16  = vreinterpretq_s16_u16(vmovl_u8(b_left));
            int16x8_t b_mid_16   = vreinterpretq_s16_u16(vmovl_u8(b_mid));
            int16x8_t b_right_16 = vreinterpretq_s16_u16(vmovl_u8(b_right));


            //Sumamos la columna derecha 
            int16x8_t right_col = vaddq_s16 (t_right_16, b_right_16);
            right_col = vaddq_s16 (right_col, vshlq_n_s16(m_right_16, 1));

            //Sumamos la clumna izquierda
            int16x8_t left_col = vaddq_s16 (t_left_16, b_left_16);
            left_col = vaddq_s16 (left_col, vshlq_n_s16(m_left_16, 1));

            //Calculamos Gx (Derecha - Izquierda)
            int16x8_t gx = vsubq_s16 (right_col, left_col);


            //Sumamos la fila inefior (Abajo)
            int16x8_t bot_row = vaddq_s16 (b_left_16, b_right_16);
            bot_row = vaddq_s16 (bot_row, vshlq_n_s16(b_mid_16, 1));

            //Suamos la fila de superior (Arriba)
            int16x8_t top_row = vaddq_s16 (t_left_16, t_right_16);
            top_row = vaddq_s16 (top_row, vshlq_n_s16(t_mid_16, 1));
            
            //Calculamos Gy (Abajo - Arriba)
            int16x8_t gy = vsubq_s16 (bot_row, top_row);
            

            //Magnitud = [Gx] + [Gy]
            int16x8_t abs_gx = vabsq_s16 (gx);
            int16x8_t abs_gy = vabsq_s16 (gy);
            int16x8_t magnitude = vaddq_s16 (abs_gx, abs_gy);

            //Convertimos de 16 bits con signo a 8 bits sin signo con saturacion 
            uint8x8_t result = vqmovn_s16 (magnitude);

            //Guardamos los 8 pixeles resultantes en la imagen de salida
            vst1_u8 (row_dst + x, result);
        }
        }
    }
}