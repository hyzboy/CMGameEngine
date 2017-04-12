/*
    Real-Time YCoCg DXT Compression (SSE2)
    Copyright (C) 2007 Id Software, Inc.
    Written by J.M.P. van Waveren

    This code is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
*/

#define ALIGN16( x )                __declspec(align(16)) x
#define R_SHUFFLE_D( x, y, z, w )   (( (w) & 3 ) << 6 | ( (z) & 3 ) << 4 | ( (y) & 3 ) << 2 | ( (x) & 3 ))

ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask0[4] ) = { 7<<0, 0, 7<<0, 0 };
ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask1[4] ) = { 7<<3, 0, 7<<3, 0 };
ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask2[4] ) = { 7<<6, 0, 7<<6, 0 };
ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask3[4] ) = { 7<<9, 0, 7<<9, 0 };
ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask4[4] ) = { 7<<12, 0, 7<<12, 0 };
ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask5[4] ) = { 7<<15, 0, 7<<15, 0 };
ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask6[4] ) = { 7<<18, 0, 7<<18, 0 };
ALIGN16( static dword SIMD_SSE2_dword_alpha_bit_mask7[4] ) = { 7<<21, 0, 7<<21, 0 };
ALIGN16( static word SIMD_SSE2_word_0[8] ) = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
ALIGN16( static word SIMD_SSE2_word_1[8] ) = { 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001 };
ALIGN16( static word SIMD_SSE2_word_2[8] ) = { 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002 };
ALIGN16( static word SIMD_SSE2_word_31[8] ) = { 31, 31, 31, 31, 31, 31, 31, 31 };
ALIGN16( static word SIMD_SSE2_word_63[8] ) = { 63, 63, 63, 63, 63, 63, 63, 63 };
ALIGN16( static word SIMD_SSE2_word_center_128[8] ) = { 128, 128, 0, 0, 0, 0, 0, 0 };
ALIGN16( static word SIMD_SSE2_word_div_by_3[8] ) = { (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1 };
ALIGN16( static word SIMD_SSE2_word_div_by_7[8] ) = { (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1 };
ALIGN16( static word SIMD_SSE2_word_div_by_14[8] ) = { (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1 };
ALIGN16( static word SIMD_SSE2_word_scale66554400[8] ) = { 6, 6, 5, 5, 4, 4, 0, 0 };
ALIGN16( static word SIMD_SSE2_word_scale11223300[8] ) = { 1, 1, 2, 2, 3, 3, 0, 0 };
ALIGN16( static word SIMD_SSE2_word_insetShiftUp[8] ) = { 1 << INSET_COLOR_SHIFT, 1 << INSET_COLOR_SHIFT, 1 << INSET_COLOR_SHIFT, 1 << INSET_ALPHA_SHIFT, 0, 0, 0, 0 };
ALIGN16( static word SIMD_SSE2_word_insetShiftDown[8] ) = { 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_ALPHA_SHIFT ), 0, 0, 0, 0 };
ALIGN16( static word SIMD_SSE2_word_insetYCoCgRound[8] ) = { ((1<<(INSET_COLOR_SHIFT-1))-1), ((1<<(INSET_COLOR_SHIFT-1))-1), ((1<<(INSET_COLOR_SHIFT-1))-1), ((1<<(INSET_ALPHA_SHIFT-1))-1), 0, 0, 0, 0 };
ALIGN16( static word SIMD_SSE2_word_insetYCoCgMask[8] ) = { 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF };
ALIGN16( static word SIMD_SSE2_word_inset565Mask[8] ) = { C565_5_MASK, C565_6_MASK, C565_5_MASK, 0xFF, C565_5_MASK, C565_6_MASK, C565_5_MASK, 0xFF };
ALIGN16( static word SIMD_SSE2_word_inset565Rep[8] ) = { 1 << ( 16 - 5 ), 1 << ( 16 - 6 ), 1 << ( 16 - 5 ), 0, 1 << ( 16 - 5 ), 1 << ( 16 - 6 ), 1 << ( 16 - 5 ), 0 };
ALIGN16( static byte SIMD_SSE2_byte_0[16] ) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_SSE2_byte_1[16] ) = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
ALIGN16( static byte SIMD_SSE2_byte_2[16] ) = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
ALIGN16( static byte SIMD_SSE2_byte_7[16] ) = { 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07 };
ALIGN16( static byte SIMD_SSE2_byte_8[16] ) = { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 };
ALIGN16( static byte SIMD_SSE2_byte_colorMask[16] ) = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_SSE2_byte_diagonalMask[16] ) = { 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_SSE2_byte_scale_mask0[16] ) = { 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF };
ALIGN16( static byte SIMD_SSE2_byte_scale_mask1[16] ) = { 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_SSE2_byte_scale_mask2[16] ) = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00 };
ALIGN16( static byte SIMD_SSE2_byte_scale_mask3[16] ) = { 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_SSE2_byte_scale_mask4[16] ) = { 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_SSE2_byte_minus_128_0[16] ) = { -128, -128, 0, 0, -128, -128, 0, 0, -128, -128, 0, 0, -128, -128, 0, 0 };

