#include "CDBTableViewer.h"
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <hgl/db/CDBTable.h>
#include <hgl/QT4.h>

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

		title.fromUtf8("CDBTableViewer - file:"+filename+" col:"+hgl::UTF8String(col_count)+" row:"+hgl::UTF8String(row_count));
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

#include "CDBTableViewer.moc"
