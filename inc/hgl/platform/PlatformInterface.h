#ifndef HGL_PLATFORM_INTERFACE_INCLUDE
#define HGL_PLATFORM_INTERFACE_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/List.h>
namespace hgl
{
    namespace platform
    {
        class Window;

        /**
         * 显示器数据结构
        */
        struct Monitor
        {
            UTF8String name;                                                                        ///<显示器名称
            int width,height;                                                                       ///<显示器尺寸(单位:毫米)
            int x,y;                                                                                ///<多显示器时的排列位置
        };

        /**
        * 显示模式数据结构
        */
        struct VideoMode
        {
            int width;                      ///<宽
            int height;                     ///<高
            int bit;                        ///<色彩位数
            int freq;                       ///<刷新率

            int red;                        ///<红色位数
            int green;                      ///<绿色位数
            int blue;                       ///<蓝色位数
        };//struct VideoMode

		struct WindowSetup
		{
			UTF8String Name;																		///<窗口标题

#if HGL_OS == HGL_OS_Windows
			WideString ClassName;																	///<类名(windows用)
#endif//HGL_OS == HGL_OS_Windows

			OSString IconFilename;																	///<图标文件名称
			OSString CursorFilename;																///<光标文件名称
			bool Edge;																				///<是否显示边框

			bool SysMenu;                                                                           ///<是否显示系统菜单
			bool Right;                                                                             ///<窗口是否使用靠右风格

			bool Resize;                                                                            ///<窗口大小是否可调整
			bool Minimize;                                                                          ///<窗口是否可以最小化
			bool Maximize;                                                                          ///<窗口是否可以最大化

			bool TopMost;                                                                           ///<永远在最上面
			bool AppTaskBar;                                                                        ///<程序项在任务栏显示
		};

        struct OpenGLSetup
        {
            int32 AlphaBits;																		///<Alpha缓冲区位深度,默认8位
            int32 DepthBits;																		///<Depth缓冲区位深度,默认24
            int32 StencilBits;																		///<Stencil缓冲区位深度,默认8

            bool no_use_stencil;                                                                    ///<不使用Stencil缓冲区

            int32 AccumRedBits;																		///<Accum缓冲区红色位深度,默认0
            int32 AccumGreenBits;																	///<Accum缓冲区绿色位深度,默认0
            int32 AccumBlueBits;																	///<Accum缓冲区蓝色位深度,默认0
            int32 AccumAlphaBits;																	///<Accum缓冲区Alpha位深度,默认0

            int32 MultiSample;																		///<多重采样级别(全屏抗矩齿级别)

            bool NicestTextureCompress;																///<高质量贴图压缩,默认为真

            bool texture_rectangle;																	///<是否启用矩形贴图
            bool texture_non_power_of_two;															///<是否启用非2次幂贴图
            bool vbo;																				///<是否启用vbo
            bool pbo;																				///<是否启用pbo
            bool fbo;																				///<是否启用fbo
            bool glsl;																				///<是否启用glsl
            bool multi_texture;																		///<是否启用多重贴图
            bool texture_storage;																	///<是否启用纹理存储api
            bool dsa;																				///<是否启用DSA
            float texture_lod_bias;                                                                 ///<默认纹理LOD Bias(默认0)
            float texture_max_anistropy;                                                            ///<纹理最大各向异性过滤值比例(使用0.0-1.0，默认0.5)

            bool es;																				///<是否使用OpenGL ES模式
            bool egl;																				///<是否使用EGL

            int32 major,minor;																		///<OpenGL模式版本需求(默认为0,0表示无需求使用兼容模式)

            bool debug;																				///<是否使用硬件Debug信息(默认不产生，并且此功能不对所有显卡以及驱动有效)
        };

        const bool InitGraphicsPlatform();                                                          ///<初始化平台接口
        void CloseGraphicsPlatform();                                                               ///<关闭平台接口

        const UTF8String &GetPlatformName();                                                        ///<获取平台名称

        const ObjectList<Monitor> &GetMonitorList();                                                ///<取得显示器列表
        const Monitor *GetDefaultMonitor();                                                         ///<取得缺省显示器

        const VideoMode *GetCurVideoMode(const Monitor *m=nullptr);                                 ///<取得指定显示器当前显示模式
        const ObjectList<VideoMode> &GetVideoMode(const Monitor *m=nullptr);                        ///<取得指定显示器所有显示模式

        Window *Create(int,int,const WindowSetup *,const OpenGLSetup *);                            ///<创建一个窗口
        Window *Create(const Monitor *,const VideoMode *,const OpenGLSetup *);                      ///<创建全屏窗口
    }//namespace platform
}//namespace hgl
#endif//HGL_PLATFORM_INTERFACE_INCLUDE

