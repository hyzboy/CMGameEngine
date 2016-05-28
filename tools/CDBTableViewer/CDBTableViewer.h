#ifndef CDBTableViewer_H
#define CDBTableViewer_H

#ifdef HGL_QT4
    #include <QtGui/QMainWindow>
#else
    #include <QMainWindow>
#endif//HGL_QT4

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
