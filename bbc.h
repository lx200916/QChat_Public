#ifndef BBC_H
#define BBC_H

#include <QMainWindow>
#include<QString>
namespace Ui {
class BBC;
}

class BBC : public QMainWindow
{
    Q_OBJECT

public:
    explicit BBC(QString nnmm,QWidget *parent = nullptr);
    ~BBC();

private:
    Ui::BBC *ui;
    QString ssbb;
};

#endif // BBC_H
