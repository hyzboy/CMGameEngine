#include<hgl/platform/PlatfomInterface.h>
namespace hgl
{
    namespace platform
    {
        const bool InitGraphicsPlatform(){}
        void CloseGraphicsPlatform(){}

        const UTF8String &GetPlatformName()
        {
            static UTF8String platform_name="X11";

            return platform_name;
        }

//         const ObjectList<Monitor> &GetMonitorList();                                                ///<取得显示器列表
//         const Monitor *GetDefaultMonitor();                                                         ///<取得缺省显示器
//
//         const VideoMode *GetCurVideoMode(const Monitor *m=nullptr);                                 ///<取得指定显示器当前显示模式
//         const ObjectList<VideoMode> &GetVideoMode(const Monitor *m=nullptr);                        ///<取得指定显示器所有显示模式

//         Window *Create(int,int,const WindowSetup *,const OpenGLSetup *);                            ///<创建一个窗口
//         Window *Create(const Monitor *,const VideoMode *,const OpenGLSetup *);                      ///<创建全屏窗口
    }//namespace platform
}//namespace hgl

