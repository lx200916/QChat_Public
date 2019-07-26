#ifndef RECEVIEFILE_H
#define RECEVIEFILE_H
#include<QHostAddress>
#include<QString>
#include <QMainWindow>
#include <QAbstractSocket>
class QFile;
class QString;
class QTcpServer;
class QTcpSocket;

namespace Ui {
    class RecevieFile;
}

class RecevieFile : public QMainWindow
{
    Q_OBJECT

public:
    explicit RecevieFile(QHostAddress ta,QWidget *parent = 0);
    ~RecevieFile();

private slots:
    void accept_connect();
    void recevie_file();
    void show_error(QAbstractSocket::SocketError);
    void on_listenBtn_clicked();

private:
    QHostAddress tcpip;
    Ui::RecevieFile *ui;
    QTcpServer *server;
    QTcpSocket *receive;
    QString fileName;
    QFile *file;
    /* 已接受数据，总数据，文件名长度 */
    qint64 gotBytes, fileBytes, nameSize;
};

#endif // RECEVIEFILE_H