void ExtractBlock_SSE2( const byte *inPtr, int width, byte *colorBlock ) {
    __asm {
        mov         esi, inPtr
        mov         edi, colorBlock
        mov         eax, width
        shl         eax, 2
        movdqa      xmm0, [esi]
        movdqa      xmmword ptr [edi+ 0], xmm0
        movdqa      xmm1, xmmword ptr [esi+eax]
        movdqa      xmmword ptr [edi+16], xmm1
        movdqa      xmm2, xmmword ptr [esi+eax*2]
        add         esi, eax
        movdqa      xmmword ptr [edi+32], xmm2
        movdqa      xmm3, xmmword ptr [esi+eax*2]
        movdqa      xmmword ptr [edi+48], xmm3
    }
}

void GetMinMaxYCoCg_SSE2( const byte *colorBlock, byte *minColor, byte *maxColor ) {
    __asm {
        mov         eax, colorBlock
        mov         esi, minColor
        mov         edi, maxColor
        movdqa      xmm0, xmmword ptr [eax+ 0]
        movdqa      xmm1, xmmword ptr [eax+ 0]
        pminub      xmm0, xmmword ptr [eax+16]
        pmaxub      xmm1, xmmword ptr [eax+16]
        pminub      xmm0, xmmword ptr [eax+32]
        pmaxub      xmm1, xmmword ptr [eax+32]
        pminub      xmm0, xmmword ptr [eax+48]
        pmaxub      xmm1, xmmword ptr [eax+48]
        pshufd      xmm3, xmm0, R_SHUFFLE_D( 2, 3, 2, 3 )
        pshufd      xmm4, xmm1, R_SHUFFLE_D( 2, 3, 2, 3 )
        pminub      xmm0, xmm3
        pmaxub      xmm1, xmm4
        pshuflw     xmm6, xmm0, R_SHUFFLE_D( 2, 3, 2, 3 )
        pshuflw     xmm7, xmm1, R_SHUFFLE_D( 2, 3, 2, 3 )
        pminub      xmm0, xmm6
        pmaxub      xmm1, xmm7
        movd        dword ptr [esi], xmm0
        movd        dword ptr [edi], xmm1
    }
}

