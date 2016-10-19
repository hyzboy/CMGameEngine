#ifndef HGL_GRAPHICS_SYSTEM_INIT_INFO_INCLUDE
#define HGL_GRAPHICS_SYSTEM_INIT_INFO_INCLUDE

#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/graph/Font.h>
namespace hgl
{
	/**
	 * 系统初始化信息类
	 */
	class GraphicsSystemInitInfo:public ConsoleSystemInitInfo										///系统初始化信息类
	{
	public:

		bool WaitActive;                                                                            ///<等待应用程序进入活动状态,默认为false
		bool BackNotDraw;                                                                           ///<当程序窗口位于后台时不绘制画面,默认为true
		bool DisableScreenSave;																		///<禁止屏幕保护程序运行,默认为true

		bool UseJoystick;																			///<是否使用游戏杆

		uint KeyPressTime;																			///<按键产生一个press触发所需的时间

		struct Graphics
		{
			bool FullScreen;                                                                        ///<是否全屏,默认false,即窗口模式

			int32 Width,Height;                                                                     ///<游戏画面大小
			int32 Bit;                                                                              ///<色彩位数,默认32
			int32 VSync;                                                                            ///<屏幕垂直刷新率,默认75Hz

			int32 FPS;                                                                              ///<期望的FPS,默认60

			struct Safe																				///<安全模式画面大小(当全屏进入失败时的选项)
			{
				int32 Width,Height;                                                                 ///<游戏画面大小
			}safe;

			struct OpenGLSetup
			{
				int32 AlphaBits;                                                                    ///<Alpha缓冲区位深度,默认8位
				int32 DepthBits;                                                                    ///<Depth缓冲区位深度,默认24
				int32 StencilBits;                                                                  ///<Stencil缓冲区位深度,默认8
				int32 AccumBits;                                                                    ///<Accum缓冲区位深度,默认0

				int32 MultiSample;                                                                  ///<多重采样级别(全屏抗矩齿级别)

				bool NicestTextureCompress;															///<高质量贴图压缩,默认为真

				bool texture_rectangle;																///<是否启用矩形贴图
				bool texture_non_power_of_two;														///<是否启用非2次幂贴图
				bool vbo;																			///<是否启用vbo
				bool pbo;																			///<是否启用pbo
				bool fbo;																			///<是否启用fbo
				bool glsl;																			///<是否启用glsl
				bool multi_texture;																	///<是否启用多重贴图

				int32 major,minor;																	///<OpenGL核心模式版本需求(默认为0,0表示无需求使用兼容模式)

				bool debug;																			///<产生硬件Debug信息(默认不产生，并且此功能不对所有显卡以及驱动有效)
				bool opengl_es;																		///<使用OpenGL ES模式
				bool egl;																			///<使用EGL模式
			}gl;

			struct GUI
			{
				bool use;																			///<是否使用GUI，默认不使用

				UTF16String pack;																	///<缺省GUI包
			}gui;

			struct
			{
				bool use;																			///<默认不使用

				Font eng_fnt;																		///<缺省英文字体
				Font chs_fnt;																		///<缺省中文字体
				int fnt_buf;																		///<缺省字体缓冲区大小,默认2000
			}fnt;
		}graphics;

		struct Audio
		{
			bool enum_device;																		///<是否枚举设备
			AnsiString DeviceName;																	///<设备名称
		}audio;

		struct WindowSetup
		{
			UTF8String Name;																		///<窗口标题
#if HGL_OS == HGL_OS_Windows
			UTF16String ClassName;																	///<类名(windows用)
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
		}win;

	public:

		GraphicsSystemInitInfo();                                                                   ///<本类构造函数
// 		virtual ~GraphicsSystemInitInfo()=default;													///<本类析构函数

		void Init(const OSStringList &args);														///<使用外部命令行参数

		void ProcBlankValue();                                                              		///<处理未填写的值
	};//class SystemInitInfo

// 	bool GetDesktopDisplayMode(int &,int &,int &,int &);											///<取得当前桌面显示模式
}//namespace hgl
#endif//HGL_GRAPHICS_SYSTEM_INIT_INFO_INCLUDE
