#ifndef HGL_GRAPHICS_SYSTEM_INIT_INFO_INCLUDE
#define HGL_GRAPHICS_SYSTEM_INIT_INFO_INCLUDE

#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/platform/PlatformInterface.h>
#include<hgl/graph/Font.h>
namespace hgl
{
    using namespace platform;

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

            OpenGLSetup gl;

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

        WindowSetup win;

	public:

		GraphicsSystemInitInfo();                                                                   ///<本类构造函数
// 		virtual ~GraphicsSystemInitInfo()=default;													///<本类析构函数

		void Init(const OSStringList &args);														///<使用外部命令行参数

		void ProcBlankValue();                                                              		///<处理未填写的值
	};//class SystemInitInfo

// 	bool GetDesktopDisplayMode(int &,int &,int &,int &);											///<取得当前桌面显示模式
}//namespace hgl
#endif//HGL_GRAPHICS_SYSTEM_INIT_INFO_INCLUDE
