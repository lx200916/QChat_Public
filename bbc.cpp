#include "bbc.h"
#include "ui_bbc.h"
#include<QFile>
#include<QTextStream>
#include<QDebug>
BBC::BBC(QString nnmm,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BBC)
{

    ui->setupUi(this);
    ui->BBC1->setReadOnly(true);
    QString gg="./"+nnmm+".ini";
    ssbb=nnmm;
    QFile ss(gg);
    if(!ss.open(QFile::ReadOnly))qDebug()<<"ssh"<<endl;
    QTextStream in(&ss);
    while(!in.atEnd()){
        QString lin = in.readLine();
        ui->BBC1->append(lin);
    }

}

BBC::~BBC()
{
    delete ui;
}
