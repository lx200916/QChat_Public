#include "mainwindow.h"
#include <QApplication>
#include<dialog.h>
#include<hello.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    HELLO ww;
    ww.show();
    return a.exec();
}
