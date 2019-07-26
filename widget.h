#ifndef WIDGET_H
#define WIDGET_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include "loginwindow.h"
#include<dialog.h>
#include <QMessageBox>
#include<externa.h>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_login_clicked();

    void on_sign_in_clicked();

private:
    Ui::Widget *ui;
    loginwindow *lw;
    QString filename="./config.ini";
    Dialog *zjm;
};

#endif // WIDGET_H
