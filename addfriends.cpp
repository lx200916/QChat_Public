#include "addfriends.h"
#include "ui_addfriends.h"
#include<QJsonObject>
#include<externa.h>
#include<QUdpSocket>
#include<QJsonDocument>
#include<QMovie>
#include<QMessageBox>
addfriends::addfriends(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addfriends)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("IP Address....");
   // connect(ui->pn,SIGNAL(),this,SLOT());
    ui->pn->setShortcut(Qt::Key_Return);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
//    QMovie *movie = new QMovie(":/img/b0.gif");
//        ui->label_2->setMovie(movie);
//        movie->start();
}

addfriends::~addfriends()
{
    delete ui;
}

void addfriends::on_pn_clicked()
{
    QMessageBox::information(NULL, "", "发送成功！", QMessageBox::Yes);
    QUdpSocket *fri=new QUdpSocket;
    QJsonObject json;
    json.insert("fdaddr", mname);
    json.insert("Pic",mtou);
  //  QString stauts="mf";
    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();
    fri->writeDatagram(ba,QHostAddress(ui->lineEdit->text()),6666);
    //qDebug() << re<<endl;
}
