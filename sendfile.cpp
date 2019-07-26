#include "SendFile.h"
#include "ui_SendFile.h"
#include <QByteArray>
#include <QDataStream>
#include <QFileDialog>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <QTcpSocket>
const quint16 PORT = 9999;
const qint64 LOADBYTES = 4 * 1024; // 4 kilo-byte
const int DATA_STREAM_VERSION = QDataStream::Qt_5_13;

SendFile::SendFile(QHostAddress ta,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SendFile)
{
    ui->setupUi(this);
    tcpip=ta;
    send = new QTcpSocket(this);
    fileBytes = sentBytes = restBytes = 0;
    loadBytes = LOADBYTES;
    file = Q_NULLPTR;
    ui->sendProg->setValue(0); // 进度条置零
    ui->sendBtn->setEnabled(false); // 禁用发送按钮
    //ui->cancelBtn->setEnabled(false);
    connect(send, SIGNAL(connected()),this, SLOT(start_transfer()));//连接建立
    connect(send, SIGNAL(bytesWritten(qint64)),this, SLOT(continue_transfer(qint64)));//数据传输
    connect(send, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(show_error(QAbstractSocket::SocketError)));//错误处理(socket)
}

SendFile::~SendFile()
{
    delete ui;
    delete send;
}
void SendFile::on_selectBtn_clicked()//选择按钮
{
    fileName = QFileDialog::getOpenFileName(this);//选择文件
    if(!fileName.isEmpty())
    {
        ui->stLabel->setText(
            QString("File %1 Opened!").arg(fileName));
        ui->sendBtn->setEnabled(true);
    }
    else
        ui->stLabel->setText(QString("*** FAIL OPENING FILE"));
}

void SendFile::on_sendBtn_clicked()//发送按钮
{
    send->connectToHost(tcpip, PORT);//发送请求，对应监听
    sentBytes = 0;
    ui->sendBtn->setEnabled(false);
   // ui->cancelBtn->setEnabled(true);
    ui->stLabel->setText(QString("Linking..."));
}
void SendFile::start_transfer()//开始发送
{
    file = new QFile(fileName);
    if(!file->open(QFile::ReadOnly))
    {
        ui->stLabel->setText(QString("FILE OPEN ERROR!!"));
        qDebug() << "start_transfer(): File-Open-Error!!";
        return;
    }

    fileBytes = file->size();
    ui->sendProg->setValue(0);
    ui->stLabel->setText(QString("Connection Established!"));

    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);
    out.setVersion(DATA_STREAM_VERSION);

    /* 无路径文件名 */
    QString sfName = fileName.right(fileName.size() -
            fileName.lastIndexOf('/') - 1);
    /* 首部 = 总大小 + 文件名长度 + 文件名 */
    out << qint64(0) << qint64(0) << sfName;
    /* 总大小加上首部的大小 */
    fileBytes += buf.size();
    ui->sendProg->setMaximum(fileBytes);
    /* 重写首部的前两个长度字段 */
    out.device()->seek(0);
    out << fileBytes << (qint64(buf.size()) - 2 * sizeof(qint64));
    /* 发送首部，计算剩余大小 */
    restBytes = fileBytes - send->write(buf);
}
void SendFile::continue_transfer(qint64 sentSize)//继续传输
{
    sentBytes += sentSize;
    ui->sendProg->setValue(sentBytes);
    if(restBytes > 0)//未完成
    {
        //读取，发送
        QByteArray buf = file->read(qMin(loadBytes, restBytes));
        restBytes -= send->write(buf);
    }
    else
        file->close();
    if(sentBytes == fileBytes)//完成
    {
        send->close(); // 关socket
        fileName.clear(); // 清空文件名
        ui->stLabel->setText(QString("Finish sending!"));
    }
}
void SendFile::show_error(QAbstractSocket::SocketError)//错误处理
{
    qDebug() << "Socket Error!!";
    send->close();
    ui->stLabel->setText(QString("*** SOCKET ERROR, RESEND LATER"));
    ui->sendBtn->setEnabled(true);
    ui->sendProg->reset(); // 进度条归零
    fileName.clear();
}
