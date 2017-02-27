#ifndef HGL_GRAPH_PIXEL_COMPENT_INCLUDE
#define HGL_GRAPH_PIXEL_COMPENT_INCLUDE

#include<glew/include/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 顶点象素成份枚举
        */
        enum PixelCompoment
        {
            HGL_PC_NONE=0,

            //单通道
            HGL_PC_ALPHA,
            HGL_PC_DEPTH,
            HGL_PC_LUMINANCE,

            //2通道
            HGL_PC_LUMINANCE_ALPHA,

            //3通道
            HGL_PC_RGB,

            //4通道
            HGL_PC_RGBA,
            HGL_PC_RGB_SPECULAR,
            HGL_PC_RGB_DEPTH,
            HGL_PC_NORMAL_HEIGHT,

            //4通道 2+2模式
            HGL_PC_RGB_NORMAL,
            HGL_PC_NORMAL_TANGENT,

            HGL_PC_END
        };//enum PixelCompoment
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_PIXEL_COMPENT_INCLUDE
