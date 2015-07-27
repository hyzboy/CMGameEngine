#ifndef HGL_GRAPHICS_INCLUDE
#define HGL_GRAPHICS_INCLUDE

#include<hgl.h>
#include<hgl/platform/GraphicsSystemInitInfo.h>
#include<hgl/platform/GraphicsApplication.h>

#define HGL_GRAPHICS_MAIN(sii_name,app_name,arg_name)	extern "C" int GraphicsAppMain(hgl::GraphicsSystemInitInfo &sii_name,hgl::graph::GraphicsApplication &app_name,const hgl::StringList<hgl::OSString> &args)

#define HGL_GRAPHICS_MAIN_FUNC()						HGL_GRAPHICS_MAIN(sii,app,args)

#define HGL_GRAPHICS_APPLICATION(name,code,start)	using namespace hgl;	\
													\
													HGL_GRAPHICS_MAIN_FUNC()\
													{	\
														sii.info.ProjectName=U8_TEXT(name);	\
														sii.info.ProjectCode=OS_TEXT(code);	\
														\
														sii.win.Name=U8_TEXT(name);	\
														\
														if(!app.Init(&sii))	\
															return(-1);	\
														\
														app.SetStart(start);	\
														\
														return app.Run();	\
													}
#endif//HGL_GRAPHICS_INCLUDE
