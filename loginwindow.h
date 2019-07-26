#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include<choose.h>
#include <QMessageBox>
#include <QDebug>
namespace Ui {
class loginwindow;
}

class loginwindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = 0);
    explicit loginwindow(QString fn,QWidget *parent = 0);

    ~loginwindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::loginwindow *ui;
    QString filename;
choose *ch;

};

#endif // LOGINWINDOW_H
