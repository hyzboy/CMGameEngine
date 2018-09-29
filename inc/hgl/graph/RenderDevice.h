#ifndef HGL_GRAPHICS_RENDER_DEVICE_INCLUDE
#define HGL_GRAPHICS_RENDER_DEVICE_INCLUDE

#include<hgl/graph/WindowConfig.h>
#include<hgl/type/List.h>
namespace hgl
{
    namespace graph
    {
        class RenderWindow;

        /**
         * 渲染设备
         */
        class RenderDevice
        {
        public:

            const   List<MonitorInfo *> *   GetMonitorList()=0;                                     ///<获取显示器列表
                    RenderWindow *          CreateWindow(const WindowConfig *)=0;                   ///<创建窗口
        };//class RenderDevice
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPHICS_RENDER_DEVICE_INCLUDE
