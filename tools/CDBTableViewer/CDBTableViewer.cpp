#include "CDBTableViewer.h"
#include <hgl/db/CDBTable.h>

#include <hgl/QT.h>

#include <QTableWidget>
#include <QTableWidgetItem>

CDBTableViewer::CDBTableViewer()
{
}

CDBTableViewer::~CDBTableViewer()
{}

void CDBTableViewer::Load(const hgl::OSString &filename)
{
    if(filename.IsEmpty())
        return;

	hgl::db::CDBTable tab;

	if(!tab.Load(filename))return;

	int col_count=tab.Fields.GetCount();
	int row_count=tab.GetDataCount();

	{
		QString title;

#if HGL_OS == HGL_OS_Windows
		hgl::UTF8String u8_filename = to_u8(filename);

		title.fromUtf8("CDBTableViewer - file:"+ u8_filename +" col:"+hgl::UTF8String(col_count)+" row:"+hgl::UTF8String(row_count));
#else
        title.fromUtf8("CDBTableViewer - file:"+ filename +" col:"+hgl::UTF8String(col_count)+" row:"+hgl::UTF8String(row_count));
#endif//

		this->setWindowTitle(title);
	}

	QTableWidget *tw=new QTableWidget(row_count,col_count,this);

	tw->setSortingEnabled(false);

	QStringList header;

	for(int col=0;col<col_count;col++)
	{
		hgl::UTF16String name;
		hgl::UTF16String info;
		hgl::db::CDBField *fields;

		tab.Fields.Get(col,name,fields);

		header<<toQString(name);

		for(int row=0;row<row_count;row++)
		{
			if(fields->ToString(row,info))
				tw->setItem(row,col,new QTableWidgetItem(toQString(info)));
		}
	}

	tw->setHorizontalHeaderLabels(header);

	tw->resizeRowsToContents();
// 	tw->resizeColumnsToContents();

	setCentralWidget(tw);
}
