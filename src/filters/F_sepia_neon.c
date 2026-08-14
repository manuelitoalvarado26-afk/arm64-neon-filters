#include <stdint.h>
#include <arm_neon.h>

void Tono_sepia_neon (uint8_t *datos ,int ancho, int alto, int canales) {


//Duplicacion por 4 veces de todos los canales 
float32x4_t c_R_r = vdupq_n_f32 (0.393f);
float32x4_t c_R_g = vdupq_n_f32 (0.769f);
float32x4_t c_R_b = vdupq_n_f32 (0.189f);

float32x4_t c_G_r = vdupq_n_f32 (0.349f);
float32x4_t c_G_g = vdupq_n_f32 (0.686f);
float32x4_t c_G_b = vdupq_n_f32 (0.168f);

float32x4_t c_B_r = vdupq_n_f32 (0.272f);
float32x4_t c_B_g = vdupq_n_f32 (0.534f);
float32x4_t c_B_b = vdupq_n_f32 (0.131f);

//Limitando el valor a 255
float32x4_t max_255 = vdupq_n_f32 (255.0f);

size_t total_de_pixeles = (size_t) ancho * (size_t) alto;
size_t total_bytes = (size_t) total_de_pixeles * 3;

for (size_t i = 0; i < total_bytes; i+=24) {

    //Desentrelazado de bits
    uint8x8x3_t pixeles = vld3_u8 (&datos[i]);

    //Convercion de R int a float 
    //Bloque A (Pixeles 0-3)
    uint16x4_t R_16_A = vget_low_u16 (vmovl_u8(pixeles.val[0]));
    float32x4_t R_f32_A = vcvtq_f32_u32 (vmovl_u16(R_16_A));

    //Bloque B (Pixeles 4-7)
    uint16x4_t R_16_B = vget_high_u16 (vmovl_u8(pixeles.val[0]));
    float32x4_t R_f32_B = vcvtq_f32_u32 (vmovl_u16(R_16_B));


    //Convercion de G int a float
    //Bloque A (Pixeles 0-3)
    uint16x4_t G_16_A = vget_low_u16 (vmovl_u8(pixeles.val[1]));
    float32x4_t G_f32_A = vcvtq_f32_u32 (vmovl_u16(G_16_A));

    //Bloque B (Pixeles 4-7)
    uint16x4_t G_16_B = vget_high_u16 (vmovl_u8(pixeles.val[1]));
    float32x4_t G_f32_B = vcvtq_f32_u32 (vmovl_u16(G_16_B));

    
    //Convercion de B int a float
    //Bloque A (Pixeles 0-3)
    uint16x4_t B_16_A = vget_low_u16 (vmovl_u8(pixeles.val[2]));
    float32x4_t B_f32_A = vcvtq_f32_u32 (vmovl_u16(B_16_A));

    //Bloque B (Pixeles 4-7)
    uint16x4_t B_16_B = vget_high_u16 (vmovl_u8(pixeles.val[2]));
    float32x4_t B_f32_B = vcvtq_f32_u32 (vmovl_u16(B_16_B));


    //Nuevos canales (sumando sus valores en conjunto)
    //Canal R
    //Bloque A (Pixeles 0-3)
    float32x4_t nuevo_R = vmulq_f32 (R_f32_A, c_R_r);
    nuevo_R = vmlaq_f32 (nuevo_R, G_f32_A, c_R_g);
    nuevo_R = vmlaq_f32 (nuevo_R, B_f32_A, c_R_b);

    //Bloque B (Pixeles 4-7) 
    float32x4_t nuevo_R2 = vmulq_f32 (R_f32_B, c_R_r);
    nuevo_R2 = vmlaq_f32 (nuevo_R2, G_f32_B, c_R_g);
    nuevo_R2 = vmlaq_f32 (nuevo_R2 ,B_f32_B, c_R_b);


    //Canal G
    //Bloque A (Pixeles 0-3)
    float32x4_t nuevo_G = vmulq_f32 (R_f32_A, c_G_r);
    nuevo_G = vmlaq_f32 (nuevo_G, G_f32_A, c_G_g);
    nuevo_G = vmlaq_f32 (nuevo_G, B_f32_A, c_G_b);

    //Bloque B (Pixeles 4-7)
    float32x4_t nuevo_G2 = vmulq_f32 (R_f32_B, c_G_r);
    nuevo_G2 = vmlaq_f32 (nuevo_G2, G_f32_B, c_G_g);
    nuevo_G2 = vmlaq_f32 (nuevo_G2, B_f32_B, c_G_b);


    //Canal B
    //Bloques A (Pixeles 0-3)
    float32x4_t nuevo_B = vmulq_f32 (R_f32_A, c_B_r);
    nuevo_B = vmlaq_f32 (nuevo_B, G_f32_A, c_B_g);
    nuevo_B = vmlaq_f32 (nuevo_B, B_f32_A, c_B_b);

    //Bloque B (Pixeles 4-7)
    float32x4_t nuevo_B2 = vmulq_f32 (R_f32_B, c_B_r);
    nuevo_B2 = vmlaq_f32 (nuevo_B2, G_f32_B, c_B_g);
    nuevo_B2 = vmlaq_f32 (nuevo_B2, B_f32_B, c_B_b);

    //Valores limitados a 255
    //Bloque A
    nuevo_R = vminq_f32 (nuevo_R, max_255);
    nuevo_G = vminq_f32 (nuevo_G, max_255);
    nuevo_B = vminq_f32 (nuevo_B, max_255);

    //Bloque B
    nuevo_R2 = vminq_f32 (nuevo_R2, max_255);
    nuevo_G2 = vminq_f32 (nuevo_G2, max_255);
    nuevo_B2 = vminq_f32 (nuevo_B2, max_255);

   //Convirtiendo los valores float devuelta a int (16bits)
   uint16x4_t R_Final_16_A = vmovn_u32 (vcvtq_u32_f32(nuevo_R));
   uint16x4_t R_Final_16_B = vmovn_u32 (vcvtq_u32_f32(nuevo_R2));

   uint16x4_t G_Final_16_A = vmovn_u32 (vcvtq_u32_f32(nuevo_G));
   uint16x4_t G_Final_16_B = vmovn_u32 (vcvtq_u32_f32(nuevo_G2));

   uint16x4_t B_Final_16_A = vmovn_u32 (vcvtq_u32_f32(nuevo_B));
   uint16x4_t B_Final_16_B = vmovn_u32 (vcvtq_u32_f32(nuevo_B2));

   //Regresandolos a int de 8 bits convinando los canales A y B
   uint8x8_t R_Final_8 = vqmovn_u16 (vcombine_u16(R_Final_16_A,R_Final_16_B));
   uint8x8_t G_Final_8 = vqmovn_u16 (vcombine_u16(G_Final_16_A, G_Final_16_B));
   uint8x8_t B_Final_8 = vqmovn_u16 (vcombine_u16(B_Final_16_A, B_Final_16_B));
    

    //Empaquetado y entrelazado de bits
    uint8x8x3_t final_pixeles;
    final_pixeles.val[0] = R_Final_8;
    final_pixeles.val[1] = G_Final_8;
    final_pixeles.val[2] = B_Final_8;

    vst3_u8(&datos[i], final_pixeles);

}
}
