#include<hgl/Console.h>
#include<hgl/platform/QT5Application.h>
#include<QApplication>

using namespace hgl;

int main(int argc,char **argv)
{
	QApplication qt_app(argc,argv);

	StringList<UTF8String> sl;

	for(int i=0;i<argc;i++)
		sl.Add(argv[i]);

	ConsoleSystemInitInfo sii;
	QT5GuiApplication app(&qt_app);

	return QT5AppMain(sii,app,sl);
}
