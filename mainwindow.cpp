#include "mainwindow.h"
#include "externa.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include<QFile>
#include<QJsonObject>
#include<QJsonDocument>
#include<QMessageBox>
#include<QFileDialog>
#include<QComboBox>
const qint64 LOADBYTES = 4 * 1024;
const int DATA_STREAM_VERSION = QDataStream::Qt_5_13;
MainWindow::MainWindow(QList<QHostAddress> group,QMap<QString,QString>* mem, Dialog *sp,QString taname,QString flag,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{this->ta=group[0];
    resize(750,800);
    webflag=flag;
    this->group=group;
    this->sp=sp;
    ui->setupUi(this);
    ui->pushButton_7->setIcon(QIcon(QPixmap(":/img/more.png")));
    ui->progressBar->hide();
    if(group.length()!=1||flag.length()!=0){
        ui->dou->hide();
    }
    this->taname=taname;
    ui->label->setText(taname);
    this->setWindowTitle("Chat With "+taname);
    QFile file("./"+taname+".ini");
    if(file.open(QFile::ReadOnly|QFile::Text)){

        qDebug()<<"1";

//        ui->texte->setText(file.readAll());}
//    ui->texte->hide();
    }
    this->client=new QUdpSocket(this);
    if(mem){
        gmem=new Gmem(*mem);
        gmem->show();
    }
    QStringList sstt;
    ui->widget_2->hide();
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(on_dou_clicked()));
    sstt << "一 般 发 情 夏 色 祭";
    //.ui->comboBox->itemText(sstt);
}
void MainWindow::closeEvent(QCloseEvent*event){
    // qDebug()<<"1rsee"<<endl;
    QString s=sp->chatlist.key(this);
    qDebug () <<"qu"<<s<<endl;
    sp->chatlist.remove(s);
    //    QFile file("./"+taname+".ini");
    //    file.open(QFile::Append|QFile::Text);
    //    QTextStream in(&file);
    //      // in<<ui->textEdit->toPlainText();
    //     file.close();
}
MainWindow::~MainWindow()
{
    delete ui;
    QFile file("./"+taname+".ini");
    file.open(QFile::Append|QFile::Text);
    QTextStream in(&file);
    // in<<ui->textEdit->toPlainText();
    file.close();
    QString s=sp->chatlist.key(this);
    sp->chatlist.remove(s);
}

void MainWindow::on_pushButton_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    bool isSending = false; // 发送中
    qDebug()<<"addMessage" << msg << time << ui->listWidget->count();

    if(isSending) {
        dealMessageTime(time);

        QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
    } else {
        bool isOver = true;
        for(int i = ui->listWidget->count() - 1; i > 0; i--) {
            QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
            if(messageW->text() == msg) {
                isOver = false;
                messageW->setTextSuccess();
            }
        }
        if(isOver) {
            dealMessageTime(time);
            QJsonObject json;
            json.insert("Name", mname);
            json.insert("Pic", mtou);
            if(group.length()==1){
                json.insert("Msg",msg.toUtf8().data());
            json.insert("target",taname);
            }
            else {
                json.insert("GMsg",msg.toUtf8().data());
            }
            QJsonDocument jsonDoc(json);
            QByteArray ba = jsonDoc.toJson();
            //ui->texte->append("您:"+msg);
            QFile file("./"+taname+".ini");
            file.open(QFile::Append|QFile::Text);
            QTextStream in(&file);
            in<<"您:"+msg<<endl;
            file.close(); if(webflag.length()==0){
            foreach(QHostAddress ta,group){
             this->client->writeDatagram(ba,ba.size(),ta,6666);}}else {
sp->dataRecvWS->sendTextMessage(ba);
}
            QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            messageW->setpicSuccess(mtou);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);

            messageW->setTextSuccess();
        }
    }

    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void MainWindow::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
        //        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}
void MainWindow::settext(QString as,QString tatou){
    if(as != "") {QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        dealMessageTime(time);
        QFile file("./"+taname+".ini");
        file.open(QFile::Append|QFile::Text);
        QTextStream in(&file);
        in<<taname+":"+as<<endl;
        file.close();
        QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        messageW->setpicSuccess(mtou,tatou);
        dealMessage(messageW, item, as, time, QNChatMessage::User_She);
    }
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);


//    ui->textEdit->resize(this->width() - 20, ui->widget->height() - 80);
//    ui->textEdit->move(10, 80);

