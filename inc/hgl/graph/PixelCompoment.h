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

            HGL_PC_ALPHA,
            HGL_PC_DEPTH,
            HGL_PC_LUMINANCE,
            HGL_PC_LUMINANCE_ALPHA,

            HGL_PC_RGB,
            HGL_PC_RGBA,

            HGL_PC_END
        };//enum PixelCompoment
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_PIXEL_COMPENT_INCLUDE