void ScaleYCoCg_SSE2( byte *colorBlock, byte *minColor, byte *maxColor ) {
    __asm {
        mov         esi, colorBlock
        mov         edx, minColor
        mov         ecx, maxColor

        movd        xmm0, dword ptr [edx]
        movd        xmm1, dword ptr [ecx]

        punpcklbw   xmm0, SIMD_SSE2_byte_0
        punpcklbw   xmm1, SIMD_SSE2_byte_0

        movdqa      xmm6, SIMD_SSE2_word_center_128
        movdqa      xmm7, SIMD_SSE2_word_center_128

        psubw       xmm6, xmm0
        psubw       xmm7, xmm1

        psubw       xmm0, SIMD_SSE2_word_center_128
        psubw       xmm1, SIMD_SSE2_word_center_128

        pmaxsw      xmm6, xmm0
        pmaxsw      xmm7, xmm1

        pmaxsw      xmm6, xmm7
        pshuflw     xmm7, xmm6, R_SHUFFLE_D( 1, 0, 1, 0 )
        pmaxsw      xmm6, xmm7
        pshufd      xmm6, xmm6, R_SHUFFLE_D( 0, 0, 0, 0 )

        movdqa      xmm7, xmm6
        pcmpgtw     xmm6, SIMD_SSE2_word_63
        pcmpgtw     xmm7, SIMD_SSE2_word_31

        pandn       xmm7, SIMD_SSE2_byte_2
        por         xmm7, SIMD_SSE2_byte_1
        pandn       xmm6, xmm7
        movdqa      xmm3, xmm6
        movdqa      xmm7, xmm6
        pxor        xmm7, SIMD_SSE2_byte_not
        por         xmm7, SIMD_SSE2_byte_scale_mask0
        paddw       xmm6, SIMD_SSE2_byte_1
        pand        xmm6, SIMD_SSE2_byte_scale_mask1
        por         xmm6, SIMD_SSE2_byte_scale_mask2

        movd        xmm4, dword ptr [edx]
        movd        xmm5, dword ptr [ecx]

        pand        xmm4, SIMD_SSE2_byte_scale_mask3
        pand        xmm5, SIMD_SSE2_byte_scale_mask3

        pslld       xmm3, 3
        pand        xmm3, SIMD_SSE2_byte_scale_mask4

        por         xmm4, xmm3
        por         xmm5, xmm3

        paddb       xmm4, SIMD_SSE2_byte_minus_128_0
        paddb       xmm5, SIMD_SSE2_byte_minus_128_0

        pmullw      xmm4, xmm6
        pmullw      xmm5, xmm6

        pand        xmm4, xmm7
        pand        xmm5, xmm7

        psubb       xmm4, SIMD_SSE2_byte_minus_128_0
        psubb       xmm5, SIMD_SSE2_byte_minus_128_0

        movd        dword ptr [edx], xmm4
        movd        dword ptr [ecx], xmm5

        movdqa      xmm0, xmmword ptr [esi+ 0*4]
        movdqa      xmm1, xmmword ptr [esi+ 4*4]
        movdqa      xmm2, xmmword ptr [esi+ 8*4]
        movdqa      xmm3, xmmword ptr [esi+12*4]

        paddb       xmm0, SIMD_SSE2_byte_minus_128_0
        paddb       xmm1, SIMD_SSE2_byte_minus_128_0
        paddb       xmm2, SIMD_SSE2_byte_minus_128_0
        paddb       xmm3, SIMD_SSE2_byte_minus_128_0

        pmullw      xmm0, xmm6
        pmullw      xmm1, xmm6
        pmullw      xmm2, xmm6
        pmullw      xmm3, xmm6

        pand        xmm0, xmm7
        pand        xmm1, xmm7
        pand        xmm2, xmm7
        pand        xmm3, xmm7

        psubb       xmm0, SIMD_SSE2_byte_minus_128_0
        psubb       xmm1, SIMD_SSE2_byte_minus_128_0
        psubb       xmm2, SIMD_SSE2_byte_minus_128_0
        psubb       xmm3, SIMD_SSE2_byte_minus_128_0

        movdqa      xmmword ptr [esi+ 0*4], xmm0
        movdqa      xmmword ptr [esi+ 4*4], xmm1
        movdqa      xmmword ptr [esi+ 8*4], xmm2
        movdqa      xmmword ptr [esi+12*4], xmm3
    }
}

void InsetYCoCgBBox_SSE2( byte *minColor, byte *maxColor ) {
    __asm {
        mov         esi, minColor
        mov         edi, maxColor
        movd        xmm0, dword ptr [esi]
        movd        xmm1, dword ptr [edi]
        punpcklbw   xmm0, SIMD_SSE2_byte_0
        punpcklbw   xmm1, SIMD_SSE2_byte_0
        movdqa      xmm2, xmm1
        psubw       xmm2, xmm0
        psubw       xmm2, SIMD_SSE2_word_insetYCoCgRound
        pand        xmm2, SIMD_SSE2_word_insetYCoCgMask
        pmullw      xmm0, SIMD_SSE2_word_insetShiftUp
        pmullw      xmm1, SIMD_SSE2_word_insetShiftUp
        paddw       xmm0, xmm2
        psubw       xmm1, xmm2
        pmulhw      xmm0, SIMD_SSE2_word_insetShiftDown
        pmulhw      xmm1, SIMD_SSE2_word_insetShiftDown
        pmaxsw      xmm0, SIMD_SSE2_word_0
        pmaxsw      xmm1, SIMD_SSE2_word_0
        pand        xmm0, SIMD_SSE2_word_inset565Mask
        pand        xmm1, SIMD_SSE2_word_inset565Mask
        movdqa      xmm2, xmm0
        movdqa      xmm3, xmm1
        pmulhw      xmm2, SIMD_SSE2_word_inset565Rep
        pmulhw      xmm3, SIMD_SSE2_word_inset565Rep
        por         xmm0, xmm2
        por         xmm1, xmm3
        packuswb    xmm0, xmm0
        packuswb    xmm1, xmm1
        movd        dword ptr [esi], xmm0
        movd        dword ptr [edi], xmm1
    }
}

