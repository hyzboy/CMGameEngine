#ifndef HGL_QT4_APPLICATION_INCLUDE
#define HGL_QT4_APPLICATION_INCLUDE

class QApplication;
#include<hgl/platform/ConsoleApplication.h>

namespace hgl
{
	class QT4GuiApplication:public ConsoleApplication
	{
		QApplication *qt_app;

	public:

		QT4GuiApplication(QApplication *);
		~QT4GuiApplication();

		int exec();
	};//class QT4GuiApplication
}//namespace hgl

/**
* 控制台程序总入口函数
* @param sii 系统初始化信息类，需由开发者补充填写一定的信息
* @param app 应用程序整体控制管理类
* @param args 由命令行或其它程序传来的参数列表
* @return 返回值，将会回传给操作系统
*/
extern "C" int QT4AppMain(hgl::ConsoleSystemInitInfo &sii,hgl::QT4GuiApplication &app,const hgl::StringList<hgl::OSString> &args);

#endif//HGL_QT4_APPLICATION_INCLUDE
