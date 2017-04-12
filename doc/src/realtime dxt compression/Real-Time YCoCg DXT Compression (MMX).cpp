/*
    Real-Time YCoCg DXT Compression (MMX)
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

ALIGN16( static dword SIMD_MMX_dword_word_mask[2] ) = { 0x0000FFFF, 0x0000FFFF };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask0[2] ) = { 7<<0, 0 };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask1[2] ) = { 7<<3, 0 };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask2[2] ) = { 7<<6, 0 };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask3[2] ) = { 7<<9, 0 };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask4[2] ) = { 7<<12, 0 };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask5[2] ) = { 7<<15, 0 };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask6[2] ) = { 7<<18, 0 };
ALIGN16( static dword SIMD_MMX_dword_alpha_bit_mask7[2] ) = { 7<<21, 0 };
ALIGN16( static word SIMD_MMX_word_0[4] ) = { 0x0000, 0x0000, 0x0000, 0x0000 };
ALIGN16( static word SIMD_MMX_word_1[4] ) = { 0x0001, 0x0001, 0x0001, 0x0001 };
ALIGN16( static word SIMD_MMX_word_2[4] ) = { 0x0002, 0x0002, 0x0002, 0x0002 };
ALIGN16( static word SIMD_MMX_word_31[4] ) = { 31, 31, 31, 31 };
ALIGN16( static word SIMD_MMX_word_63[4] ) = { 63, 63, 63, 63 };
ALIGN16( static word SIMD_MMX_word_center_128[4] ) = { 128, 128, 0, 0 };
ALIGN16( static word SIMD_MMX_word_div_by_3[4] ) = { (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1, (1<<16)/3+1 };
ALIGN16( static word SIMD_MMX_word_div_by_7[4] ) = { (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1, (1<<16)/7+1 };
ALIGN16( static word SIMD_MMX_word_div_by_14[4] ) = { (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1, (1<<16)/14+1 };
ALIGN16( static word SIMD_MMX_word_scale654[4] ) = { 6, 5, 4, 0 };
ALIGN16( static word SIMD_MMX_word_scale123[4] ) = { 1, 2, 3, 0 };
ALIGN16( static word SIMD_MMX_word_insetShift[4] ) = { 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_ALPHA_SHIFT ) };
ALIGN16( static word SIMD_MMX_word_insetShiftUp[4] ) = { 1 << INSET_COLOR_SHIFT, 1 << INSET_COLOR_SHIFT, 1 << INSET_COLOR_SHIFT, 1 << INSET_ALPHA_SHIFT };
ALIGN16( static word SIMD_MMX_word_insetShiftDown[4] ) = { 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_COLOR_SHIFT ), 1 << ( 16 - INSET_ALPHA_SHIFT ) };
ALIGN16( static word SIMD_MMX_word_insetYCoCgRound[4] ) = { ((1<<(INSET_COLOR_SHIFT-1))-1), ((1<<(INSET_COLOR_SHIFT-1))-1), ((1<<(INSET_COLOR_SHIFT-1))-1), ((1<<(INSET_ALPHA_SHIFT-1))-1) };
ALIGN16( static word SIMD_MMX_word_insetYCoCgMask[4] ) = { 0xFFFF, 0xFFFF, 0x0000, 0xFFFF };
ALIGN16( static word SIMD_MMX_word_inset565Mask[4] ) = { C565_5_MASK, C565_6_MASK, C565_5_MASK, 0xFF };
ALIGN16( static word SIMD_MMX_word_inset565Rep[4] ) = { 1 << ( 16 - 5 ), 1 << ( 16 - 6 ), 1 << ( 16 - 5 ), 0 };
ALIGN16( static byte SIMD_MMX_byte_0[8] ) = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_MMX_byte_1[8] ) = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
ALIGN16( static byte SIMD_MMX_byte_2[8] ) = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
ALIGN16( static byte SIMD_MMX_byte_7[8] ) = { 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07 };
ALIGN16( static byte SIMD_MMX_byte_8[8] ) = { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 };
ALIGN16( static byte SIMD_MMX_byte_not[8] ) = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
ALIGN16( static byte SIMD_MMX_byte_colorMask[8] ) = { 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_MMX_byte_diagonalMask[8] ) = { 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_MMX_byte_scale_mask0[8] ) = { 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF };
ALIGN16( static byte SIMD_MMX_byte_scale_mask1[8] ) = { 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_MMX_byte_scale_mask2[8] ) = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00 };
ALIGN16( static byte SIMD_MMX_byte_scale_mask3[8] ) = { 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_MMX_byte_scale_mask4[8] ) = { 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 };
ALIGN16( static byte SIMD_MMX_byte_minus_128_0[8] ) = { -128, -128, 0, 0, -128, -128, 0, 0 };

void ExtractBlock_MMX( const byte *inPtr, int width, byte *colorBlock ) {
    __asm {
        mov         esi, inPtr
        mov         edi, colorBlock
        mov         eax, width
        shl         eax, 2
        movq        mm0, qword ptr [esi+0]
        movq        qword ptr [edi+ 0], mm0
        movq        mm1, qword ptr [esi+8]
        movq        qword ptr [edi+ 8], mm1
        movq        mm2, qword ptr [esi+eax+0]
        movq        qword ptr [edi+16], mm2
        movq        mm3, qword ptr [esi+eax+8]
        movq        qword ptr [edi+24], mm3
        movq        mm4, qword ptr [esi+eax*2+0]
        movq        qword ptr [edi+32], mm4
        movq        mm5, qword ptr [esi+eax*2+8]
        add         esi, eax
        movq        qword ptr [edi+40], mm5
        movq        mm6, qword ptr [esi+eax*2+0]
        movq        qword ptr [edi+48], mm6
        movq        mm7, qword ptr [esi+eax*2+8]
        movq        qword ptr [edi+56], mm7
        emms
    }
}

void GetMinMaxYCoCg_MMX( const byte *colorBlock, byte *minColor, byte *maxColor ) {
    __asm {
        mov         eax, colorBlock
        mov         esi, minColor
        mov         edi, maxColor
        pshufw      mm0, qword ptr [eax+ 0], R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm1, qword ptr [eax+ 0], R_SHUFFLE_D( 0, 1, 2, 3 )
        pminub      mm0, qword ptr [eax+ 8]
        pmaxub      mm1, qword ptr [eax+ 8]
        pminub      mm0, qword ptr [eax+16]
        pmaxub      mm1, qword ptr [eax+16]
        pminub      mm0, qword ptr [eax+24]
        pmaxub      mm1, qword ptr [eax+24]
        pminub      mm0, qword ptr [eax+32]
        pmaxub      mm1, qword ptr [eax+32]
        pminub      mm0, qword ptr [eax+40]
        pmaxub      mm1, qword ptr [eax+40]
        pminub      mm0, qword ptr [eax+48]
        pmaxub      mm1, qword ptr [eax+48]
        pminub      mm0, qword ptr [eax+56]
        pmaxub      mm1, qword ptr [eax+56]
        pshufw      mm6, mm0, R_SHUFFLE_D( 2, 3, 2, 3 )
        pshufw      mm7, mm1, R_SHUFFLE_D( 2, 3, 2, 3 )
        pminub      mm0, mm6
        pmaxub      mm1, mm7
        movd        dword ptr [esi], mm0
        movd        dword ptr [edi], mm1
        emms
    }
}

void ScaleYCoCg_MMX( byte *colorBlock, byte *minColor, byte *maxColor ) {
    __asm {
        mov         esi, colorBlock
        mov         edx, minColor
        mov         ecx, maxColor

        movd        mm0, dword ptr [edx]
        movd        mm1, dword ptr [ecx]

        punpcklbw   mm0, SIMD_MMX_byte_0
        punpcklbw   mm1, SIMD_MMX_byte_0

        movq        mm6, SIMD_MMX_word_center_128
        movq        mm7, SIMD_MMX_word_center_128

        psubw       mm6, mm0
        psubw       mm7, mm1

        psubw       mm0, SIMD_MMX_word_center_128
        psubw       mm1, SIMD_MMX_word_center_128

        pmaxsw      mm6, mm0
        pmaxsw      mm7, mm1

        pmaxsw      mm6, mm7
        pshufw      mm7, mm6, R_SHUFFLE_D( 1, 0, 1, 0 )
        pmaxsw      mm6, mm7
        pshufw      mm6, mm6, R_SHUFFLE_D( 0, 1, 0, 1 )

        movq        mm7, mm6
        pcmpgtw     mm6, SIMD_MMX_word_63
        pcmpgtw     mm7, SIMD_MMX_word_32

        pandn       mm7, SIMD_MMX_byte_2
        por         mm7, SIMD_MMX_byte_1
        pandn       mm6, mm7
        movq        mm3, mm6
        movq        mm7, mm6
        pxor        mm7, SIMD_MMX_byte_not
        por         mm7, SIMD_MMX_byte_scale_mask0
        paddw       mm6, SIMD_MMX_byte_1
        pand        mm6, SIMD_MMX_byte_scale_mask1
        por         mm6, SIMD_MMX_byte_scale_mask2

        movd        mm4, dword ptr [edx]
        movd        mm5, dword ptr [ecx]

        pand        mm4, SIMD_MMX_byte_scale_mask3
        pand        mm5, SIMD_MMX_byte_scale_mask3

        pslld       mm3, 3
        pand        mm3, SIMD_MMX_byte_scale_mask4

        por         mm4, mm3
        por         mm5, mm3

        paddb       mm4, SIMD_MMX_byte_minus_128_0
        paddb       mm5, SIMD_MMX_byte_minus_128_0

        pmullw      mm4, mm6
        pmullw      mm5, mm6

        pand        mm4, mm7
        pand        mm5, mm7

        psubb       mm4, SIMD_MMX_byte_minus_128_0
        psubb       mm5, SIMD_MMX_byte_minus_128_0

        movd        dword ptr [edx], mm4
        movd        dword ptr [ecx], mm5

        movq        mm0, qword ptr [esi+ 0*4]
        movq        mm1, qword ptr [esi+ 2*4]
        movq        mm2, qword ptr [esi+ 4*4]
        movq        mm3, qword ptr [esi+ 6*4]

        paddb       mm0, SIMD_MMX_byte_minus_128_0
        paddb       mm1, SIMD_MMX_byte_minus_128_0
        paddb       mm2, SIMD_MMX_byte_minus_128_0
        paddb       mm3, SIMD_MMX_byte_minus_128_0

        pmullw      mm0, mm6
        pmullw      mm1, mm6
        pmullw      mm2, mm6
        pmullw      mm3, mm6

        pand        mm0, mm7
        pand        mm1, mm7
        pand        mm2, mm7
        pand        mm3, mm7

        psubb       mm0, SIMD_MMX_byte_minus_128_0
        psubb       mm1, SIMD_MMX_byte_minus_128_0
        psubb       mm2, SIMD_MMX_byte_minus_128_0
        psubb       mm3, SIMD_MMX_byte_minus_128_0

        movq        qword ptr [esi+ 0*4], mm0
        movq        qword ptr [esi+ 2*4], mm1
        movq        qword ptr [esi+ 4*4], mm2
        movq        qword ptr [esi+ 6*4], mm3

        movq        mm0, qword ptr [esi+ 8*4]
        movq        mm1, qword ptr [esi+10*4]
        movq        mm2, qword ptr [esi+12*4]
        movq        mm3, qword ptr [esi+14*4]

        paddb       mm0, SIMD_MMX_byte_minus_128_0
        paddb       mm1, SIMD_MMX_byte_minus_128_0
        paddb       mm2, SIMD_MMX_byte_minus_128_0
        paddb       mm3, SIMD_MMX_byte_minus_128_0

        pmullw      mm0, mm6
        pmullw      mm1, mm6
        pmullw      mm2, mm6
        pmullw      mm3, mm6

        pand        mm0, mm7
        pand        mm1, mm7
        pand        mm2, mm7
        pand        mm3, mm7

        psubb       mm0, SIMD_MMX_byte_minus_128_0
        psubb       mm1, SIMD_MMX_byte_minus_128_0
        psubb       mm2, SIMD_MMX_byte_minus_128_0
        psubb       mm3, SIMD_MMX_byte_minus_128_0

        movq        qword ptr [esi+ 8*4], mm0
        movq        qword ptr [esi+10*4], mm1
        movq        qword ptr [esi+12*4], mm2
        movq        qword ptr [esi+14*4], mm3

        emms
    }
}

void InsetYCoCgBBox_MMX( byte *minColor, byte *maxColor ) {
    __asm {
        mov         esi, minColor
        mov         edi, maxColor
        movd        mm0, dword ptr [esi]
        movd        mm1, dword ptr [edi]
        punpcklbw   mm0, SIMD_MMX_byte_0
        punpcklbw   mm1, SIMD_MMX_byte_0
        movq        mm2, mm1
        psubw       mm2, mm0
        psubw       mm2, SIMD_MMX_word_insetYCoCgRound
        pand        mm2, SIMD_MMX_word_insetYCoCgMask
        pmullw      mm0, SIMD_MMX_word_insetShiftUp
        pmullw      mm1, SIMD_MMX_word_insetShiftUp
        paddw       mm0, mm2
        psubw       mm1, mm2
        pmulhw      mm0, SIMD_MMX_word_insetShiftDown
        pmulhw      mm1, SIMD_MMX_word_insetShiftDown
        pmaxsw      mm0, SIMD_MMX_word_0
        pmaxsw      mm1, SIMD_MMX_word_0
        pand        mm0, SIMD_MMX_word_inset565Mask
        pand        mm1, SIMD_MMX_word_inset565Mask
        movq        mm2, mm0
        movq        mm3, mm1
        pmulhw      mm2, SIMD_MMX_word_inset565Rep
        pmulhw      mm3, SIMD_MMX_word_inset565Rep
        por         mm0, mm2
        por         mm1, mm3
        packuswb    mm0, mm0
        packuswb    mm1, mm1
        movd        dword ptr [esi], mm0
        movd        dword ptr [edi], mm1
        emms
    }
}

void SelectYCoCgDiagonal_MMX( const byte *colorBlock, byte *minColor, byte *maxColor ) {
    __asm {
        mov         esi, colorBlock
        mov         edx, minColor
        mov         ecx, maxColor

        movq        mm0, qword ptr [esi+ 0]
        movq        mm2, qword ptr [esi+ 8]
        movq        mm1, qword ptr [esi+16]
        movq        mm3, qword ptr [esi+24]

        pand        mm0, SIMD_MMX_dword_word_mask
        pand        mm2, SIMD_MMX_dword_word_mask
        pand        mm1, SIMD_MMX_dword_word_mask
        pand        mm3, SIMD_MMX_dword_word_mask

        psllq       mm0, 16
        psllq       mm3, 16
        por         mm0, mm2
        por         mm1, mm3

        movq        mm2, qword ptr [esi+32]
        movq        mm4, qword ptr [esi+40]
        movq        mm3, qword ptr [esi+48]
        movq        mm5, qword ptr [esi+56]

        pand        mm2, SIMD_MMX_dword_word_mask
        pand        mm4, SIMD_MMX_dword_word_mask
        pand        mm3, SIMD_MMX_dword_word_mask
        pand        mm5, SIMD_MMX_dword_word_mask

        psllq       mm2, 16
        psllq       mm5, 16
        por         mm2, mm4
        por         mm3, mm5

        movd        mm4, dword ptr [edx]
        movd        mm5, dword ptr [ecx]

        pavgb       mm4, mm5
        pshufw      mm4, mm4, R_SHUFFLE_D( 0, 0, 0, 0 )
        movq        mm5, mm4
        movq        mm6, mm4
        movq        mm7, mm4

        pmaxub      mm4, mm0
        pmaxub      mm5, mm1
        pmaxub      mm6, mm2
        pmaxub      mm7, mm3

        pcmpeqb     mm4, mm0
        pcmpeqb     mm5, mm1
        pcmpeqb     mm6, mm2
        pcmpeqb     mm7, mm3

        movq        mm0, mm4
        movq        mm1, mm5
        movq        mm2, mm6
        movq        mm3, mm7

        psrlq       mm0, 8
        psrlq       mm1, 8
        psrlq       mm2, 8
        psrlq       mm3, 8

        pxor        mm0, mm4
        pxor        mm1, mm5
        pxor        mm2, mm6
        pxor        mm3, mm7

        pand        mm0, SIMD_MMX_word_1
        pand        mm1, SIMD_MMX_word_1
        pand        mm2, SIMD_MMX_word_1
        pand        mm3, SIMD_MMX_word_1

        paddw       mm0, mm3
        paddw       mm1, mm2

        movd        mm6, dword ptr [edx]
        movd        mm7, dword ptr [ecx]

#ifdef NVIDIA_7X_HARDWARE_BUG_FIX
        paddw       mm1, mm0
        psadbw      mm1, SIMD_MMX_byte_0
        pcmpgtw     mm1, SIMD_MMX_word_8
        pand        mm1, SIMD_MMX_byte_diagonalMask
        movq        mm0, mm6
        pcmpeqb     mm0, mm7
        psllq       mm0, 8
        pandn       mm0, mm1
#else
        paddw       mm0, mm1
        psadbw      mm0, SIMD_MMX_byte_0
        pcmpgtw     mm0, SIMD_MMX_word_8
        pand        mm0, SIMD_MMX_byte_diagonalMask
#endif

        pxor        mm6, mm7
        pand        mm0, mm6
        pxor        mm7, mm0
        pxor        mm6, mm7

        movd        dword ptr [edx], mm6
        movd        dword ptr [ecx], mm7

        emms
    }
}

void EmitAlphaIndices_MMX( const byte *colorBlock, const byte minAlpha, const byte maxAlpha ) {

    ALIGN16( byte alphaBlock[16] );
    ALIGN16( byte ab1[8] );
    ALIGN16( byte ab2[8] );
    ALIGN16( byte ab3[8] );
    ALIGN16( byte ab4[8] );
    ALIGN16( byte ab5[8] );
    ALIGN16( byte ab6[8] );
    ALIGN16( byte ab7[8] );

    __asm {
        mov         esi, colorBlock
        movq        mm0, qword ptr [esi+ 0]
        movq        mm5, qword ptr [esi+ 8]
        psrld       mm0, 24
        psrld       mm5, 24
        packuswb    mm0, mm5

        movq        mm6, qword ptr [esi+16]
        movq        mm4, qword ptr [esi+24]
        psrld       mm6, 24
        psrld       mm4, 24
        packuswb    mm6, mm4

        packuswb    mm0, mm6
        movq        alphaBlock+0, mm0

        movq        mm0, qword ptr [esi+32]
        movq        mm5, qword ptr [esi+40]
        psrld       mm0, 24
        psrld       mm5, 24
        packuswb    mm0, mm5

        movq        mm6, qword ptr [esi+48]
        movq        mm4, qword ptr [esi+56]
        psrld       mm6, 24
        psrld       mm4, 24
        packuswb    mm6, mm4

        packuswb    mm0, mm6
        movq        alphaBlock+8, mm0

        movzx       ecx, maxAlpha
        movd        mm0, ecx
        pshufw      mm0, mm0, R_SHUFFLE_D( 0, 0, 0, 0 )
        movq        mm1, mm0

        movzx       edx, minAlpha
        movd        mm2, edx
        pshufw      mm2, mm2, R_SHUFFLE_D( 0, 0, 0, 0 )
        movq        mm3, mm2

        movq        mm4, mm0
        psubw       mm4, mm2
        pmulhw      mm4, SIMD_MMX_word_div_by_14

        movq        mm5, mm2
        paddw       mm5, mm4
        packuswb    mm5, mm5
        movq        ab1, mm5

        pmullw      mm0, SIMD_MMX_word_scale654
        pmullw      mm1, SIMD_MMX_word_scale123
        pmullw      mm2, SIMD_MMX_word_scale123
        pmullw      mm3, SIMD_MMX_word_scale654
        paddw       mm0, mm2
        paddw       mm1, mm3
        pmulhw      mm0, SIMD_MMX_word_div_by_7
        pmulhw      mm1, SIMD_MMX_word_div_by_7
        paddw       mm0, mm4
        paddw       mm1, mm4

        pshufw      mm2, mm0, R_SHUFFLE_D( 0, 0, 0, 0 )
        pshufw      mm3, mm0, R_SHUFFLE_D( 1, 1, 1, 1 )
        pshufw      mm4, mm0, R_SHUFFLE_D( 2, 2, 2, 2 )
        packuswb    mm2, mm2
        packuswb    mm3, mm3
        packuswb    mm4, mm4
        movq        ab2, mm2
        movq        ab3, mm3
        movq        ab4, mm4

        pshufw      mm2, mm1, R_SHUFFLE_D( 2, 2, 2, 2 )
        pshufw      mm3, mm1, R_SHUFFLE_D( 1, 1, 1, 1 )
        pshufw      mm4, mm1, R_SHUFFLE_D( 0, 0, 0, 0 )
        packuswb    mm2, mm2
        packuswb    mm3, mm3
        packuswb    mm4, mm4
        movq        ab5, mm2
        movq        ab6, mm3
        movq        ab7, mm4

        pshufw      mm0, alphaBlock+0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm1, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm2, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm3, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm4, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm5, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm6, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm7, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pminub      mm1, ab1
        pminub      mm2, ab2
        pminub      mm3, ab3
        pminub      mm4, ab4
        pminub      mm5, ab5
        pminub      mm6, ab6
        pminub      mm7, ab7
        pcmpeqb     mm1, mm0
        pcmpeqb     mm2, mm0
        pcmpeqb     mm3, mm0
        pcmpeqb     mm4, mm0
        pcmpeqb     mm5, mm0
        pcmpeqb     mm6, mm0
        pcmpeqb     mm7, mm0
        pand        mm1, SIMD_MMX_byte_1
        pand        mm2, SIMD_MMX_byte_1
        pand        mm3, SIMD_MMX_byte_1
        pand        mm4, SIMD_MMX_byte_1
        pand        mm5, SIMD_MMX_byte_1
        pand        mm6, SIMD_MMX_byte_1
        pand        mm7, SIMD_MMX_byte_1
        pshufw      mm0, SIMD_MMX_byte_1, R_SHUFFLE_D( 0, 1, 2, 3 )
        paddusb     mm0, mm1
        paddusb     mm0, mm2
        paddusb     mm0, mm3
        paddusb     mm0, mm4
        paddusb     mm0, mm5
        paddusb     mm0, mm6
        paddusb     mm0, mm7
        pand        mm0, SIMD_MMX_byte_7
        pshufw      mm1, SIMD_MMX_byte_2, R_SHUFFLE_D( 0, 1, 2, 3 )
        pcmpgtb     mm1, mm0
        pand        mm1, SIMD_MMX_byte_1
        pxor        mm0, mm1
        pshufw      mm1, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm2, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm3, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        psrlq       mm1,  8- 3
        psrlq       mm2, 16- 6
        psrlq       mm3, 24- 9
        pshufw      mm4, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm5, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm6, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm7, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        psrlq       mm4, 32-12
        psrlq       mm5, 40-15
        psrlq       mm6, 48-18
        psrlq       mm7, 56-21
        pand        mm0, SIMD_MMX_dword_alpha_bit_mask0
        pand        mm1, SIMD_MMX_dword_alpha_bit_mask1
        pand        mm2, SIMD_MMX_dword_alpha_bit_mask2
        pand        mm3, SIMD_MMX_dword_alpha_bit_mask3
        pand        mm4, SIMD_MMX_dword_alpha_bit_mask4
        pand        mm5, SIMD_MMX_dword_alpha_bit_mask5
        pand        mm6, SIMD_MMX_dword_alpha_bit_mask6
        pand        mm7, SIMD_MMX_dword_alpha_bit_mask7
        por         mm0, mm1
        por         mm2, mm3
        por         mm4, mm5
        por         mm6, mm7
        por         mm0, mm2
        por         mm4, mm6
        por         mm0, mm4
        mov         esi, globalOutData
        movd        dword ptr [esi+0], mm0

        pshufw      mm0, alphaBlock+8, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm1, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm2, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm3, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm4, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm5, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm6, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm7, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pminub      mm1, ab1
        pminub      mm2, ab2
        pminub      mm3, ab3
        pminub      mm4, ab4
        pminub      mm5, ab5
        pminub      mm6, ab6
        pminub      mm7, ab7
        pcmpeqb     mm1, mm0
        pcmpeqb     mm2, mm0
        pcmpeqb     mm3, mm0
        pcmpeqb     mm4, mm0
        pcmpeqb     mm5, mm0
        pcmpeqb     mm6, mm0
        pcmpeqb     mm7, mm0
        pand        mm1, SIMD_MMX_byte_1
        pand        mm2, SIMD_MMX_byte_1
        pand        mm3, SIMD_MMX_byte_1
        pand        mm4, SIMD_MMX_byte_1
        pand        mm5, SIMD_MMX_byte_1
        pand        mm6, SIMD_MMX_byte_1
        pand        mm7, SIMD_MMX_byte_1
        pshufw      mm0, SIMD_MMX_byte_1, R_SHUFFLE_D( 0, 1, 2, 3 )
        paddusb     mm0, mm1
        paddusb     mm0, mm2
        paddusb     mm0, mm3
        paddusb     mm0, mm4
        paddusb     mm0, mm5
        paddusb     mm0, mm6
        paddusb     mm0, mm7
        pand        mm0, SIMD_MMX_byte_7
        pshufw      mm1, SIMD_MMX_byte_2, R_SHUFFLE_D( 0, 1, 2, 3 )
        pcmpgtb     mm1, mm0
        pand        mm1, SIMD_MMX_byte_1
        pxor        mm0, mm1
        pshufw      mm1, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm2, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm3, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        psrlq       mm1,  8- 3
        psrlq       mm2, 16- 6
        psrlq       mm3, 24- 9
        pshufw      mm4, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm5, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm6, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        pshufw      mm7, mm0, R_SHUFFLE_D( 0, 1, 2, 3 )
        psrlq       mm4, 32-12
        psrlq       mm5, 40-15
        psrlq       mm6, 48-18
        psrlq       mm7, 56-21
        pand        mm0, SIMD_MMX_dword_alpha_bit_mask0
        pand        mm1, SIMD_MMX_dword_alpha_bit_mask1
        pand        mm2, SIMD_MMX_dword_alpha_bit_mask2
        pand        mm3, SIMD_MMX_dword_alpha_bit_mask3
        pand        mm4, SIMD_MMX_dword_alpha_bit_mask4
        pand        mm5, SIMD_MMX_dword_alpha_bit_mask5
        pand        mm6, SIMD_MMX_dword_alpha_bit_mask6
        pand        mm7, SIMD_MMX_dword_alpha_bit_mask7
        por         mm0, mm1
        por         mm2, mm3
        por         mm4, mm5
        por         mm6, mm7
        por         mm0, mm2
        por         mm4, mm6
        por         mm0, mm4
        movd        dword ptr [esi+3], mm0
        emms
    }

    globalOutData += 6;
}

void EmitColorIndices_MMX( const byte *colorBlock, const byte *minColor, const byte *maxColor ) {

    ALIGN16( byte color0[8] );
    ALIGN16( byte color1[8] );
    ALIGN16( byte color2[8] );
    ALIGN16( byte color3[8] );
    ALIGN16( byte result[8] );

    __asm {
        mov         esi, maxColor
        mov         edi, minColor
        pxor        mm7, mm7
        movq        result, mm7

        movd        mm0, dword ptr [esi]
        pand        mm0, SIMD_MMX_byte_colorMask
        movq        color0, mm0

        movd        mm1, dword ptr [edi]
        pand        mm1, SIMD_MMX_byte_colorMask
        movq        color1, mm1

        punpcklbw   mm0, mm7
        punpcklbw   mm1, mm7

        movq        mm6, mm1
        paddw       mm1, mm0
        paddw       mm0, mm1
        pmulhw      mm0, SIMD_MMX_word_div_by_3
        packuswb    mm0, mm7
        movq        color2, mm0

        paddw       mm1, mm6
        pmulhw      mm1, SIMD_MMX_word_div_by_3
        packuswb    mm1, mm7
        movq        color3, mm1

        mov         eax, 48
        mov         esi, colorBlock

    loop1:          // iterates 4 times
        movd        mm3, dword ptr [esi+eax+0]
        movd        mm5, dword ptr [esi+eax+4]

        movq        mm0, mm3
        movq        mm6, mm5
        psadbw      mm0, color0
        psadbw      mm6, color0
        packssdw    mm0, mm6
        movq        mm1, mm3
        movq        mm6, mm5
        psadbw      mm1, color1
        psadbw      mm6, color1
        packssdw    mm1, mm6
        movq        mm2, mm3
        movq        mm6, mm5
        psadbw      mm2, color2
        psadbw      mm6, color2
        packssdw    mm2, mm6
        psadbw      mm3, color3
        psadbw      mm5, color3
        packssdw    mm3, mm5

        movd        mm4, dword ptr [esi+eax+8]
        movd        mm5, dword ptr [esi+eax+12]

        movq        mm6, mm4
        movq        mm7, mm5
        psadbw      mm6, color0
        psadbw      mm7, color0
        packssdw    mm6, mm7
        packssdw    mm0, mm6
        movq        mm6, mm4
        movq        mm7, mm5
        psadbw      mm6, color1
        psadbw      mm7, color1
        packssdw    mm6, mm7
        packssdw    mm1, mm6
        movq        mm6, mm4
        movq        mm7, mm5
        psadbw      mm6, color2
        psadbw      mm7, color2
        packssdw    mm6, mm7
        packssdw    mm2, mm6
        psadbw      mm4, color3
        psadbw      mm5, color3
        packssdw    mm4, mm5
        packssdw    mm3, mm4

        movq        mm7, result
        pslld       mm7, 8

        movq        mm4, mm0
        movq        mm5, mm1
        pcmpgtw     mm0, mm3
        pcmpgtw     mm1, mm2
        pcmpgtw     mm4, mm2
        pcmpgtw     mm5, mm3
        pcmpgtw     mm2, mm3
        pand        mm4, mm1
        pand        mm5, mm0
        pand        mm2, mm0
        por         mm4, mm5
        pand        mm2, SIMD_MMX_word_1
        pand        mm4, SIMD_MMX_word_2
        por         mm2, mm4

        pshufw      mm5, mm2, R_SHUFFLE_D( 2, 3, 0, 1 )
        punpcklwd   mm2, SIMD_MMX_word_0
        punpcklwd   mm5, SIMD_MMX_word_0
        pslld       mm5, 4
        por         mm7, mm5
        por         mm7, mm2
        movq        result, mm7

        sub         eax, 16
        jge         loop1

        mov         esi, globalOutData
        movq        mm6, mm7
        psrlq       mm6, 32-2
        por         mm7, mm6
        movd        dword ptr [esi], mm7
        emms
    }

    globalOutData += 4;
}

bool CompressYCoCgDXT5_MMX( const byte *inBuf, byte *outBuf, int width, int height, int &outputBytes ) {
    ALIGN16( byte block[64] );
    ALIGN16( byte minColor[4] );
    ALIGN16( byte maxColor[4] );

    globalOutData = outBuf;

    for ( int j = 0; j < height; j += 4, inBuf += width * 4*4 ) {
        for ( int i = 0; i < width; i += 4 ) {

            ExtractBlock_MMX( inBuf + i * 4, width, block );

            GetMinMaxYCoCg_MMX( block, minColor, maxColor );
            ScaleYCoCg_MMX( block, minColor, maxColor );
            InsetYCoCgBBox_MMX( minColor, maxColor );
            SelectYCoCgDiagonal_MMX( block, minColor, maxColor );

            EmitByte( maxColor[3] );
            EmitByte( minColor[3] );

            EmitAlphaIndices_MMX( block, minColor[3], maxColor[3] );

            EmitUShort( ColorTo565( maxColor ) );
            EmitUShort( ColorTo565( minColor ) );

            EmitColorIndices_MMX( block, minColor, maxColor );
        }
    }

    outputBytes = globalOutData - outBuf;

    return true;
}
