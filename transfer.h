#ifndef TRANSFER_H
#define TRANSFER_H

#include <QMainWindow>
#include <QAbstractSocket>
#include<QHostAddress>
class QFile;
class QString;
class QTcpServer;
class QTcpSocket;
namespace Ui {
class transfer;
}

class transfer : public QMainWindow
{
    Q_OBJECT

public:
    explicit transfer(QHostAddress ta,QString Filename,QWidget *parent = nullptr);
    ~transfer();
private slots:
       void accept_connect();
       void recevie_file();
       void show_error(QAbstractSocket::SocketError);

private:

       QTcpServer *server;
       QTcpSocket *receiver;
       QString fileName;
       QFile *file;
   qint64 gotBytes, fileBytes, nameSize;
    QHostAddress tcpip;
    Ui::transfer *ui;
};

#endif // TRANSFER_H
