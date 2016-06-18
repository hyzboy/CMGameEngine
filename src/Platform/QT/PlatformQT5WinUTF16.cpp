#include<hgl/Console.h>
#include<windows.h>
#include<hgl/platform/QT5Application.h>
#include<QApplication>

#if (__CODEGEARC__ >= 0x610)// C++Builder 2009 is 0x0610
	extern "C" int WINAPI wWinMain(HINSTANCE,HINSTANCE,wchar_t *,int)
#else
	extern "C" int WINAPI WinMain(HINSTANCE, HINSTANCE, char *, int)
#endif//
{
	wchar_t **w_argv;
	char **argv;
	int argc;

	w_argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	hgl::StringList<hgl::UTF16String> sl;
	hgl::StringList<hgl::UTF8String> sl8;

	argv = new char *[argc];

	for (int i = 0; i < argc; i++)
	{
		sl.Add(w_argv[i]);
		sl8.Add(hgl::to_u8(w_argv[i]));

		argv[i] = sl8[i].c_str();
	}
	
	QApplication qt_app(argc,argv);

	hgl::ConsoleSystemInitInfo sii;
	hgl::QT5GuiApplication app(&qt_app);

	delete[] argv;
	return QT5AppMain(sii, app, sl);
}
