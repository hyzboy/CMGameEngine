#include<hgl/platform/QT4Application.h>
#include<QtGui/QApplication>

namespace hgl
{
	QTGuiApplication::QTGuiApplication(QApplication *qa):qt_app(qa)
	{
	}

	QTGuiApplication::~QTGuiApplication()
	{
	}

	int QTGuiApplication::exec()
	{
		return qt_app->exec();
	}
}//namespace hgl
