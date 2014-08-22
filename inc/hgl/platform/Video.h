#ifndef HGL_PLATFORM_VIDEO_INCLUDE
#define HGL_PLATFORM_VIDEO_INCLUDE

namespace hgl
{
	/**
	* 平台相关专用名字空间
	*/
	namespace platform
	{
		/**
		* 画面拉伸方式
		*/
		enum CanvasStretch
		{
			csNone=0,				///<起始定义，如使用表示不拉伸

			csEuploid,				///<整倍最大拉伸
			csCorrectAspectRatio,	///<保持纵横比
			csFull,					///<完全拉伸

			csEnd					///<结束定义，如使用表示不拉伸
		};

		/**
		* 窗口设置数据结构
		*/
		struct Window
		{
			bool Edge;										///<是否有边框

			bool SysMenu;									///<是否有系统菜单
			bool Resize;									///<是否可调整大小
			bool Min;										///<是否可最小化
			bool Max;										///<是否可最大化

			bool TopMost;									///<是否永远在最上方

			u16char Name[128];								///<窗口标题

			int Width;										///<窗口宽
			int Height;										///<窗口高

			bool AlignToCenter;								///<是否自动桌面居中
		};

		/**
		* 显示模式数据结构
		*/
		struct VideoMode
		{
			int Width;										///<宽
			int Height;										///<高
			int Bit;										///<色彩位数

			int Refresh;									///<垂直刷新率
			bool RSync;										///<垂直同步
		};

		/**
		* 游戏画布数据结构
		* @see CanvasStretch
		*/
		struct GameCanvas
		{
			int Width;										///<宽
			int Height;										///<高

			int Stretch;									///<拉伸方式
		};

		bool ChangeToWindow(const Window &,const GameCanvas &);										///<设置到窗口模式
		bool ResizeWindow(int,int);																	///<调整窗口大小

		bool ChangeToFullScreen(const VideoMode &,const GameCanvas &);								///<设置到全屏模式
		void RestartVideoMode();																	///<恢复到最初的显示模式
	}//namespace platform

	using namespace platform;
}//namespace hgl
#endif//HGL_PLATFORM_VIDEO_INCLUDE
