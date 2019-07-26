#ifndef HELLO_H
#define HELLO_H
#include<widget.h>
#include <QMainWindow>

namespace Ui {
class HELLO;
}

class HELLO : public QMainWindow
{
    Q_OBJECT

public:
    explicit HELLO(QWidget *parent = nullptr);
    ~HELLO();
private slots:
    void on_pushButton_clicked();

private:
    Ui::HELLO *ui;
    Widget *ww;
};

#endif // HELLO_H
