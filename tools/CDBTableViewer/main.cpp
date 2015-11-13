#include"CDBTableViewer.h"
#include<hgl/QT4.h>

HGL_QT_MAIN_FUNC()
{
    const hgl::OSString &filename=args[1];

    if(filename.IsEmpty())
        return(-1);

	sii.info.ProjectName=OS_TEXT("CDB表格查看器");
	sii.info.ProjectCode=OS_TEXT("CDBTableViewer");

	app.Init(&sii);

    CDBTableViewer foo;

	foo.Load(filename);
//    foo.show();
	foo.showMaximized();
    return app.exec();
}
