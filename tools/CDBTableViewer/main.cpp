#include"CDBTableViewer.h"
#include<hgl/QT4.h>

HGL_QT_MAIN_FUNC()
{
	sii.info.ProjectName=OS_TEXT("CDB表格查看器");
	sii.info.ProjectCode=OS_TEXT("CDBTableViewer");

	app.Init(&sii);

    CDBTableViewer foo;

	foo.Load(args[1]);
//    foo.show();
	foo.showMaximized();
    return app.exec();
}
