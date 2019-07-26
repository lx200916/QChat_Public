#include "hello.h"
#include "ui_hello.h"
#include<QMovie>

HELLO::HELLO(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HELLO)
{
    ui->setupUi(this);
    auto movie = new QMovie(this);
    this->setWindowTitle(" ");
    movie->setFileName(":/19-07-19-15-55-08.gif");
    connect(movie, &QMovie::frameChanged, [=]{
        ui->pushButton->setIcon(movie->currentPixmap());
    });
    movie->start();

//    QMovie *movie = new QMovie(":/19-07-19-15-55-08.gif");
//    ui->label_2->setMovie(movie);
//    movie->start();
   // this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
}

HELLO::~HELLO()
{
    delete ui;
}

void HELLO::on_pushButton_clicked()
{
    ww = new Widget;
    ww->show();
    this->close();
}
//void BlueButton::mousePressEvent(QMouseEvent *event)
//{
//    //按下切换背景图片
//    //setStyleSheet("BlueButton{border-image:url(:/icon/G:/防偷懒小工具/pic/press_btn.png); color: #229aff}");
//}

//void BlueButton::mouseReleaseEvent(QMouseEvent *event)
//{
//    //释放切换背景图片
//    setStyleSheet("BlueButton{border-image:url(:/icon/G:/防偷懒小工具/pic/un_press_btn.png); color: #959595}");

//    //如果this的矩形包含鼠标当前的位置
//    //也就是鼠标在当前控件上
//    //发出click信号
//    if(frameRect().contains(event->pos())){
//        emit on_click();
//    }
//}
