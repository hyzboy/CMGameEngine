#include<hgl/Console.h>
#include<hgl/platform/ConsoleApplication.h>
#include<windows.h>

#if defined(_MSC_VER)&&defined(_DEBUG)
#include<crtdbg.h>
#endif//_MSC_VER&&_DEBUG

using namespace hgl;

int main()
{
#ifdef _DEBUG
	try
	{
#endif//_DEBUG
        ConsoleSystemInitInfo sii;
        ConsoleApplication app;
		StringList<WideString> sl;

		wchar_t **argv;
		int argc;

		argv = CommandLineToArgvW(GetCommandLineW(), &argc);

		for(int i=0;i<argc;i++)
			sl.Add(argv[i]);

		return ConsoleAppMain(sii,app,sl);
#ifdef _DEBUG
	}
	catch(...)
	{
		return(-1);
	}
#endif//_DEBUG
}
