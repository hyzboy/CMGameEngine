#ifndef HGL_RENDER_DEVICE_NOOP_DEVICE_INCLUDE
#define HGL_RENDER_DEVICE_NOOP_DEVICE_INCLUDE

#include<hgl/graph/RenderDevice.h>
namespace hgl
{
    namespace graph
    {
        /**
         * 空渲染设备<br>
         * 不会处理任何渲染，但仍会处理各种3D数据，一般为一些无图形界面的3D工具软件服务
         */
        class NoopDevice:public RenderDevice
        {
        public:

            const   List<MonitorInfo *> *   GetMonitorList()override{return nullptr;}               ///<获取显示器列表
                    RenderWindow *          CreateWindow(const WindowConfig *)override;             ///<创建窗口
        };//class NoopDevice:public RenderDevice
    }//namespace graph
}//namespace hgl
#endif//HGL_RENDER_DEVICE_NOOP_DEVICE_INCLUDE
