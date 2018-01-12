#include"MainWindow.h"
#include<hgl/QT.h>

HGL_QT_MAIN_FUNC()
{
	sii.info.ProjectName=U8_TEXT("文本编码转换工具");
	sii.info.ProjectCode=OS_TEXT("TextEncodingConvert");

	app.Init(&sii);

    MainWindow win;

    win.Init();
    win.show();
    return app.exec();
}
