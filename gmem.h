#ifndef GMEM_H
#define GMEM_H
#include<item.h>
#include <QMainWindow>

namespace Ui {
class Gmem;
}

class Gmem : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gmem(QMap<QString,QString> mem,QWidget *parent = nullptr);
    ~Gmem();
item* it;
private:
    Ui::Gmem *ui;
};

#endif // GMEM_H
