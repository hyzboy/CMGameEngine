#include<hgl/platform/QT5Application.h>
#include<QApplication>

namespace hgl
{
	QT5GuiApplication::QT5GuiApplication(QApplication *qa):qt_app(qa)
	{
	}

	QT5GuiApplication::~QT5GuiApplication()
	{
	}

	int QT5GuiApplication::exec()
	{
		return qt_app->exec();
	}
}//namespace hgl
