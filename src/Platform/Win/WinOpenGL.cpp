#include<hgl/Graphics.h>
#include<hgl/platform/GraphicsApplication.h>
#include<hgl/type/StringList.h>

#include<windows.h>

#if defined(_MSC_VER)&&defined(_DEBUG)
#include<crtdbg.h>
#endif//_MSC_VER&&_DEBUG

using namespace hgl;
using namespace hgl::graph;

extern "C" int WINAPI WinMain(HINSTANCE,HINSTANCE,char *,int)
{
#ifdef _DEBUG
	try
	{
#endif//_DEBUG
        GraphicsSystemInitInfo sii;
        GraphicsApplication app;
        StringList<WideString> sl;

		wchar_t **argv;
		int argc;

		argv = CommandLineToArgvW(GetCommandLineW(), &argc);

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
