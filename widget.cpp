#include "widget.h"
#include "ui_widget.h"
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QMovie>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(" ");
    QMovie *movie = new QMovie(":/img/bl.gif");
    ui->background->setMovie(movie);
    movie->start();
    ui->ID->setPlaceholderText("Username");
    ui->password->setPlaceholderText("Password");
    ui->ID->setAlignment(Qt::AlignHCenter);
    ui->password->setAlignment(Qt::AlignHCenter);
    ui->password->setEchoMode(QLineEdit::Password);
    ui->sign_in->setShortcut(Qt::Key_Return);
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_login_clicked()
{
    lw=new loginwindow(filename);
    lw->show();
}

void Widget::on_sign_in_clicked()
{
//    QFile f(filename);
//    f.open(QFile::ReadOnly);
//    QString read;
//    int a=1;//判断标识

    if(ui->ID->text().length()==0||ui->password->text().length()==0)//若用户名或密码为空，则提示用户名或密码不能为空。
    {
        QMessageBox::warning(NULL, "warning", "用户名或密码不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    QString baseUrl = "http://0.0.0.0:5000/signin/";
    QUrl url(baseUrl);

    // 表单数据
    QByteArray dataArray;
    dataArray.append("Username="+ui->ID->text()+"&Password="+ui->password->text());


    // 构造请求
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(url);
    QNetworkAccessManager* m_accessManager;
    m_accessManager = new QNetworkAccessManager(this);
       // QObject::connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

    QNetworkReply *pReplay = m_accessManager->post(request, dataArray);;

    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(m_accessManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    // 获取响应信息
    QByteArray bytes = pReplay->readAll();

    QJsonParseError e;
    QJsonDocument jsonDoc1 = QJsonDocument::fromJson(bytes, &e);
    QJsonObject object1 =jsonDoc1.object();

       if(object1.value("Msg").toString()=="Wrong")
       {


                QMessageBox::warning(NULL, "warning", "用户名或密码错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
              ui->password->clear();


           }
          if(object1.value("Msg").toString()=="Success")
           {
               //这里登陆成功，传给后面窗口用户名，关闭登陆界面。
    mtou=object1.value("Pic").toString();
               mname = object1.value("Name").toString();


               QString str=object1.value("Mem").toString();
               QJsonParseError e;
               QJsonDocument jsonDoc = QJsonDocument::fromJson(str.toLocal8Bit().data(), &e);
               QJsonArray object =jsonDoc.array();



               zjm=new Dialog(object);

               qDebug()<<object;
               zjm->show();
               this->close();


           }



    }

