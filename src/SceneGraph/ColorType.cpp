#include<hgl/type/ColorType.h>
#include<hgl/Str.h>

namespace hgl
{
    const u16char ColorTypeName[ColorTypeCount][32]=
    {
        U16_TEXT(""),

        U16_TEXT("Luminance"),
        U16_TEXT("Luminance Alpha"),
        U16_TEXT("RGB"),
        U16_TEXT("RGBA"),
        U16_TEXT("Alpha"),

        U16_TEXT("BGR"),
        U16_TEXT("BGRA"),

        U16_TEXT("")
    };

    const unsigned int GLFormatByColorType[ColorTypeCount]=    ///针对色彩类型而对应的OpenGL格式
    {
        //使用数字是为了避在编译时非渲染库时依然需要依赖opengl开发库
        0,      //GL_NONE,                    //  ctNone

        0x1909, //GL_LUMINANCE,               //  ctLuminance
        0x190A, //GL_LUMINANCE_ALPHA,         //  ctLuminanceAlpha
        0x1907, //GL_RGB,                     //  ctRGB
        0x1908, //GL_RGBA,                    //  ctRGBA
        0x1906, //GL_ALPHA,                   //  ctAlpha

        0x80E0, //GL_BGR,                     //  ctBGR
        0x80E1, //GL_BGRA,                    //  ctBGRA

        0       //GL_NONE                     //  ctEnd
    };

    const unsigned int BytesByColorType[ColorTypeCount]=  ///针对色彩类型而对应的每象素字节数
    {
        0,      //  ctNone

        1,      //  ctLuminance
        2,      //  ctLuminanceAlpha
        3,      //  ctRGB
        4,      //  ctRGBA
        1,      //  ctAlpha

        3,      //  ctBGR
        4,      //  ctBGRA

        0,      //  ctEnd
    };

       const bool BlendByColorType[]=          //这一种格式是否需要混合
    {
        false,  //  ctNone

        false,  //  ctLuminance
        true,   //  ctLuminanceAlpha
        false,  //  ctRGB
        true,   //  ctRGBA
        true,   //  ctAlpha

        false,  //  ctBGR
        true,   //  ctBGRA

        false,  //  ctEnd
    };

    unsigned int ColorNameToFormat(const u16char *str)
    {
        for(int i=ctNone+1;i<ctEnd;i++)
        {
            if(stricmp(str,ColorTypeName[i])==0)
                return(GLFormatByColorType[i]);
        }

        return 0;
    }
}//namespace hgl
