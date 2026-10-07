#include "static/Sepia_static.hpp"
#include <arm_neon.h>
#include <cstdint>
#include <algorithm>

namespace core {
    void SepiaStatic::process(const uint8_t* src, uint8_t* dst, std::size_t total_pixels) {
        if (!src || !dst || total_pixels == 0) return;

        //Coeficientes para el canal Rojo (R)
        const float32x4_t c_R_r = vdupq_n_f32 (0.393F);
        const float32x4_t c_R_g = vdupq_n_f32 (0.769F);
        const float32x4_t c_R_b = vdupq_n_f32 (0.189F);

        //Coeficientes para el canal Verde (G)
        const float32x4_t c_G_r = vdupq_n_f32 (0.349f);
        const float32x4_t c_G_g = vdupq_n_f32 (0.686f);
        const float32x4_t c_G_b = vdupq_n_f32 (0.168f);

        //Coeficientes para el canal Azul (B)
        const float32x4_t c_B_r = vdupq_n_f32 (0.272f);
        const float32x4_t c_B_g = vdupq_n_f32 (0.534f);
        const float32x4_t c_B_b = vdupq_n_f32 (0.131f);

        //Valor máximo para evitar rebasar 255
        const float32x4_t max_255 = vdupq_n_f32 (255.0f);

        //Declaracion de variables 
        std::size_t i = 0;
        std::size_t vector_pixels = (total_pixels / 8) * 8;

        
        for (; i < vector_pixels; i += 8) {
            uint8x8x3_t pixels = vld3_u8 (src + (i * 3));
            
            //Ampliamos de 8 bits a 16 bits
            uint16x8_t r_16 = vmovl_u8 (pixels.val[0]);
            uint16x8_t g_16 = vmovl_u8 (pixels.val[1]);
            uint16x8_t b_16 = vmovl_u8 (pixels.val[2]);

            //Separamos los 8 pixeles en dos bloques de 4 para pasarlos a float32x4//
            //Bloque Bajo (Pixeles 0 a 3)
            float32x4_t r_low = vcvtq_f32_u32 (vmovl_u16(vget_low_u16(r_16)));
            float32x4_t g_low = vcvtq_f32_u32 (vmovl_u16(vget_low_u16(g_16)));
            float32x4_t b_low = vcvtq_f32_u32 (vmovl_u16(vget_low_u16(b_16)));

            //Canal Rojo nuevo (low)
            float32x4_t nr_low_acc = vmulq_f32(r_low, c_R_r);
            nr_low_acc = vfmaq_f32 (nr_low_acc, g_low, c_R_g);
            nr_low_acc = vfmaq_f32 (nr_low_acc, b_low, c_R_b);
            float32x4_t nr_low = vminq_f32(nr_low_acc, max_255);  //Clamping a 255.0f

            //Canal verde nuevo (low)
            float32x4_t ng_low_acc = vmulq_f32(r_low, c_G_r);
            ng_low_acc = vfmaq_f32 (ng_low_acc, g_low, c_G_g);
            ng_low_acc = vfmaq_f32 (ng_low_acc, b_low, c_G_b);
            float32x4_t ng_low = vminq_f32 (ng_low_acc, max_255);

            //Canal Azul nuevo (low)
            float32x4_t nb_low_acc = vmulq_f32 (r_low, c_B_r);
            nb_low_acc = vfmaq_f32 (nb_low_acc, g_low, c_B_g);
            nb_low_acc = vfmaq_f32 (nb_low_acc, b_low, c_B_b);
            float32x4_t nb_low = vminq_f32 (nb_low_acc, max_255);


            //Bloque Alto (Pixeles 4 a 7)
            float32x4_t r_high = vcvtq_f32_u32 (vmovl_u16(vget_high_u16(r_16)));
            float32x4_t g_high = vcvtq_f32_u32 (vmovl_u16(vget_high_u16(g_16)));
            float32x4_t b_high = vcvtq_f32_u32 (vmovl_u16(vget_high_u16(b_16)));

            //Canal Rojo nuevo (high)
            float32x4_t nr_high_acc =  vmulq_f32 (r_high, c_R_r);
            nr_high_acc = vfmaq_f32 (nr_high_acc, g_high, c_R_g);
            nr_high_acc = vfmaq_f32 (nr_high_acc, b_high, c_R_b);
            float32x4_t nr_high = vminq_f32 (nr_high_acc, max_255);


            //Canal Verde nuevo (high)
            float32x4_t ng_high_acc = vmulq_f32 (r_high, c_G_r);
            ng_high_acc = vfmaq_f32 (ng_high_acc, g_high, c_G_g);
            ng_high_acc = vfmaq_f32 (ng_high_acc, b_high, c_G_b);
            float32x4_t ng_high = vminq_f32 (ng_high_acc, max_255);


            //Canal Azul nuevo (high)
            float32x4_t nb_high_acc = vmulq_f32 (r_high, c_B_r);
            nb_high_acc = vfmaq_f32 (nb_high_acc, g_high, c_B_g);
            nb_high_acc = vfmaq_f32 (nb_high_acc, b_high, c_B_b);
            float32x4_t nb_high = vminq_f32 (nb_high_acc, max_255);


            //Empaquetado y escritura de memoria//
            //Canal Rojo (R)
            uint32x4_t r_low_u32 = vcvtq_u32_f32 (nr_low);
            uint32x4_t r_high_u32 = vcvtq_u32_f32 (nr_high);

            //Narrowing u32 -> u16
            uint16x4_t r_low_u16 = vmovn_u32 (r_low_u32);
            uint16x4_t r_high_u16 = vmovn_u32 (r_high_u32);

            //Combinar low y high (8 pixeles)
            uint16x8_t r_combined = vcombine_u16 (r_low_u16, r_high_u16);
            uint8x8_t out_r = vmovn_u16 (r_combined);


            //Canal Verde (G)
            uint32x4_t g_low_u32 = vcvtq_u32_f32 (ng_low);
            uint32x4_t g_high_u32 = vcvtq_u32_f32 (ng_high);

            //Narrowing u32 -> u16
            uint16x4_t g_low_u16 = vmovn_u32 (g_low_u32);
            uint16x4_t g_high_u16 = vmovn_u32 (g_high_u32);

            //Combinar low y high (8 pixeles)
            uint16x8_t g_combined = vcombine_u16 (g_low_u16, g_high_u16);
            uint8x8_t out_g = vmovn_u16 (g_combined);


            //Canal Azul (B)
            uint32x4_t b_low_u32 = vcvtq_u32_f32 (nb_low);
            uint32x4_t b_high_u32 = vcvtq_u32_f32 (nb_high);

            //Narrowing u32 -> u16
            uint16x4_t b_low_u16 = vmovn_u32 (b_low_u32);
            uint16x4_t b_high_u16 = vmovn_u32 (b_high_u32);

            //Combinar low y high (8 pixeles)
            uint16x8_t b_combined = vcombine_u16 (b_low_u16, b_high_u16);
            uint8x8_t out_b = vmovn_u16 (b_combined);


            //Carga devuelta a la RAM
            uint8x8x3_t out_pixels = {out_r, out_g, out_b};
            vst3_u8(dst + (i * 3), out_pixels); 
        }

        //Bucle residual escalar (Sobras del final)//
        for (;i < total_pixels; ++i) {
            std::size_t idx = i * 3;

            //Convercion de uint a float 
            float r = static_cast<float>(src[idx]);
            float g = static_cast<float>(src[idx + 1]);
            float b = static_cast<float>(src[idx + 2]);

            //Coeficientes y matematica del filtro sepia
            float nr = r * 0.393f + g * 0.769f + b * 0.186f;
            float ng = r * 0.349f + g * 0.686f + b * 0.168f;
            float nb = r * 0.272f + g * 0.534f + b * 0.131f;

            //Clamping a 255.0f y guardado en RAM
            dst[idx]     = static_cast<uint8_t>(std::min(255.0f, nr));
            dst[idx + 1] = static_cast<uint8_t>(std::min(255.0f, ng));
            dst[idx + 2] = static_cast<uint8_t>(std::min(255.0f, nb));
        }
    }
}