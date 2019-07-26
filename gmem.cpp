#include "gmem.h"
#include "ui_gmem.h"

Gmem::Gmem(QMap<QString,QString> mem,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gmem)
{this->setWindowTitle("成员列表");

    ui->setupUi(this);
    QMapIterator<QString,QString> i(mem);
    while (i.hasNext()) {
        i.next();
        ui->listWidget->setResizeMode(QListView::Adjust);
        it=new item(i.key(),i.value(),this);
        QListWidgetItem* Items=new QListWidgetItem(ui->listWidget);
        Items->setSizeHint(QSize(50,100));
        ui->listWidget->setItemWidget(Items,it);



    }
}

Gmem::~Gmem()
{
    delete ui;
}
