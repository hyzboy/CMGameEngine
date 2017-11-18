/*
* encode_uvec2_gbuffer 生成的数据需要保存在双道通64位纹理中，也就是RG32UI
* RG32UI最低需要OpenGL 3.0,nv显卡最低G80(G8800 GTX/GTS)
* packUnorm4x8最低需要OpenGL 4.1
*/

// OpenGL 3.0/64位版
// uvec4 encode_uvec2_gbuffer_3(in vec3 color,in vec3 normal,in float spec)
// {
//     uvec4
//     return uvec2(packUnorm4x8(color,spec),packUnorm4x8(normal,0));
// }

// OpenGL 4.1/64位版
uvec2 encode_uvec2_gbuffer_4_1(in vec3 color,in vec3 normal,in float spec)
{
    return uvec2(packUnorm4x8(color,    spec),
                 packUnorm4x8(normal,   0   ));
}
