#ifndef ITEM_H
#define ITEM_H

#include <QWidget>

namespace Ui {
class item;
}

class item : public QWidget
{
    Q_OBJECT

public:
    explicit item(QString head,QString name,QWidget *parent = nullptr);
    ~item();

private:
    Ui::item *ui;
};

#endif // ITEM_H
