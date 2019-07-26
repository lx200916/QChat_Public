#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<dialog.h>
#include <QMainWindow>
#include <QListWidgetItem>
#include "chatmessage/qnchatmessage.h"
#include<QHostAddress>
#include<QUdpSocket>
#include<QFile>
#include<QTcpServer>
#include<QTcpSocket>
#include<QCloseEvent>
#include<dialog.h>
#include<gmem.h>
#include<bbc.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString webflag;
    BBC *sh;
    void closeEvent(QCloseEvent*event);
    explicit MainWindow(QList<QHostAddress> group,QMap<QString,QString>* mem,Dialog *sp,QString taname,QString flag,QWidget *parent);
    ~MainWindow();
    Dialog *sp;
   Gmem *gmem;
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
QList<QHostAddress> group;
    void resizeEvent(QResizeEvent *event);
    QHostAddress ta;
    QString taname;
    void settext(QString as,QString tatou);
    QUdpSocket *client;
private:
    Ui::MainWindow *ui;
    QTcpSocket *send;
    QFile *file;
    QString fileName;
    /* 总数据大小，已发送数据大小，剩余数据大小，每次发送数据块大小 */
    qint64 fileBytes, sentBytes, restBytes, loadBytes;
public slots:

    void on_pushButton_clicked();
    void on_dou_clicked();
    void start_transfer();
        void continue_transfer(qint64);
        void show_error(QAbstractSocket::SocketError);
        void on_pushButton_2_clicked();

        void on_pushButton_3_clicked();


signals:
    void quit(MainWindow* m);
private slots:
    void on_pushButton_7_clicked();
};





#endif // MAINWINDOW_H