//    ui->pushButton->move(ui->textEdit->width()+ui->textEdit->x() - ui->pushButton->width() ,
//                         ui->textEdit->height()+ui->textEdit->y() - ui->pushButton->height() + 60);


    for(int i = 0; i < ui->listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    }
}

void MainWindow::on_dou_clicked()
{
    QMessageBox message(QMessageBox::Warning,"Choose","Do you want to send files?",QMessageBox::Yes|QMessageBox::No,NULL);
    if (message.exec()==QMessageBox::Yes)//确定，加入列表
    {
        fileName=QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty()){
            //File *ss=new File(ta);
            //ss->show();
            send = new QTcpSocket(this);
            fileBytes = sentBytes = restBytes = 0;
            loadBytes = LOADBYTES;
            file = Q_NULLPTR;
            ui->progressBar->show();
            ui->progressBar->setValue(0); // 进度条置零
            //                       ui->sendBtn->setEnabled(false); // 禁用发送按钮
            connect(send, SIGNAL(connected()),
                    this, SLOT(start_transfer()));
            connect(send, SIGNAL(bytesWritten(qint64)),/* 数据已发出 -> 继续发 */
                    this, SLOT(continue_transfer(qint64)));
            /* socket出错 -> 错误处理 */
            connect(send, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(show_error(QAbstractSocket::SocketError)));
            QJsonObject json;
            //发送请求
            json.insert("File", mname);
            json.insert("Filename",fileName.right(fileName.size() -
                                                  fileName.lastIndexOf('/') - 1));



            QJsonDocument jsonDoc(json);
            QByteArray ba = jsonDoc.toJson();

            this->client->writeDatagram(ba,ta,6666);
            send->connectToHost(ta, 9999);
            sentBytes = 0;
        }else {
            qDebug()<<"None!!";
        }

    }
    else
    {

    }
}
void MainWindow::start_transfer(){
    file = new QFile(fileName);
    if(!file->open(QFile::ReadOnly))
    {
        qDebug() << "*** start_transfer(): File-Open-Error";
        return;
    }fileBytes = file->size();

    ui->progressBar->setValue(0);
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
    ui->progressBar->setMaximum(fileBytes);
    /* 重写首部的前两个长度字段 */
    out.device()->seek(0);
    out << fileBytes << (qint64(buf.size()) - 2 * sizeof(qint64));
    /* 发送首部，计算剩余大小 */
    restBytes = fileBytes - send->write(buf);
}
void MainWindow::continue_transfer(qint64 sentSize)
{
    sentBytes += sentSize;
    ui->progressBar->setValue(sentBytes);
    /* 还有数据要发 */
    if(restBytes > 0)
    {
        /* 从文件读数据 */
        QByteArray buf = file->read((loadBytes, restBytes));
        /* 发送 */
        restBytes -= send->write(buf);
    }
    else
        file->close();
    /* 全部发送完 */
    if(sentBytes == fileBytes)
    {
        send->close(); // 关socket
        fileName.clear(); // 清空文件名
    }
}
void MainWindow::show_error(QAbstractSocket::SocketError)
{
    qDebug() << "*** Socket Error";
    send->close();
    // ui->sendBtn->setEnabled(true);
    ui->progressBar->reset(); // 进度条归零
    fileName.clear();
}


void MainWindow::on_pushButton_2_clicked()
{
    QJsonObject json;
    qDebug()<<"poked!"<<endl;
    json.insert("chocho", mname);
    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();
    foreach(QHostAddress ta,group){
        this->client->writeDatagram(ba,ba.size(),ta,6666);
        qDebug()<<"dend:"<<ba<<" "<<ta<<endl;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
     sh=new BBC(taname);
     sh->show();
}



void MainWindow::on_pushButton_7_clicked()
{if(ui->pushButton_7->isChecked()){
        ui->widget_2->hide();
        ui->pushButton_7->setIcon(QIcon(QPixmap(":/img/more2.png")));
        //ui->pushButton_7->setStyleSheet("border-image: url(:/img/more2.png);background-color: rgba(255, 255, 255, 0);");
    }else {
        ui->widget_2->show();
        //ui->pushButton_7->setStyleSheet("border-image: url(:/img/more2.png);background-color: rgba(255, 255, 255, 0);");
          ui->pushButton_7->setIcon(QIcon(QPixmap(":/img/more.png")));


    }


}
