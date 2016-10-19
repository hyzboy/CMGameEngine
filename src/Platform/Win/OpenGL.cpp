#include<hgl/Graphics.h>
#include<hgl/platform/GraphicsApplication.h>
#include<hgl/type/StringList.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif//WIN32_LEAN_AND_MEAN

#include<windows.h>

#if defined(_MSC_VER)&&defined(_DEBUG)
#include<crtdbg.h>
#endif//_MSC_VER&&_DEBUG

using namespace hgl;
using namespace hgl::graph;

extern "C" int WINAPI wWinMain(HINSTANCE,HINSTANCE,wchar_t *cmd_line,int)
{
#ifdef _DEBUG
	try
	{
#endif//_DEBUG
        GraphicsSystemInitInfo sii;
        GraphicsApplication app;
		OSStringList sl;

		wchar_t **argv;
		int argc;

		argv = CommandLineToArgvW(cmd_line, &argc);

		for(int i=0;i<argc;i++)
			sl.Add(argv[i]);

		return GraphicsAppMain(sii,app,sl);
#ifdef _DEBUG
	}
	catch(...)
	{
		return(-1);
	}
#endif//_DEBUG
}
