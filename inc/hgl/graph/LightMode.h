#ifndef HGL_GRAPH_LIGHT_MODE_INCLUDE
#define HGL_GRAPH_LIGHT_MODE_INCLUDE

#include<hgl/platform/Platform.h>

namespace hgl
{
    namespace graph
    {
        enum LightMode:uint8
        {
            HGL_NONE_LIGHT = 0,
            HGL_VERTEX_LIGHT,
            HGL_PIXEL_LIGHT
        };//enum LightMode:uint8
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_LIGHT_MODE_INCLUDE