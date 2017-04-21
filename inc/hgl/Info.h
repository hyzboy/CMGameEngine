#ifndef HGL_INFO_INCLUDE
#define HGL_INFO_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl                           				///古月游戏开发库所使用的名字空间
{
	namespace info										//内部功能，请不要使用
	{
		void InitString();								//初始化标志字符串
		void SetString(int,const OSString &);			//设置标志字符串
	}//namespace info

	namespace info
	{
		/**
		* 古月标志字串枚举
		* @see GetString
		*/
		enum HGLFlagString
		{
			hfsNone=0,				///<起始定义，无意义

			hfsName,              	///<名称
			hfsVersion,             ///<版本字串

			hfsCMGDKPath,			///<CMGDK路径
			hfsPlugInPath,			///<CMGDK插件路径
			hfsGUIPath,				///<CMGDK GUI路径
			hfsStartPath,			///<程序启动路径

			hfsOSPath,				///<操作系统路径
			hfsOSLibraryPath,		///<操作系统公用动态库路径
			hfsUserDesktopPath,		///<用户桌面路径
            hfsCommonDesktopPath,   ///<所有用户公用桌面路径
			hfsTempPath,			///<用户临时文件路径
			hfsUserDataPath,		///<用户文件路径(我的数据),(Win2000/XP/2003:Application Data,WinVista/2008/7:AppData\Roaming,unix:~)
			hfsUserProgramPath,		///<用户程序路径(开始菜单)

			hfsAppFileName,			///<当前程序文件名

			hfsEnd,					///<结束定义，无意义
		};//enum HGLFlagString

		const OSString &GetString(HGLFlagString);                                                    ///<取得古月标记字符串
	}//namespace info

	using namespace info;
}//namespace hgl
#endif//HGL_INFO_INCLUDE
