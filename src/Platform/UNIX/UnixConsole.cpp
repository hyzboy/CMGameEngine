#include<hgl/Console.h>
#include<hgl/platform/ConsoleApplication.h>

using namespace hgl;

int main(int argc,char **argv)
{
#ifdef _DEBUG
	try
	{
#endif//_DEBUG
		StringList<UTF8String> sl;

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
