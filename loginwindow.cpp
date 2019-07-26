#include "loginwindow.h"
#include "ui_loginwindow.h"
#include<choose.h>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QJsonDocument>
#include<QJsonObject>
#include<QMovie>
loginwindow::loginwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);
    ui->ID->setPlaceholderText("Username");
    ui->pw1->setPlaceholderText("Password");
    ui->pw2->setPlaceholderText("Retype Password");
    ui->username->setPlaceholderText("Name");
    ui->pw1->setEchoMode(QLineEdit::Password);
    ui->pw2->setEchoMode(QLineEdit::Password);
    ui->pushButton->setShortcut(Qt::Key_Return);
}


loginwindow::loginwindow(QString fn, QWidget *parent):
    QWidget(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);
    ui->ID->setPlaceholderText("Username");
    ui->pw1->setPlaceholderText("Password");
    ui->pw2->setPlaceholderText("Retype Password");
    ui->username->setPlaceholderText("Name");
    QMovie *movie = new QMovie(":/img/be.gif");
    ui->beiji->setMovie(movie);
    movie->start();
    this->setWindowTitle("注册界面");
    filename=fn;
    ui->pw1->setEchoMode(QLineEdit::Password);
    ui->pw2->setEchoMode(QLineEdit::Password);
}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::on_pushButton_clicked()
{
    QString pw1;
    QString pw2;
    QString name=ui->username->text();
    pw1=ui->pw1->text();
    pw2=ui->pw2->text();
    if(ui->ID->text().length()==0)
    {
        QMessageBox::warning(NULL, "warning", "帐号不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        //帐号为空提示
    }
    if(ui->username->text()==NULL)
    {
        QMessageBox::warning(NULL, "warning", "用户名不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        //用户名为空提示
    }
    if(pw1==pw2&&pw1.length()!=0)
    {
//        QFile f(filename);
//        f.open(QFile::Append);
//        QTextStream ts(&f);
//        ts<<ui->ID->text()<<":"<<pw1<<":"<<name;
        QString baseUrl = "http://0.0.0.0:5000/user/";
        QUrl url(baseUrl);

        // 表单数据
        QByteArray dataArray;
        dataArray.append("Username="+ui->ID->text()+"&Password="+pw2+"&Name="+name);


        // 构造请求
        QNetworkRequest request;
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setUrl(url);
        QNetworkAccessManager* m_accessManager;
        m_accessManager = new QNetworkAccessManager(this);
        //m_accessManager->post(request, dataArray);
        QNetworkReply *pReplay = m_accessManager->post(request,dataArray);

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

         QMessageBox::warning(NULL, "success", "注册成功", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
         ch=new choose(ui->ID->text(),pw2,name);
         ch->show();
         this->close();

        }if(object1.value("Msg").toString()=="Name Exist"){
            QMessageBox::warning(NULL, "warning", "用户名被占用", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        }
        if(object1.value("Msg").toString()=="Already"){
            QMessageBox::warning(NULL, "warning", "你已经注册，请直接登录", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);


        }if(object1.value("Msg").toString()=="Exist"){
            QMessageBox::warning(NULL, "warning", "账号已存在", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        }

    }
    else if(pw1!=pw2)
    {
        QMessageBox::warning(NULL, "warning", "密码不一致", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        //俩次密码不一致提示
    }
    else
    {
        QMessageBox::warning(NULL, "warning", "密码不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}
