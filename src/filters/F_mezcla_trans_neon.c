#include <stdint.h>
#include <arm_neon.h>

typedef struct 
{ uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
}PixelRGBA;

void Mezclar_ImagenesC(const PixelRGBA* imgA, const PixelRGBA* imgB, PixelRGBA* imgDest, size_t numPixeles ) {

    for (size_t i = 0; i < numPixeles; i+=4 ) {
    
        //Cargado y empaquetado de los canles 
        uint8x16x4_t vecA = vld4q_u8 ((const uint8_t*)&imgA[i]);
        uint8x16x4_t vecB = vld4q_u8 ((const uint8_t*)&imgB[i]);
     
        //Extracicion del canal Alfa
        uint8x16_t PorcA = vecB.val[3];
        uint8x16_t PorcB = vsubq_u8 (vdupq_n_u8(255), PorcA);


        //Canal Rojo convirtiendo a 16 bits 
        //Multiplicacion Parte A
        uint16x8_t RojoA_low = vmull_u8 (vget_low_u8(vecA.val[0]), vget_low_u8 (PorcA));
        uint16x8_t RojoA_high= vmull_high_u8 (vecA.val[0], PorcA);

        //Multiplicacion Parte B
        uint16x8_t RojoB_low = vmull_u8 (vget_low_u8(vecB.val[0]), vget_low_u8 (PorcB));
        uint16x8_t RojoB_high = vmull_high_u8(vecB.val[0], PorcB);

        //Sumando parte low y high A y B (Devuelta a 8 bits)
        uint8x8_t Rojo_res_low = vshrn_n_u16 (vaddq_u16(RojoA_low, RojoB_low), 8);
        uint8x16_t Rojo_final = vshrn_high_n_u16 (Rojo_res_low, vaddq_u16(RojoA_high, RojoB_high), 8); 


        //Canal verde, onvirtiendo a 16 bits
        //Multiplicacion parte A
        uint16x8_t VerdeA_low = vmull_u8 (vget_low_u8(vecA.val[1]), vget_low_u8 (PorcA));
        uint16x8_t VerdeA_high = vmull_high_u8 (vecA.val[1], PorcA);

        //Multiplicacion parte B
        uint16x8_t VerdeB_low = vmull_u8 (vget_low_u8(vecB.val[1]), vget_low_u8 (PorcB));
        uint16x8_t VerdeB_high = vmull_high_u8 (vecB.val[1], PorcB);

        //Sumando low y high A y B (Devuelta a 8 bits)
        uint8x8_t Verde_res_low = vshrn_n_u16 (vaddq_u16(VerdeA_low, VerdeB_low), 8);
        uint8x16_t Verde_final = vshrn_high_n_u16 (Verde_res_low, vaddq_u16(VerdeA_high, VerdeB_high), 8);


        //Canal azul, convirtiendo a a 16 bits
        //Multiplicacion parte A
        uint16x8_t AzulA_low = vmull_u8 (vget_low_u8(vecA.val[2]), vget_low_u8 (PorcA));
        uint16x8_t AzulA_high = vmull_high_u8 (vecA.val[2], PorcA);
        
        //Multiplicacion parte B
        uint16x8_t AzulB_low = vmull_u8 (vget_low_u8(vecB.val[2]), vget_low_u8 (PorcB));
        uint16x8_t AzulB_high = vmull_high_u8 (vecB.val[2], PorcB);

        //Sumando low y high parte A y B (devuelta a 8 bits)
        uint8x8_t Azul_res_low = vshrn_n_u16 (vaddq_u16(AzulA_low, AzulB_low), 8);
        uint8x16_t Azul_final = vshrn_high_n_u16 (Azul_res_low, vaddq_u16(AzulA_high, AzulB_high), 8);


        //Definiendo canal Alfa (Opaco, 255)
        uint8x16_t Alfa_final = vdupq_n_u8 (255);

        //Agrupacion de los 4 canales procesados
        uint8x16x4_t vec_res;
        vec_res.val[0] = Rojo_final;
        vec_res.val[1] = Verde_final;
        vec_res.val[2] = Azul_final;
        vec_res.val[3] = Alfa_final;

        //Devuealta a imgDest con los 16 pixeles modificados
        vst4q_u8 ((uint8_t*)&imgDest[i], vec_res);

    }
}