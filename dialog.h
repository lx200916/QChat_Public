#ifndef DIALOG_H
#define DIALOG_H
#include<QMap>
#include<QFile>
#include <QDialog>
#include<register.h>
#include<QHostAddress>
#include<QHostInfo>
#include<QUdpSocket>
#include<QNetworkInterface>
#include<QMessageBox>
#include<QHostAddress>
#include<QDebug>
#include<QString>
#include<QTextCodec>
#include<QTreeWidgetItem>
#include<QMap>
#include<QtWebSockets>
#include<QCloseEvent>
#include "externa.h"
class MainWindow;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    void createDataRecvWS();    /*-<创建websocket连接 */

    QWebSocket *dataRecvWS;     /*-<websocket类 */
    bool connectStatus;         /*-<websocket连接状态，连接成功：true；断开：false */
    void reconnect();           /*-<周期重连函数 */
    QTimer *m_timer;
    void closeEvent(QCloseEvent*event);
    QUdpSocket *server;
    explicit Dialog(QJsonArray mem, QWidget *parent = nullptr);
QMap<QString,MainWindow*> chatlist;
QMap<QString,MainWindow*> gchatlist;

QMap<QString,QHostAddress> member;
    ~Dialog();
QString name;
QHostAddress ipd;

void startchat(QTreeWidgetItem * item,int column);
protected slots:
private slots:
void onConnected();                 /*-<socket建立成功后，触发该函数 */
void onTextReceived(QString msg);   /*-<收到Sev端的数据时，触发该函数 */
void onDisConnected();              /*-<socket连接断开后，触发该函数 */

void ok();
void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
void readyread();
void on_pushButton_clicked();
void on_pushButton_2_clicked();
void on_pushButton_3_clicked();

void on_pushButton_4_clicked();

private:
    Ui::Dialog *ui;
    MainWindow *cj;
    MainWindow *newchat;
    QMenu *m_contextMenu;
    QAction *m_addAction;
    QAction *m_delAction;
    bool over=false;
};

#endif  // DIALOG_H
