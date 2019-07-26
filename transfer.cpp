#include "transfer.h"
#include "ui_transfer.h"
#include <iostream>
#include <QDataStream>
#include <QFile>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include<QMessageBox>
const int DATA_STREAM_VERSION = QDataStream::Qt_5_13;
transfer::transfer(QHostAddress ta,QString Filename,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::transfer)
{
    ui->setupUi(this);
    tcpip=ta;

        ui->setupUi(this);
          /* 进度条调零 */
          ui->recvProg->setValue(0);
          /* 启用监听按钮 */

          fileBytes = gotBytes = nameSize = 0;
          file = Q_NULLPTR;
          ui->label->setText(Filename);
          receiver = Q_NULLPTR;
          server = new QTcpServer(this);
          /* 连接请求 -> 接受连接 */
          connect(server, SIGNAL(newConnection()),
                  this, SLOT(accept_connect()));
          server->listen(QHostAddress::Any, 9999);
         qDebug()<<"listen";
}

transfer::~transfer()
{
    delete ui;
    delete server;
}
void transfer::accept_connect()
{
    receiver = server->nextPendingConnection();
    /* 有数据到 -> 接受数据 */
    connect(receiver, SIGNAL(readyRead()),
            this, SLOT(recevie_file()));
    /* socket出错 -> 出错处理 */
    connect(receiver, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(show_error(QAbstractSocket::SocketError)));
    gotBytes = 0;
    server->close();
}
void transfer::recevie_file()
{
    QDataStream in(receiver);
    in.setVersion(DATA_STREAM_VERSION);

    /* 首部未接收/未接收完 */
    if(gotBytes <= 2 * sizeof(qint64))
    {
        if(!nameSize) // 前两个长度字段未接收
        {
            if(receiver->bytesAvailable() >= 2 * sizeof(qint64))
            {
                in >> fileBytes >> nameSize;
                gotBytes += 2 * sizeof(qint64);
                ui->recvProg->setMaximum(fileBytes);
                ui->recvProg->setValue(gotBytes);
            }
            else // 数据不足，等下次
               return;
        }
        else if(receiver->bytesAvailable() >= nameSize)
        {
            in >> fileName;
            gotBytes += nameSize;
            ui->recvProg->setValue(gotBytes);
            qDebug() << "--- File Name: "<< fileName << endl;
        }
        else // 数据不足文件名长度，等下次
            return;
    }
    if(!fileName.isEmpty() && file == Q_NULLPTR)
        {
            file = new QFile(fileName);
            if(!file->open(QFile::WriteOnly)) // 打开失败
            {
                qDebug() << "*** File Open Failed ***" << endl;
                delete file;
                file = Q_NULLPTR;
                return;
            }
        }
        if(file == Q_NULLPTR) // 文件未打开，不能进行后续操作
            return;

        if(gotBytes < fileBytes) // 文件未接收完
        {
            gotBytes += receiver->bytesAvailable();
            ui->recvProg->setValue(gotBytes);
            file->write(receiver->readAll());
        }
        if(gotBytes == fileBytes) // 文件接收完
        {
            receiver->close(); // 关socket
            file->close(); // 关文件
            delete file;
            QMessageBox::information(this,"Complete!","Transfer Done!","Ok");
        }
    }
void transfer::show_error(QAbstractSocket::SocketError)
{
    std::cerr << "*** Socket Error ***" << std::endl;
    qDebug() << receiver->errorString();
    receiver->close(); // 关cocket
    receiver = Q_NULLPTR;
    file = Q_NULLPTR;
    fileName.clear(); // 清空文件名
    fileBytes = gotBytes = nameSize = 0;
    ui->recvProg->reset(); // 进度条归零
}
