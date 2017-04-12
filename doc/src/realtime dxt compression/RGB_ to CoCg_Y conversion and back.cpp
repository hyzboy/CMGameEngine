/*
    RGB_ to CoCg_Y conversion and back.
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

typedef unsigned char   byte;

/*
    RGB <-> YCoCg

    Y  = [ 1/4  1/2  1/4] [R]
    Co = [ 1/2    0 -1/2] [G]
    CG = [-1/4  1/2 -1/4] [B]

    R  = [   1    1   -1] [Y]
    G  = [   1    0    1] [Co]
    B  = [   1   -1   -1] [Cg]

*/

byte CLAMP_BYTE( int x ) { return ( (x) < 0 ? (0) : ( (x) > 255 ? 255 : (x) ) ); }

#define RGB_TO_YCOCG_Y( r, g, b )   ( ( (    r +   (g<<1) +  b     ) + 2 ) >> 2 )
#define RGB_TO_YCOCG_CO( r, g, b )  ( ( (   (r<<1)        - (b<<1) ) + 2 ) >> 2 )
#define RGB_TO_YCOCG_CG( r, g, b )  ( ( ( -  r +   (g<<1) -  b     ) + 2 ) >> 2 )

#define COCG_TO_R( co, cg )         ( co - cg )
#define COCG_TO_G( co, cg )         ( cg )
#define COCG_TO_B( co, cg )         ( - co - cg )

void ConvertRGBToCoCg_Y( byte *image, int width, int height ) {
    for ( int i = 0; i < width * height; i++ ) {
        int r = image[i*4+0];
        int g = image[i*4+1];
        int b = image[i*4+2];
        int a = image[i*4+3];
        image[i*4+0] = CLAMP_BYTE( RGB_TO_YCOCG_CO( r, g, b ) + 128 );
        image[i*4+1] = CLAMP_BYTE( RGB_TO_YCOCG_CG( r, g, b ) + 128 );
        image[i*4+2] = a;
        image[i*4+3] = CLAMP_BYTE( RGB_TO_YCOCG_Y( r, g, b ) );
    }
}

void ConvertCoCg_YToRGB( byte *image, int width, int height ) {
    for ( int i = 0; i < width * height; i++ ) {
        int y  = image[i*4+3];
        int co = image[i*4+0] - 128;
        int cg = image[i*4+1] - 128;
        int a  = image[i*4+2];
        image[i*4+0] = CLAMP_BYTE( y + COCG_TO_R( co, cg ) );
        image[i*4+1] = CLAMP_BYTE( y + COCG_TO_G( co, cg ) );
        image[i*4+2] = CLAMP_BYTE( y + COCG_TO_B( co, cg ) );
        image[i*4+3] = a;
    }
}
