#ifndef CDBTableViewer_H
#define CDBTableViewer_H

#include <QMainWindow>
#include <hgl/type/BaseString.h>

class CDBTableViewer : public QMainWindow
{
Q_OBJECT
public:
    CDBTableViewer();
    virtual ~CDBTableViewer();

	void Load(const hgl::OSString &);
};

#endif // CDBTableViewer_H
