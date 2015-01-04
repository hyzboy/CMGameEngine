#ifndef HGL_QT5_APPLICATION_INCLUDE
#define HGL_QT5_APPLICATION_INCLUDE

class QApplication;
#include<hgl/platform/ConsoleApplication.h>

namespace hgl
{
	class QT5GuiApplication:public ConsoleApplication
	{
		QApplication *qt_app;

	public:

		QT5GuiApplication(QApplication *);
		~QT5GuiApplication();

		int exec();
	};//class QT5GuiApplication
}//namespace hgl

/**
* 控制台程序总入口函数
* @param sii 系统初始化信息类，需由开发者补充填写一定的信息
* @param app 应用程序整体控制管理类
* @param args 由命令行或其它程序传来的参数列表
* @return 返回值，将会回传给操作系统
*/
extern "C" int QT5AppMain(hgl::ConsoleSystemInitInfo &sii,hgl::QT5GuiApplication &app,const hgl::StringList<hgl::OSString> &args);

#endif//HGL_QT5_APPLICATION_INCLUDE
