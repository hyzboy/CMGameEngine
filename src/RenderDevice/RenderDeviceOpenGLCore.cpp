#include<hgl/graph/RenderDevice.h>

namespace hgl
{
    namespace graph
    {
        class RenderDeviceOpenGLCore:public RenderDevice
        {
        };//class RenderDeviceOpenGLCore

        RenderDevice *CreateRenderDeviceOpenGLCore4()
        {
            return(new RenderDeviceOpenGLCore);
        }

        RenderableData *RenderDeviceOpenGLCore::CreateRenderable()
        {
            return(new OpenGLCoreRenderableData);
        }

        Renderable *RenderDeviceOpenGLCore::CreateRenderable(RenderableData *rd)
        {
            if(!rd)return nullptr;

            return(new OpenGLCoreRenderable(rd));
        }
    }//namespace graph
}//namespace hgl
