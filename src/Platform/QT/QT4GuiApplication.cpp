#include<hgl/platform/QT4Application.h>
#include<QtGui/QApplication>

namespace hgl
{
	QT4GuiApplication::QT4GuiApplication(QApplication *qa):qt_app(qa)
	{
	}

	QT4GuiApplication::~QT4GuiApplication()
	{
	}

	int QT4GuiApplication::exec()
	{
		return qt_app->exec();
	}
}//namespace hgl
