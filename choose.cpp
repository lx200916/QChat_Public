#include "choose.h"
#include "ui_choose.h"
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QJsonDocument>
#include<QJsonObject>

choose::choose(QString username,QString password,QString name,QWidget *parent):
    QWidget(parent),
    ui(new Ui::choose)
{this->username=username;
    this->password=password;
    this->name=name;
    ui->setupUi(this);
}

choose::choose(QString filename,QWidget *parent):
    QWidget(parent),
    ui(new Ui::choose)
{
    ui->setupUi(this);
    this->filename=filename;

}

choose::~choose()
{
    delete ui;
}



void choose::on_tou1_clicked()
{
    tou=":/img/tou1.jpg";
    ui->big->setPixmap(QPixmap(tou));

}

void choose::on_tou2_clicked()
{
    tou=":/img/tou2.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou3_clicked()
{
    tou=":/img/tou3.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou4_clicked()
{
    tou=":/img/tou4.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou5_clicked()
{
    tou=":/img/tou5.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou6_clicked()
{
    tou=":/img/tou6.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou7_clicked()
{
    tou=":/img/tou7.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou8_clicked()
{
    tou=":/img/tou8.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou9_clicked()
{
    tou=":/img/tou9.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou10_clicked()
{
    tou=":/img/tou10.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou11_clicked()
{
    tou=":/img/tou11.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou12_clicked()
{
    tou=":/img/tou12.jpg";
    ui->big->setPixmap(QPixmap(tou));
}


void choose::on_tou13_clicked()
{
    tou=":/img/tou13.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou14_clicked()
{
    tou=":/img/tou14.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou15_clicked()
{
    tou=":/img/tou15.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_tou16_clicked()
{
    tou=":/img/tou16.jpg";
    ui->big->setPixmap(QPixmap(tou));
}

void choose::on_pushButton_clicked()
{QString baseUrl = "http://0.0.0.0:5000/signup/";
    QUrl url(baseUrl);

    // 表单数据
    QByteArray dataArray;
    dataArray.append("Username="+username+"&Password="+password+"&Name="+name+"&Pic="+tou.replace(":",""));


    // 构造请求
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(url);
    QNetworkAccessManager* m_accessManager;
    m_accessManager = new QNetworkAccessManager(this);
        //QObject::connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

    QNetworkReply *pReplay = m_accessManager->post(request, dataArray);

    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(m_accessManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    // 获取响应信息
    QByteArray bytes = pReplay->readAll();

    QJsonParseError e;
    QJsonDocument jsonDoc1 = QJsonDocument::fromJson(bytes, &e);
    QJsonObject object1 =jsonDoc1.object();
    if(object1.value("Msg").toString()=="Success"){
    QMessageBox::warning(NULL, "information", "注册成功！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//    QFile f(filename);
//    f.open(QFile::Append);
//    QTextStream ts(&f);
  //  ts<<":"<<tou.replace(":","")<<endl;}
    this->hide();}
}