void SelectYCoCgDiagonal_SSE2( const byte *colorBlock, byte *minColor, byte *maxColor ) {
    __asm {
        mov         esi, colorBlock
        mov         edx, minColor
        mov         ecx, maxColor

        movdqa      xmm0, xmmword ptr [esi+ 0]
        movdqa      xmm1, xmmword ptr [esi+16]
        movdqa      xmm2, xmmword ptr [esi+32]
        movdqa      xmm3, xmmword ptr [esi+48]

        pand        xmm0, SIMD_SSE2_dword_word_mask
        pand        xmm1, SIMD_SSE2_dword_word_mask
        pand        xmm2, SIMD_SSE2_dword_word_mask
        pand        xmm3, SIMD_SSE2_dword_word_mask

        pslldq      xmm1, 2
        pslldq      xmm3, 2
        por         xmm0, xmm1
        por         xmm2, xmm3

        movd        xmm1, dword ptr [edx]
        movd        xmm3, dword ptr [ecx]

        movdqa      xmm6, xmm1
        movdqa      xmm7, xmm3

        pavgb       xmm1, xmm3
        pshuflw     xmm1, xmm1, R_SHUFFLE_D( 0, 0, 0, 0 )
        pshufd      xmm1, xmm1, R_SHUFFLE_D( 0, 0, 0, 0 )
        movdqa      xmm3, xmm1

        pmaxub      xmm1, xmm0
        pmaxub      xmm3, xmm2
        pcmpeqb     xmm1, xmm0
        pcmpeqb     xmm3, xmm2

        movdqa      xmm0, xmm1
        movdqa      xmm2, xmm3
        psrldq      xmm0, 1
        psrldq      xmm2, 1

        pxor        xmm0, xmm1
        pxor        xmm2, xmm3
        pand        xmm0, SIMD_SSE2_word_1
        pand        xmm2, SIMD_SSE2_word_1

        paddw       xmm0, xmm2
        psadbw      xmm0, SIMD_SSE2_byte_0
        pshufd      xmm1, xmm0, R_SHUFFLE_D( 2, 3, 0, 1 )

#ifdef NVIDIA_7X_HARDWARE_BUG_FIX
        paddw       xmm1, xmm0
        pcmpgtw     xmm1, SIMD_SSE2_word_8
        pand        xmm1, SIMD_SSE2_byte_diagonalMask
        movdqa      xmm0, xmm6
        pcmpeqb     xmm0, xmm7
        pslldq      xmm0, 1
        pandn       xmm0, xmm1
#else
        paddw       xmm0, xmm1
        pcmpgtw     xmm0, SIMD_SSE2_word_8
        pand        xmm0, SIMD_SSE2_byte_diagonalMask
#endif

        pxor        xmm6, xmm7
        pand        xmm0, xmm6
        pxor        xmm7, xmm0
        pxor        xmm6, xmm7

        movd        dword ptr [edx], xmm6
        movd        dword ptr [ecx], xmm7
    }
}

