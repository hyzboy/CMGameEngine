#ifndef HGL_GRAPHICS_INCLUDE
#define HGL_GRAPHICS_INCLUDE

#include<hgl.h>
#include<hgl/platform/GraphicsSystemInitInfo.h>
#include<hgl/platform/GraphicsApplication.h>

// #if !defined(__MAKE_HGL__) && !defined(__MAKE_PLUGIN__)
// 	#pragma comment(lib,HGL_LIB_Base)         	//连入基础库文件
// 	#pragma comment(lib,HGL_LIB_APR)			//连入Apache Portable Runtime
// 	#pragma comment(lib,HGL_LIB_Network)      	//连入网络库文件
// 	#pragma comment(lib,HGL_LIB_OpenALEE)		//连入OpenALEE音频库文件
// 	#pragma comment(lib,HGL_LIB_Graphics)		//连入图形库文件
// 	#pragma comment(lib,HGL_LIB_GUI)			//连入图形用户界面库文件
// 	#pragma comment(lib,HGL_LIB_UtilityTools) 	//连入实用工具库文件
// 	#pragma comment(lib,HGL_LIB_WinExternal)	//连入标准Windows平台接口库文件
//
//     #ifdef _MSC_VER                             //如果是Visual C++
//         #pragma comment(lib,"OpenGL32.LIB")		//连入OpenGL库
//         #pragma comment(lib,"GLU32.LIB")		//连入GLU库
//     #endif//_MSC_VER
//
//     #ifndef _WINDLL								//如果是DLL则不连入WinMain接口库文件
//         #pragma comment(lib,HGL_LIB_WinGame)	//加入标准Windows平台WinMain接口库文件
//     #endif//_WINDLL
// #endif//__MAKE_HGL__ && __MAKE_PLUGIN__

#define HGL_GRAPHICS_MAIN(sii_name,app_name,arg_name)	extern "C" int GraphicsAppMain(hgl::GraphicsSystemInitInfo &sii_name,hgl::graph::GraphicsApplication &app_name,const hgl::StringList<hgl::OSString> &args)

#define HGL_GRAPHICS_MAIN_FUNC()						HGL_GRAPHICS_MAIN(sii,app,args)

#define HGL_GRAPHICS_APPLICATION(name,code,start)	using namespace hgl;	\
													\
													HGL_GRAPHICS_MAIN_FUNC()\
													{	\
														sii.info.ProjectName=name;	\
														sii.info.ProjectCode=code;	\
														\
														sii.win.Name=name;	\
														\
														if(!app.Init(&sii))	\
															return(-1);	\
														\
														app.SetStart(start);	\
														\
														return app.Run();	\
													}
#endif//HGL_GRAPHICS_INCLUDE
