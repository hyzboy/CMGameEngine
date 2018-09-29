#ifndef HGL_GRAPHICS_NOOP_RENDER_WINDOW_INCLUDE
#define HGL_GRAPHICS_NOOP_RENDER_WINDOW_INCLUDE

#include<hgl/graph/RenderWindow.h>
namespace hgl
{
    namespace graph
    {
        /**
         * 空设备渲染窗口
         */
        class NoopWindow:public RenderWindow
        {
        public:

            using RenderWindow::RenderWindow;
            virtual ~NoopWindow();

        };//class NoopWindow:public RenderWindow
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPHICS_NOOP_RENDER_WINDOW_INCLUDE
