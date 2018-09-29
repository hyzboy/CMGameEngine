#ifndef HGL_GRAPHICS_WINDOW_CONFIG_INCLUDE
#define HGL_GRAPHICS_WINDOW_CONFIG_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
    namespace graph
    {
        /**
         * 显示器信息
         */
        struct MonitorInfo
        {
            uint id;                    ///<显示器ID
        };//struct MonitorInfo

        /**
         * 窗口配置
         */
        struct WindowConfig
        {
            uint monitor_id;            ///<显示器ID
            bool fullscreen;            ///<是否全屏
            uint width,height;          ///<窗口尺寸
            uint bits;                  ///<色彩位深(窗口下此参数不一定有效)
            uint vsync;                 ///<垂直同步频率(窗口下此参数无效)

            OSString win_name;          ///<窗口标题

            bool resize;                ///<是否允许缩放尺寸(全屏下此参数无效)
        };//struct WindowConfig
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPHICS_WINDOW_CONFIG_INCLUDE
