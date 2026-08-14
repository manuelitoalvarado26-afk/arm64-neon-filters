#include <stdio.h>
#include <stdint.h>
#include <arm_neon.h>

void filtro_neon (uint8_t* imagen) {
    uint8x8_t vector_pixeles = vld1_u8 (imagen);
    uint8x8_t resultado = vqadd_u8 (vector_pixeles, vdup_n_u8(20));
    vst1_u8(imagen,resultado);
}

int main() {
    uint8_t mi_imagen [8] = {
    10, 20, 30, 255,
    40, 50, 60, 255
};
filtro_neon(mi_imagen);

for (int i = 0; i < 8; i++) {
    printf("Pixel : %d\n", mi_imagen[i]);
}
return 0;
}