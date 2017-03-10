#include"ShaderStringList.h"
#include"ShaderLight.h"
#include<malloc.h>
#include<string.h>
#include<stdarg.h>
#include<stdio.h>
#include<hgl/Other.h>
#include<hgl/graph/TextureFormat.h>

namespace hgl
{
    namespace graph
    {
        namespace shadergen
        {
            const char shader_float_type[4][8]=
            {
                "float",
                "vec2",
                "vec3",
                "vec4"
            };

            const char *get_sampler_by_tex_type(uint type)
            {
                switch(type)
                {
                    case HGL_TEXTURE_1D:            return "sampler1D";
                    case HGL_TEXTURE_2D:            return "sampler2D";
                    case HGL_TEXTURE_3D:            return "sampler3D";

                    case HGL_TEXTURE_1D_ARRAY:      return "sampler1DArray";
                    case HGL_TEXTURE_2D_ARRAY:      return "sampler2DArray";

                    case HGL_TEXTURE_RECTANGLE:     return "samplerRect";

                    case HGL_TEXTURE_CUBE_MAP:      return "samplerCube";
                    case HGL_TEXTURE_CUBE_MAP_ARRAY:return "samplerCubeArray";

                    case HGL_TEXTURE_2D_MS:         return "sampler2DMS";
                    case HGL_TEXTURE_2D_MS_ARRAY:   return "sampler2DMSArray";

                    case HGL_TEXTURE_BUFFER:        return "samplerBuffer";

                    default:                        return nullptr;
                }
            }

            const char *get_sampler_shadow_by_tex_type(uint type)
            {
                switch(type)
                {
                    case HGL_TEXTURE_1D:            return "sampler1DShadow";
                    case HGL_TEXTURE_2D:            return "sampler2DShadow";
                    case HGL_TEXTURE_RECTANGLE:     return "samplerRectShadow";
                    case HGL_TEXTURE_CUBE_MAP:      return "samplerCubeShadow";

                    default:                        return nullptr;
                }
            }

            void shader_stringlist::debug_out(const os_char *filename)
            {
                if(!filename)return;

                SaveMemoryToFile(filename,shader_string.c_str(),shader_string.Length());
            }

            void shader_stringlist::add_version(int ver)
            {
                add("#version "+UTF8String(ver)+" core\n\n");
            }

			void shader_stringlist::add_texture_smooth()
            {//高质量纹理采样,对应HGL_FILTER_SMOOTH(源代码参考自OpenGL SB7，注：原文代码有bug，同时smoothstep函数需要OpenGL 4.0)
                add("vec4 texture_smooth(sampler2D samp, vec2 tc)\n"
                    "{\n"
                    "\n"
                    "\tvec2 texSize = textureSize(samp, 0);\n"
                    "\tvec2 uvScaled = tc * texSize + 0.5;\n"
                    "\tvec2 uvInt = floor(uvScaled);\n"
                    "\tvec2 uvFrac = fract(uvScaled);\n"
                    "\n"
                    "\tuvFrac = smoothstep(0.0, 1.0, uvFrac);\n"
                    "\tvec2 uv = (uvInt + uvFrac - 0.5) / texSize;\n"
                    "\n"
                    "\treturn texture(samp, uv);\n"
                    "}\n\n");
            }

            void shader_stringlist::add_normal_3to2()
            {
                add("vec2 normal_3to2(vec3 normal)\n"
                    "{\n"
                    "\treturn normalize(normal.xy)*sqrt(normal.z*0.5+0.5);\n"
                    "}\n\n");
            }

            void shader_stringlist::add_normal_2to3()
            {
                add("vec3 normal_2to3(vec2 n)\n"
                    "{\n"
                    "\tvec3 normal;\n"
                    "\n"
                    "\tnormal.z=dot(n,n)*2-1;\n"
                    "\tnormal.xy=normalize(n)*sqrt(1-normal.z*normal.z);\n"
                    "\n"
                    "\treturn normal;\n"
                    "}\n\n");
            }

            void shader_stringlist::add_material(bool two_side)
            {
                add("struct Material\n"
                    "{\n"
                    "\tvec4 emission;\n"
                    "\tvec4 ambient;\n"
                    "\tvec4 diffuse;\n"
                    "\tvec4 specular;\n"
                    "\tfloat shininess;\n"
                    "};\n"
                    "\n"
                    "uniform Material FrontMaterial;\n");

                if(two_side)
                    add("uniform Material BackMaterial;\n");
            }

            void shader_stringlist::add_direction_light     (){add(glsl_light_directional   );}
            void shader_stringlist::add_point_light         (){add(glsl_light_point         );}
            void shader_stringlist::add_spot_light          (){add(glsl_light_spot          );}
            void shader_stringlist::add_infinite_spot_light (){add(glsl_light_infinite_spot );}

            void shader_stringlist::add_light(int dl,int pl,int sl,int isl,bool sky)
            {
                UTF8String str;

                if(dl>0||sky)
                {
                    str="uniform DirectionLight DirectionLightSource["+UTF8String(dl+(sky?1:0))+"];\n\n";

                    add(str);
                }

                if(pl>0)
                {
                    str="uniform PointLight PointLightSource["+UTF8String(pl)+"];\n\n";

                    add(str);
                }

                if(sl>0)
                {
                    str="uniform SpotLight SpotLightSource["+UTF8String(sl)+"];\n\n";

                    add(str);
                }

                if(isl>0)
                {
                    str="uniform InfiniteSpotLight InfiniteSpotLightSource["+UTF8String(isl)+"];\n\n";

                    add(str);
                }
            }

            void shader_stringlist::add_main_begin()
            {
                add("void main(void)\n"
                    "{\n");
            }

            void shader_stringlist::add_main_end()
            {
                add("}\n");
            }
        }//namespace shadergen
    }//namespace graph
}//namespace hgl
