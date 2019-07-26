#ifndef ADDFRIENDS_H
#define ADDFRIENDS_H

#include <QDialog>
#include<QHostAddress>
namespace Ui {
class addfriends;
}

class addfriends : public QDialog
{
    Q_OBJECT

public:
    explicit addfriends(QWidget *parent = nullptr);
    ~addfriends();

private slots:
    void on_pn_clicked();

private:
    Ui::addfriends *ui;
};

#endif // ADDFRIENDS_H
