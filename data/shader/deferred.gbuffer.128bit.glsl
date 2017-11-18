// Minimum support Hardword
// nVidia: NV30(Geforce FX) RGBA32F
// ATI:

// OpenGL 4.1/128位版
vec4 encode_vec4_gbuffer_4_1(in vec3 color,in vec3 normal,in float spec)
{
    return vec4(packHalf2x16(color.rg       ),
                packHalf2x16(color.b,   spec),
                packHalf2x16(normal.rg      ),
                packHalf2x16(normal.b,  0   ));
}
