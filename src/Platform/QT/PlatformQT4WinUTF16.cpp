#include<hgl/Console.h>
#include<windows.h>
#include<hgl/platform/QT4Application.h>
#include<QtGui/QApplication>

using namespace hgl;

#if (__CODEGEARC__ >= 0x610)// C++Builder 2009 is 0x0610
	extern "C" int WINAPI wWinMain(HINSTANCE,HINSTANCE,wchar_t *,int)
#else
	extern "C" int WINAPI WinMain(HINSTANCE,HINSTANCE,char *,int)
#endif//
{
	wchar_t **argv;
	int argc;

	argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	QApplication qt_app(argc,argv);

	StringList<UTF16String> sl;

	for(int i=0;i<argc;i++)
		sl.Add(argv[i]);

	ConsoleSystemInitInfo sii;
	QT4GuiApplication app(&qt_app);

	return QT4AppMain(sii,app,sl);
}
