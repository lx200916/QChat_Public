#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QAbstractSocket>
#include<QHostAddress>
class QByteArray;
class QFile;
class QString;
class QTcpSocket;

namespace Ui {
    class SendFile;
}

class SendFile : public QMainWindow
{
    Q_OBJECT

public:
    explicit SendFile(QHostAddress ta,QWidget *parent = 0);
    ~SendFile();

private slots:
    void start_transfer();
    void continue_transfer(qint64);
    void show_error(QAbstractSocket::SocketError);
    void on_selectBtn_clicked();
    void on_sendBtn_clicked();

private:
    Ui::SendFile *ui;
    QTcpSocket *send;
    QFile *file;
    QString fileName;
    QHostAddress tcpip;
    /* 总数据大小，已发送数据大小，剩余数据大小，每次发送数据块大小 */
    qint64 fileBytes, sentBytes, restBytes, loadBytes;
};

#endif // MAINWINDOW_H
