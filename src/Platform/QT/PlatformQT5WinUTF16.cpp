#include<hgl/Console.h>
#include<windows.h>
#include<hgl/platform/QT5Application.h>
#include<QApplication>

extern "C" int WINAPI wWinMain(HINSTANCE,HINSTANCE,wchar_t *cmd_line,int)
{
    wchar_t **w_argv;
    char **argv;
    int argc;

    w_argv = CommandLineToArgvW(cmd_line, &argc);

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
