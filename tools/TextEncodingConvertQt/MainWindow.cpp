#include"MainWindow.h"
#include<QtWidgets/QLabel>
#include<QtWidgets/QLineEdit>
#include<QtWidgets/QPushButton>

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::Init()
{
    this->setWindowTitle("TextEncodingConvert");
    this->resize(400,300);

    {
        QLabel *lab=new QLabel(this);

        lab->setObjectName("input_label");
        lab->setGeometry(QRect(10,10,100,16));
        lab->setText("Input Path");
    }

    {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setObjectName("input_edit");
        lineEdit->setGeometry(QRect(80, 10, 310, 24));
    }

    //{
    //    QPushButton *but=new QPushButton(this);
    //    but->setObjectName("input_path_browser");
    //    but->setGeometry(QRect(330,10,64,24));
    //    but->setText("Browser");
    //}

    {
        QLabel *lab=new QLabel(this);

        lab->setObjectName("output_label");
        lab->setGeometry(QRect(10,40,100,16));
        lab->setText("Output Path");
    }

    {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setObjectName("output_edit");
        lineEdit->setGeometry(QRect(80, 40, 310, 24));
    }

}
