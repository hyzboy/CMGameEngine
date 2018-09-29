#ifndef HGL_GRAPHICS_RENDER_WINDOW_INCLUDE
#define HGL_GRAPHICS_RENDER_WINDOW_INCLUDE

#include<hgl/graph/WindowConfig.h>
namespace hgl
{
    namespace graph
    {
        /**
         * 渲染窗口
         */
        class RenderWindow
        {
            WindowConfig win_cfg;

        public:

            RenderWindow();
            virtual ~RenderWindow();

            const WindowConfig *GetConfig()const{return &win_cfg;}                                  ///<取得窗口配置信息
        };//class RenderWindow
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPHICS_RENDER_WINDOW_INCLUDE
