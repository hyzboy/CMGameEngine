#ifndef HGL_CONSOLE_INCLUDE
#define HGL_CONSOLE_INCLUDE

#include<hgl.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/platform/ConsoleApplication.h>

#define HGL_CONSOLE_MAIN(sii_name,app_name,arg_name)	extern "C" int ConsoleAppMain(hgl::ConsoleSystemInitInfo &sii_name,hgl::ConsoleApplication &app_name,const hgl::StringList<hgl::OSString> &arg_name)

#define HGL_CONSOLE_MAIN_FUNC()							using namespace hgl;    \
                                                        HGL_CONSOLE_MAIN(sii,app,args)

#define HGL_CONSOLE_APPLICATION(name,code,start)	using namespace hgl;	\
													\
													HGL_CONSOLE_MAIN_FUNC()\
													{	\
														sii.info.ProjectName=OS_TEXT(name);	\
														sii.info.ProjectCode=OS_TEXT(code);	\
														\
														if(!app.Init(&sii))	\
															return(-1);	\
														\
														app.SetStart(start);	\
														\
														return app.Run();	\
													}
#endif//HGL_CONSOLE_INCLUDE
