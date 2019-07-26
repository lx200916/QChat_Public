#ifndef CHOOSE_H
#define CHOOSE_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class choose;
}

class choose : public QWidget
{
    Q_OBJECT

public:QString username,password,name;
    explicit choose(QString username,QString password,QString name,QWidget *parent = nullptr);
    explicit choose(QString filename,QWidget *parent = nullptr);
    ~choose();

private slots:



    void on_tou1_clicked();

    void on_tou2_clicked();

    void on_tou3_clicked();

    void on_tou4_clicked();

    void on_tou5_clicked();

    void on_tou6_clicked();

    void on_tou7_clicked();

    void on_tou8_clicked();

    void on_tou9_clicked();

    void on_tou10_clicked();

    void on_tou11_clicked();

    void on_tou12_clicked();


    void on_tou13_clicked();

    void on_tou14_clicked();

    void on_tou15_clicked();

    void on_tou16_clicked();

    void on_pushButton_clicked();

private:
    Ui::choose *ui;
    QString filename;
    QString tou;
};

#endif // CHOOSE_H
