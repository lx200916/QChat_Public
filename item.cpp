#include "item.h"
#include "ui_item.h"

item::item(QString head,QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::item)
{
    ui->setupUi(this);
ui->head->setPixmap(QPixmap(":"+head));
ui->name->setText(name);}

item::~item()
{
    delete ui;
}