void EmitAlphaIndices_SSE2( const byte *colorBlock, const byte minAlpha, const byte maxAlpha ) {
    __asm {
        mov         esi, colorBlock
        movdqa      xmm0, xmmword ptr [esi+ 0]
        movdqa      xmm5, xmmword ptr [esi+16]
        psrld       xmm0, 24
        psrld       xmm5, 24
        packuswb    xmm0, xmm5

        movdqa      xmm6, xmmword ptr [esi+32]
        movdqa      xmm4, xmmword ptr [esi+48]
        psrld       xmm6, 24
        psrld       xmm4, 24
        packuswb    xmm6, xmm4

        movzx       ecx, maxAlpha
        movd        xmm5, ecx
        pshuflw     xmm5, xmm5, R_SHUFFLE_D( 0, 0, 0, 0 )
        pshufd      xmm5, xmm5, R_SHUFFLE_D( 0, 0, 0, 0 )
        movdqa      xmm7, xmm5

        movzx       edx, minAlpha
        movd        xmm2, edx
        pshuflw     xmm2, xmm2, R_SHUFFLE_D( 0, 0, 0, 0 )
        pshufd      xmm2, xmm2, R_SHUFFLE_D( 0, 0, 0, 0 )
        movdqa      xmm3, xmm2

        movdqa      xmm4, xmm5
        psubw       xmm4, xmm2
        pmulhw      xmm4, SIMD_SSE2_word_div_by_14

        movdqa      xmm1, xmm2
        paddw       xmm1, xmm4
        packuswb    xmm1, xmm1

        pmullw      xmm5, SIMD_SSE2_word_scale66554400
        pmullw      xmm7, SIMD_SSE2_word_scale11223300
        pmullw      xmm2, SIMD_SSE2_word_scale11223300
        pmullw      xmm3, SIMD_SSE2_word_scale66554400
        paddw       xmm5, xmm2
        paddw       xmm7, xmm3
        pmulhw      xmm5, SIMD_SSE2_word_div_by_7
        pmulhw      xmm7, SIMD_SSE2_word_div_by_7
        paddw       xmm5, xmm4
        paddw       xmm7, xmm4

        pshufd      xmm2, xmm5, R_SHUFFLE_D( 0, 0, 0, 0 )
        pshufd      xmm3, xmm5, R_SHUFFLE_D( 1, 1, 1, 1 )
        pshufd      xmm4, xmm5, R_SHUFFLE_D( 2, 2, 2, 2 )
        packuswb    xmm2, xmm2
        packuswb    xmm3, xmm3
        packuswb    xmm4, xmm4

        packuswb    xmm0, xmm6

        pshufd      xmm5, xmm7, R_SHUFFLE_D( 2, 2, 2, 2 )
        pshufd      xmm6, xmm7, R_SHUFFLE_D( 1, 1, 1, 1 )
        pshufd      xmm7, xmm7, R_SHUFFLE_D( 0, 0, 0, 0 )
        packuswb    xmm5, xmm5
        packuswb    xmm6, xmm6
        packuswb    xmm7, xmm7

        pminub      xmm1, xmm0
        pminub      xmm2, xmm0
        pminub      xmm3, xmm0
        pcmpeqb     xmm1, xmm0
        pcmpeqb     xmm2, xmm0
        pcmpeqb     xmm3, xmm0
        pminub      xmm4, xmm0
        pminub      xmm5, xmm0
        pminub      xmm6, xmm0
        pminub      xmm7, xmm0
        pcmpeqb     xmm4, xmm0
        pcmpeqb     xmm5, xmm0
        pcmpeqb     xmm6, xmm0
        pcmpeqb     xmm7, xmm0
        pand        xmm1, SIMD_SSE2_byte_1
        pand        xmm2, SIMD_SSE2_byte_1
        pand        xmm3, SIMD_SSE2_byte_1
        pand        xmm4, SIMD_SSE2_byte_1
        pand        xmm5, SIMD_SSE2_byte_1
        pand        xmm6, SIMD_SSE2_byte_1
        pand        xmm7, SIMD_SSE2_byte_1
        movdqa      xmm0, SIMD_SSE2_byte_1
        paddusb     xmm0, xmm1
        paddusb     xmm2, xmm3
        paddusb     xmm4, xmm5
        paddusb     xmm6, xmm7
        paddusb     xmm0, xmm2
        paddusb     xmm4, xmm6
        paddusb     xmm0, xmm4
        pand        xmm0, SIMD_SSE2_byte_7
        movdqa      xmm1, SIMD_SSE2_byte_2
        pcmpgtb     xmm1, xmm0
        pand        xmm1, SIMD_SSE2_byte_1
        pxor        xmm0, xmm1
        movdqa      xmm1, xmm0
        movdqa      xmm2, xmm0
        movdqa      xmm3, xmm0
        movdqa      xmm4, xmm0
        movdqa      xmm5, xmm0
        movdqa      xmm6, xmm0
        movdqa      xmm7, xmm0
        psrlq       xmm1,  8- 3
        psrlq       xmm2, 16- 6
        psrlq       xmm3, 24- 9
        psrlq       xmm4, 32-12
        psrlq       xmm5, 40-15
        psrlq       xmm6, 48-18
        psrlq       xmm7, 56-21
        pand        xmm0, SIMD_SSE2_dword_alpha_bit_mask0
        pand        xmm1, SIMD_SSE2_dword_alpha_bit_mask1
        pand        xmm2, SIMD_SSE2_dword_alpha_bit_mask2
        pand        xmm3, SIMD_SSE2_dword_alpha_bit_mask3
        pand        xmm4, SIMD_SSE2_dword_alpha_bit_mask4
        pand        xmm5, SIMD_SSE2_dword_alpha_bit_mask5
        pand        xmm6, SIMD_SSE2_dword_alpha_bit_mask6
        pand        xmm7, SIMD_SSE2_dword_alpha_bit_mask7
        por         xmm0, xmm1
        por         xmm2, xmm3
        por         xmm4, xmm5
        por         xmm6, xmm7
        por         xmm0, xmm2
        por         xmm4, xmm6
        por         xmm0, xmm4
        mov         esi, globalOutData
        movd        dword ptr [esi+0], xmm0
        pshufd      xmm1, xmm0, R_SHUFFLE_D( 2, 3, 0, 1 )
        movd        dword ptr [esi+3], xmm1
    }

    globalOutData += 6;
}

