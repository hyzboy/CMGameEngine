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

                filesystem::SaveMemoryToFile(filename,shader_string.c_str(),shader_string.Length());
            }

            void shader_stringlist::add_version(int ver)
            {
                glsl_version=ver;

                add("#version "+UTF8String(ver)+" core\n\n");
            }

			void shader_stringlist::add_texture_smooth()
            {//高质量纹理采样,对应HGL_FILTER_SMOOTH
                if(glsl_version<400)                                        //GLSL 4.0以下不支持smoothstep，所以使用自行实现
                add("float smoothstep(float edge0,float edge1,float x)\n"
                    "{\n"
                    "\tx=clamp((x-edge0)/(edge1-edge0),0.0,1.0);\n"
                    "\treturn x*x*(3.0-2.0*x);\n"
                    "}\n\n");

                add("vec4 texture_smooth(sampler2D samp, vec2 tc)\n"
                    "{\n"
                    "\n"
                    "\tvec2 texSize=textureSize(samp, 0);\n"
                    "\tvec2 uvScaled=tc * texSize + 0.5;\n"
                    "\tvec2 uvInt=floor(uvScaled);\n"
                    "\tvec2 uvFrac=fract(uvScaled);\n"
                    "\n"
                    "\tuvFrac=smoothstep(0.0, 1.0, uvFrac);\n"
                    "\tvec2 uv=(uvInt + uvFrac - 0.5) / texSize;\n"
                    "\n"
                    "\treturn texture(samp, uv);\n"
                    "}\n\n");
            }

            void shader_stringlist::add_rgb2lum()
            {
                add("float rgb2lum(vec3 color)\n"
                    "{\n"
                    "\treturn (color.r*0.299)+(color.g*0.587)+(color.b*0.114);\n"
                    "}\n\n");
            }

            void shader_stringlist::add_rgb2hsv()
            {
                add("vec3 rgb2hsv(vec3 c)\n"
                    "{\n"
                    "\tvec3 hsv;\n"
                    "\n"
                    "\tfloat min,max,delta,tmp;\n"
                    "\n"
                    "\ttmp=c.r>c.g?c.g:c.r;\n"
                    "\tmin=tmp>c.b?c.b:tmp;\n"
                    "\ttmp=c.r>c.g?c.r:c.g;\n"
                    "\tmax=tmp>c.b?tmp:c.b;\n"
                    "\n"
                    "\thsv.b=max;\n"
                    "\tdelta=max-min;\n"
                    "\n"
                    "\tif(max!=0)hsv.g=delta/max;\n"
                    "\telse\n"
                    "\t{\n"
                    "\t\thsv.r=0;\n"
                    "\t\thsv.g=0;\n"
                    "\t\treturn hsv;\n"
                    "\t}\n"
                    "\n"
                    "\tif(delta==0)\n"
                    "\t{\n"
                    "\t\thsv.r=0;\n"
                    "\t\treturn hsv;\n"
                    "\t}\n"
                    "\telse if(c.r==max)\n"
                    "\t{\n"
                    "\t\tif(c.g>=c.b)\n"
                    "\t\t\thsv.r=(c.g-c.b)/delta;\n"
                    "\t\telse\n"
                    "\t\t\thsv.r=(c.g-c.b)/delta+6.0;\n"
                    "\t}\n"
                    "\telse if(c.g==max)\n"
                    "\t\thsv.r=2.0+(c.b-c.r)/delta;\n"
                    "\telse if(c.b==max)\n"
                    "\t\thsv.r=4.0+(c.r-c.g)/delta;\n"
                    "\n"
//                    "\thsv.r *= 60.0;\n"
                    "\thsv.r/=6.0;\n"            //标准HSV中H的值是0-360，所以上面的*60我们不要了，直接/6
                    "\n"
                    "\treturn hsv;\n"
                    "}\n\n");
            }

            void shader_stringlist::add_hsv_clamp()
            {
                add("bool hsv_clamp(vec3 hsv)\n"
                    "{\n"
                    //"\tvec3 hsv=rgb2hsv(rgb_color);\n"
                    //"\n"
                    "\tif(abs(hsv.r-" HGL_FS_HSV_CLAMP_COLOR ".r)>" HGL_FS_HSV_CLAMP_COLOR ".b)return(false);\n"            //传进来的CLAMP_COLOR，只存了HV以及两个容差，放弃S值
                    "\tif(abs(hsv.b-" HGL_FS_HSV_CLAMP_COLOR ".g)>" HGL_FS_HSV_CLAMP_COLOR ".a)return(false);\n"            //所以上一行是(R-R)>B,下一行是(B-G)>A
                    "\n"
                    "\treturn(true);\n"
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
