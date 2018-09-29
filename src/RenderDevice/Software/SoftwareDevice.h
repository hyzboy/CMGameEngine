#ifndef HGL_RENDER_DEVICE_SOFTWARE_DEVICE_INCLUDE
#define HGL_RENDER_DEVICE_SOFTWARE_DEVICE_INCLUDE

#include<hgl/graph/RenderDevice.h>
namespace hgl
{
    namespace graph
    {
        /**
         * 软件渲染设备<br>
         * 仅有超简单的2D渲染功能，用于常规GUI的应用程序
         */
        class SoftwareDevice:public RenderDevice
        {
        };//class SoftwareDevice:public RenderDevice
    }//namespace graph
}//namespace hgl
#endif//HGL_RENDER_DEVICE_SOFTWARE_DEVICE_INCLUDE