void EmitColorIndices_SSE2( const byte *colorBlock, const byte *minColor, const byte *maxColor ) {

    ALIGN16( byte color0[16] );
    ALIGN16( byte color1[16] );
    ALIGN16( byte color2[16] );
    ALIGN16( byte color3[16] );
    ALIGN16( byte result[16] );

    __asm {
        mov         esi, maxColor
        mov         edi, minColor
        pxor        xmm7, xmm7
        movdqa      result, xmm7

        movd        xmm0, [esi]
        pand        xmm0, SIMD_SSE2_byte_colorMask
        pshufd      xmm0, xmm0, R_SHUFFLE_D( 0, 1, 0, 1 )
        movdqa      color0, xmm0

        movd        xmm1, [edi]
        pand        xmm1, SIMD_SSE2_byte_colorMask
        pshufd      xmm1, xmm1, R_SHUFFLE_D( 0, 1, 0, 1 )
        movdqa      color1, xmm1

        punpcklbw   xmm0, xmm7
        punpcklbw   xmm1, xmm7

        movdqa      xmm6, xmm1
        paddw       xmm1, xmm0
        paddw       xmm0, xmm1
        pmulhw      xmm0, SIMD_SSE2_word_div_by_3
        packuswb    xmm0, xmm7
        pshufd      xmm0, xmm0, R_SHUFFLE_D( 0, 1, 0, 1 )
        movdqa      color2, xmm0

        paddw       xmm1, xmm6
        pmulhw      xmm1, SIMD_SSE2_word_div_by_3
        packuswb    xmm1, xmm7
        pshufd      xmm1, xmm1, R_SHUFFLE_D( 0, 1, 0, 1 )
        movdqa      color3, xmm1

        mov         eax, 32
        mov         esi, colorBlock

    loop1:          // iterates 2 times
        movq        xmm3, qword ptr [esi+eax+0]
        pshufd      xmm3, xmm3, R_SHUFFLE_D( 0, 2, 1, 3 )
        movq        xmm5, qword ptr [esi+eax+8]
        pshufd      xmm5, xmm5, R_SHUFFLE_D( 0, 2, 1, 3 )

        movdqa      xmm0, xmm3
        movdqa      xmm6, xmm5
        psadbw      xmm0, color0
        psadbw      xmm6, color0
        packssdw    xmm0, xmm6
        movdqa      xmm1, xmm3
        movdqa      xmm6, xmm5
        psadbw      xmm1, color1
        psadbw      xmm6, color1
        packssdw    xmm1, xmm6
        movdqa      xmm2, xmm3
        movdqa      xmm6, xmm5
        psadbw      xmm2, color2
        psadbw      xmm6, color2
        packssdw    xmm2, xmm6
        psadbw      xmm3, color3
        psadbw      xmm5, color3
        packssdw    xmm3, xmm5

        movq        xmm4, qword ptr [esi+eax+16]
        pshufd      xmm4, xmm4, R_SHUFFLE_D( 0, 2, 1, 3 )
        movq        xmm5, qword ptr [esi+eax+24]
        pshufd      xmm5, xmm5, R_SHUFFLE_D( 0, 2, 1, 3 )

        movdqa      xmm6, xmm4
        movdqa      xmm7, xmm5
        psadbw      xmm6, color0
        psadbw      xmm7, color0
        packssdw    xmm6, xmm7
        packssdw    xmm0, xmm6
        movdqa      xmm6, xmm4
        movdqa      xmm7, xmm5
        psadbw      xmm6, color1
        psadbw      xmm7, color1
        packssdw    xmm6, xmm7
        packssdw    xmm1, xmm6
        movdqa      xmm6, xmm4
        movdqa      xmm7, xmm5
        psadbw      xmm6, color2
        psadbw      xmm7, color2
        packssdw    xmm6, xmm7
        packssdw    xmm2, xmm6
        psadbw      xmm4, color3
        psadbw      xmm5, color3
        packssdw    xmm4, xmm5
        packssdw    xmm3, xmm4

        movdqa      xmm7, result
        pslld       xmm7, 16

        movdqa      xmm4, xmm0
        movdqa      xmm5, xmm1
        pcmpgtw     xmm0, xmm3
        pcmpgtw     xmm1, xmm2
        pcmpgtw     xmm4, xmm2
        pcmpgtw     xmm5, xmm3
        pcmpgtw     xmm2, xmm3
        pand        xmm4, xmm1
        pand        xmm5, xmm0
        pand        xmm2, xmm0
        por         xmm4, xmm5
        pand        xmm2, SIMD_SSE2_word_1
        pand        xmm4, SIMD_SSE2_word_2
        por         xmm2, xmm4

        pshufd      xmm5, xmm2, R_SHUFFLE_D( 2, 3, 0, 1 )
        punpcklwd   xmm2, SIMD_SSE2_word_0
        punpcklwd   xmm5, SIMD_SSE2_word_0
        pslld       xmm5, 8
        por         xmm7, xmm5
        por         xmm7, xmm2
        movdqa      result, xmm7

        sub         eax, 32
        jge         loop1

        mov         esi, globalOutData
        pshufd      xmm4, xmm7, R_SHUFFLE_D( 1, 2, 3, 0 )
        pshufd      xmm5, xmm7, R_SHUFFLE_D( 2, 3, 0, 1 )
        pshufd      xmm6, xmm7, R_SHUFFLE_D( 3, 0, 1, 2 )
        pslld       xmm4, 2
        pslld       xmm5, 4
        pslld       xmm6, 6
        por         xmm7, xmm4
        por         xmm7, xmm5
        por         xmm7, xmm6
        movd        dword ptr [esi], xmm7
    }

    globalOutData += 4;
}

bool CompressYCoCgDXT5_SSE2( const byte *inBuf, byte *outBuf, int width, int height, int &outputBytes ) {
    ALIGN16( byte block[64] );
    ALIGN16( byte minColor[4] );
    ALIGN16( byte maxColor[4] );

    globalOutData = outBuf;

    for ( int j = 0; j < height; j += 4, inBuf += width * 4*4 ) {
        for ( int i = 0; i < width; i += 4 ) {

            ExtractBlock_SSE2( inBuf + i * 4, width, block );

            GetMinMaxYCoCg_SSE2( block, minColor, maxColor );
            ScaleYCoCg_SSE2( block, minColor, maxColor );
            InsetYCoCgBBox_SSE2( minColor, maxColor );
            SelectYCoCgDiagonal_SSE2( block, minColor, maxColor );

            EmitByte( maxColor[3] );
            EmitByte( minColor[3] );

            EmitAlphaIndices_SSE2( block, minColor[3], maxColor[3] );

            EmitUShort( ColorTo565( maxColor ) );
            EmitUShort( ColorTo565( minColor ) );

            EmitColorIndices_SSE2( block, minColor, maxColor );
        }
    }

    outputBytes = globalOutData - outBuf;

    return true;
}
