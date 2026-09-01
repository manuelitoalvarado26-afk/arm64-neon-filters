#include <stdint.h>
#include <arm_neon.h>

void Sobel_neon(const uint8_t *src, uint8_t *dst, int width, int height) {

    //Recorriendo filas verticales y (evitando horizontales)
    for (int y = 1; y < height -1 ; y++) {
        
        //Punteros marcando el inico de las 3 filas (inicio de los pixeles)
        const uint8_t *row_top = src + (y - 1) * width;
        const uint8_t *row_mid = src + y * width;
        const uint8_t *row_bot = src + (y + 1) * width;

        //Destino del puntero donde se escribira 
        uint8_t *out = dst + y * width;

        //Recorriendo filas horizontales x 
        for (int x = 1; x <= width - 9; x +=8) {

            //Cargando 8 pixeles a izquierda y derecha 
            //Fila Arriba
            uint8x8_t top_left = vld1_u8 (row_top + x - 1);
            uint8x8_t top_right = vld1_u8 (row_top + x + 1);

            //Fila Medio
            uint8x8_t mid_left = vld1_u8 (row_mid + x - 1);
            uint8x8_t mid_right = vld1_u8 (row_mid +x + 1);

            //Fila Abajo
            uint8x8_t bot_left = vld1_u8 (row_bot + x - 1);
            uint8x8_t bot_right = vld1_u8 (row_bot + x + 1);

            //Resta con estiramiento a 16 bits con signo (Derecha e Izquierda)
            int16x8_t diff_top = vsubl_u8 (top_right, top_left);
            int16x8_t diff_mid = vsubl_u8 (mid_right, mid_left);
            int16x8_t diff_bot = vsubl_u8 (bot_right, bot_left);
            
            //Multiplicando la fila de enmedio por 2 (Desplazamiento)
            int16x8_t mid_x2 = vshlq_n_s16 (diff_mid, 1);

            //Sumando las dos filas para obtener el Gx (Gradiente Horizontal)
            int16x8_t Gx = vaddq_s16 (diff_top, mid_x2);
            Gx = vaddq_s16 (Gx, diff_bot);

            //Tomando el valor absoluto 
            int16x8_t Gx_abs = vabsq_s16 (Gx);

            //Reducir de 16 bits a 8 bits sin signo y saturando
            uint8x8_t res_u8 = vqmovn_s16 (Gx_abs);

            //Guardando en la memoria 
            vst1_u8(out + x, res_u8);

        }
    }
}
 
