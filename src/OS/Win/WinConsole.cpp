#include<hgl/Console.h>
#include<hgl/platform/ConsoleApplication.h>
#include<windows.h>

using namespace hgl;

int main()
{
#ifdef _DEBUG
	try
	{
#endif//_DEBUG
		StringList<UTF16String> sl;

		char16_t **argv;
		int argc;

		argv = CommandLineToArgvW(GetCommandLineW(), &argc);

		for(int i=0;i<argc;i++)
			sl.Add(argv[i]);

		ConsoleSystemInitInfo sii;
		ConsoleApplication app;

		return ConsoleAppMain(sii,app,sl);
#ifdef _DEBUG
	}
	catch(...)
	{
		return(-1);
	}
#endif//_DEBUG
}
