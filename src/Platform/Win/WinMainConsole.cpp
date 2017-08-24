#include<hgl/Console.h>
#include<hgl/platform/ConsoleApplication.h>
#include<windows.h>

#if defined(_MSC_VER)&&defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC 
#include<crtdbg.h>
#endif//_MSC_VER&&_DEBUG

using namespace hgl;

int wmain(int argc,wchar_t **argv)
{
#ifdef _DEBUG
    
#ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//_MSC_VER
    
    try
    {
#endif//_DEBUG
        StringList<WideString> sl;

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
