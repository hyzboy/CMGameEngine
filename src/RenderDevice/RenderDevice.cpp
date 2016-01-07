#include<hgl/graph/RenderDevice.h>

namespace hgl
{
    namespace graph
    {
        namespace
        {
            typedef RenderDevice *CreateRenderDeviceFunc();

            RenderDevice *CreateRenderDeviceOpenGLCore4();

            CreateRenderDeviceFunc create_render_device_func_list[rdtEnd-1]=
            {
                nullptr,
                CreateRenderDeviceOpenGLCore4,

                nullptr,
                nullptr,
                nullptr,
                nullptr,
            };//
        }

        RenderDevice *CreateRenderDevice(const RenderDeivceType &type)
        {
            if(create_render_device_func_list[type-1])
                return create_render_device_func_list[type-1]();

            return nullptr;
        }
    }//namespace graph
}//namespace hgl
