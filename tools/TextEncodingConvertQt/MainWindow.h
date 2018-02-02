#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
Q_OBJECT
public:

    MainWindow();
    virtual ~MainWindow();

    void Init();
};
