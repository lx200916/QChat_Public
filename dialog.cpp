#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include<QHostAddress>
#include<QString>
#include<QTreeWidgetItem>
#include<QTreeWidget>
#include<QMap>
#include<QJsonDocument>
#include<QJsonObject>
#include<addfriends.h>
#include<QtDebug>
#include<QDebug>
#include<QHostInfo>
#include<transfer.h>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QJsonArray>
#include<QMovie>
Dialog::Dialog(QJsonArray mem,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    QMovie *movie = new QMovie(":/img/dial/animation.gif");
    ui->label_5->setMovie(movie);
    movie->start();
    this->setWindowTitle("好友列表");
    this->server = new QUdpSocket(this);
    this->server->bind(6666);
    connect(this->server,SIGNAL(readyRead()),this,SLOT(readyread()));
    QString baseUrl = "https://api.gushi.ci/all";
    QNetworkRequest request;
    request.setUrl(QUrl(baseUrl));
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    // 发送请求
    QNetworkReply *pReplay = manager->get(request);
    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    // 获取响应信息
    QByteArray bytes = pReplay->readAll();
    QJsonParseError e;
    QJsonDocument jsonDoc1 = QJsonDocument::fromJson(bytes, &e);
    QJsonObject object1 =jsonDoc1.object();
    ui->label_3->setText(object1.value("content").toString());
    ui->label->setPixmap(QPixmap(":"+mtou));
    ui->label_2->setText(mname);
    ui->treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //读取用户名，若无则要求输入
    //    QString filename="./config.ini";
    //    QFile file(filename);
    //    if(!file.open(QFile::ReadOnly|QFile::QFile::Text)){
    //        Register* reg=new Register(this,this);
    //        reg->show();
    /*while(!over){
    ;
}*/
    //}
    //    else {
    //        mname=file.readAll();
    //        file.close();}
    //      QStringList Friends;
    //      Friends<<"Friends"<<"";
    //      QStringList Blocked;
    //      Friends<<"Blocked"<<"";
    //      QStringList Anonymous;
    //      Friends<<"Anonymous"<<"";
    //      QTreeWidgetItem *friends =new QTreeWidgetItem(ui->treeWidget,Friends);
    //      QTreeWidgetItem *blocked =new QTreeWidgetItem(ui->treeWidget,Blocked);
    //      QTreeWidgetItem *anonymous=new QTreeWidgetItem(ui->treeWidget,Anonymous);

    //读取好友列表
    ui->treeWidget->setIconSize(QSize(40,40));
    //    QString filename="./member.ini";
    //    QFile file1(filename);
    //    file1.open(QFile::ReadOnly|QFile::Text);
    //    while (!file1.atEnd()) {
    int nSize = mem.size();
    qDebug()<<nSize;
    for (int i = 0; i < nSize; ++i) {

        QJsonObject object2 =mem.at(i).toObject();

        if(nSize!=0){

            int i=object2["label"].toString().toInt()-1;


            QTreeWidgetItem *child= new QTreeWidgetItem();
            child->setIcon(0,QIcon(QPixmap(":"+object2.value("pic").toString().replace("\n",""))));
            child->setToolTip(0,object2.value("pic").toString());
            child->setText(1,object2.value("name").toString());
            child->setText(2,"::ffff:"+object2.value("ip").toString());
            child->setText(3,"Offline");
            ui->treeWidget->topLevelItem(i)->addChild(child);
        }
    }


    QJsonObject json;
    //发送在线信息
    json.insert("Up", mname);



    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();

    this->server->writeDatagram(ba,QHostAddress::Broadcast,6666);
    qDebug()<<"SS"<<ba;

    dataRecvWS = Q_NULLPTR;
        connectStatus = false;
        m_timer = new QTimer();
        createDataRecvWS();

}
void Dialog::createDataRecvWS(){
    if(dataRecvWS == Q_NULLPTR){
           dataRecvWS = new QWebSocket();
           qDebug()<<"create websocket!";
           connect(dataRecvWS, &QWebSocket::disconnected, this, &Dialog::onDisConnected, Qt::AutoConnection);
           connect(dataRecvWS, &QWebSocket::textMessageReceived, this, &Dialog::onTextReceived, Qt::AutoConnection);
           connect(dataRecvWS, &QWebSocket::connected, this, &Dialog::onConnected, Qt::AutoConnection);
           connect(m_timer, &QTimer::timeout, this, &Dialog::reconnect, Qt::AutoConnection);
           dataRecvWS->open(QUrl("ws://0.0.0.0:10002/message/"+mname));
       }


}
void Dialog::onConnected(){
    qDebug()<<"DataReveive websocket is already connect!";
    connectStatus = true;
    m_timer->stop();
    qDebug()<<"Address："<<dataRecvWS->peerAddress();
    QJsonObject json;
    //发送在线信息
    json.insert("Online", mname);



    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();
    dataRecvWS->sendTextMessage(ba);



}
void Dialog::onTextReceived(QString msg){
    qDebug()<<"----------------data-----------------";
    qDebug()<<msg<<endl;
    qDebug()<<msg.toLocal8Bit().data()<<endl;
      QJsonDocument jsonDoc = QJsonDocument::fromJson(msg.toUtf8().data());
      QJsonObject object =jsonDoc.object();
      if(object.contains("Up")){if(object.value("Up").toString()==mname) return;
          if(ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1).length()!=0&&ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1)[0]->parent()!=NULL){
              QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive,1);
              qDebug()<<ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1)[0]->parent();
              qDebug()<<"@3"<<endl;
              if(clist[0]->text(3)=="Offline")
              clist[0]->setText(3,"Web Online");
              QJsonObject json;
              //发送在线信息
              json.insert("Up0", mname);
              json.insert("target",object.value("Up").toString());
              QJsonDocument jsonDoc(json);
              QByteArray ba = jsonDoc.toJson();
              dataRecvWS->sendTextMessage(ba);

          }else
          {
              QTreeWidgetItem* child= new QTreeWidgetItem;
              child->setText(1,object.value("Up").toString());
              child->setText(2,"Web chat");
              child->setText(3,"Online");

              ui->treeWidget->topLevelItem(2)->addChild(child);}
          QJsonObject json;
          //发送在线信息
          json.insert("Up0", mname);
          json.insert("target",object.value("Up").toString());
          QJsonDocument jsonDoc(json);
          QByteArray ba = jsonDoc.toJson();
          dataRecvWS->sendTextMessage(ba);
      }
      if(object.contains("Msg")){
qDebug()<<object.value("Msg").toString()<<endl;
          if(object.value("target")!=mname) return;
          else {
              if(ui->treeWidget->findItems(object.value("Name").toString(),Qt::MatchContains | Qt::MatchRecursive,1).length()!=0){
                                  if(ui->treeWidget->findItems(object.value("Name").toString(),Qt::MatchContains | Qt::MatchRecursive,1)[0]->parent()==ui->treeWidget->topLevelItem(0)){
                                      QString tatou=object.value("Pic").toString();
                                      if(chatlist.count(object.value("Name").toString())){
                                          //chatlist.value(ip)
                                          qDebug()<<object.value("Msg").toString();
                                          chatlist.value(object.value("Name").toString())->settext(object.value("Msg").toString(),tatou);

                                      }else {//找不到，新建
                                          qDebug()<<object.value("Msg").toString();
                                          QList<QHostAddress> list1;
                                          list1.append(QHostAddress("192.168.1.1"));
                                          MainWindow* newchat=new MainWindow(list1,nullptr,this,object.value("Name").toString(),object.value("Name").toString(),this);
                                          chatlist.insert(object.value("Name").toString(),newchat);
                                          newchat->show();
                                          newchat->settext(object.value("Msg").toString(),tatou);
                                          //connect(newchat,SIGNAL(quit(MainWindow*)),this,SLOT(quitchat(MainWindow*)));

                                      }
          }
      }


}}if(object.contains("Up0")){if(object.value("Up0").toString()==mname) return;
          if(ui->treeWidget->findItems(object.value("Up0").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1).length()!=0&&ui->treeWidget->findItems(object.value("Up0").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1)[0]->parent()!=NULL){
              QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(object.value("Up0").toString(),Qt::MatchContains | Qt::MatchRecursive,1);
              qDebug()<<ui->treeWidget->findItems(object.value("Up0").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1)[0]->parent();
              qDebug()<<"@3"<<endl;
              if(clist[0]->text(3)=="Offline")
              clist[0]->setText(3,"Web Online");


          }else
          {
              QTreeWidgetItem* child= new QTreeWidgetItem;
              child->setText(1,object.value("Up0").toString());
              child->setText(2,"Web chat");
              child->setText(3,"Online");

              ui->treeWidget->topLevelItem(2)->addChild(child);}}
      if(object.contains("Down")){if(ui->treeWidget->findItems(object.value("Down").toString(),Qt::MatchContains | Qt::MatchRecursive,1).length()!=0){
              QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(object.value("Down").toString(),Qt::MatchContains | Qt::MatchRecursive,1);
              clist[0]->setText(3,"Offline");
          }}


}
void Dialog::onDisConnected(){
    qDebug()<<"DataReceive websocket is disconnected!!!";
    connectStatus = false;
    m_timer->start(3000);/*-<当连接失败时，触发重连计时器，设置计数周期为3秒 */
}
void Dialog::reconnect(){
    qDebug()<<"try to reconnect!";
    dataRecvWS->abort();
    dataRecvWS->open(QUrl("ws://0.0.0.0:10002/message/"+mname));
}
void Dialog::ok(){
    over=true;
}

QHostAddress getHostIPV4Address()
{
    foreach(const QHostAddress& hostAddress,QNetworkInterface::allAddresses())
        if ( hostAddress != QHostAddress::LocalHost && hostAddress.toIPv4Address() )
            return hostAddress;

    return QHostAddress::LocalHost;
}
//接受槽
void Dialog::readyread(){
    while (this->server->hasPendingDatagrams()) {

        QHostAddress ta;
        QByteArray datagram;
        QString dataStr;
        datagram.resize(this->server->pendingDatagramSize());
        this->server->readDatagram(datagram.data(),datagram.size(),&ta);
        qDebug()<<datagram;
        QJsonParseError e;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(datagram, &e);
        QJsonObject object =jsonDoc.object();
        QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());
        QHostAddress address=getHostIPV4Address();
        qDebug()<<ta.toString();
        qDebug()<<address.toString();

        //处理完毕，object为可使用对象
        if(address.isEqual(ta,QHostAddress::TolerantConversion)){qDebug()<<"11";}else {// 拒收自己信息
            if(object.contains("chocho")){
                qDebug()<<"ssr"<<endl;
                QMessageBox::warning(NULL, "warning", "You were poked by "+object.value("chocho").toString()+"!", QMessageBox::Yes, NULL);
               // QMessageBox message(QMessageBox::Warning,"Information","You were poked by "+object.value("chocho").toString()+"!",QMessageBox::Yes|QMessageBox::No,NULL);
            }
            if(object.contains("File")){QString file=object.value("Filename").toString();
                QMessageBox message(QMessageBox::Warning,"Information",object.value("File").toString()+"Wants To Send You "+file+".Recieve?",QMessageBox::Yes|QMessageBox::No,NULL);
                if (message.exec()==QMessageBox::Yes)//确定
                {
                    transfer* rec;
                    rec=new transfer(ta,file,this);
                    rec->show();
                    //rec
                }
            }
            if(object.contains("Groupadd")){

                QStringList addr=object.value("Groupadd").toString().split(",");
                QList<QHostAddress> temp;
                QMap<QString ,QString> meml;
                QString addfull;

                foreach(QString add,addr){
                    if(add.length()==0)
                        continue;
                    addfull=add;
                    QStringList strl=add.split(":");
                    qDebug()<<"ok1"<<endl;
                    if(!address.isEqual(QHostAddress(strl[0]),QHostAddress::TolerantConversion)){
                        qDebug()<<"add:"<<add<<endl;
                        //                        if(ui->treeWidget->findItems(add,Qt::MatchContains | Qt::MatchRecursive,2).length()==0||ui->treeWidget->findItems(add,Qt::MatchContains | Qt::MatchRecursive,2)[0]->parent()!=ui->treeWidget->topLevelItem(0))
                        //                        {
                        //                            qDebug()<<"ok2"<<endl;
                        //                            QJsonObject json;
                        //                            json.insert("Get","00");
                        //                            QJsonDocument jsonDoc(json);
                        //                            QByteArray ba = jsonDoc.toJson();
                        //                            this->server->writeDatagram(ba,QHostAddress(add.replace("::ffff:","")),6666);
                        ////                            QEventLoop eventLoop;
                        ////                            connect(this->server, SIGNAL(readyRead()), &eventLoop, SLOT(quit()));
                        ////                            eventLoop.exec();
                        ////                            while (this->server->hasPendingDatagrams()) {

                        ////                                QHostAddress ta;
                        ////                                QByteArray datagram;
                        ////                                QString dataStr;
                        ////                                datagram.resize(this->server->pendingDatagramSize());
                        ////                                this->server->readDatagram(datagram.data(),datagram.size(),&ta);
                        ////                                qDebug()<<datagram;
                        ////                                QJsonParseError e;
                        ////                                QJsonDocument jsonDoc = QJsonDocument::fromJson(datagram, &e);
                        ////                                QJsonObject object =jsonDoc.object();
                        //                                meml.insert(object.value("Pic").toString(),object.value("Name").toString());
                        //                                qDebug()<<object.value("Name").toString();
                        //                            qDebug()<<"ok3"<<endl;

                        //                            }
                        //                        else
                        //                       {qDebug()<<"add:"<<add<<endl;
                        //                            qDebug()<<"ok22"<<endl;
                        meml.insert(strl[2],strl[1]);



                        temp.append(QHostAddress(strl[0]));
                        qDebug()<<"ok4"<<endl;


                    }}
                meml.insert(ui->treeWidget->findItems(ta.toString(),Qt::MatchContains | Qt::MatchRecursive,2)[0]->toolTip(0),ui->treeWidget->findItems(ta.toString(),Qt::MatchContains | Qt::MatchRecursive,2)[0]->text(1));
                qDebug()<<"qDshow"<<endl;
                temp.append(ta);
                MainWindow* chat1;
                chat1=new MainWindow(temp,&meml,this,"Group Chat","",this);
                foreach(QHostAddress a,temp){
                    gchatlist.insert(a.toString(),chat1);
                }
                chat1->show();
            }
            if(object.contains("fdaddr")){//好友申请消息
                QString temp=object.value("fdaddr").toString();
                qDebug()<<ui->treeWidget->findItems(temp,Qt::MatchContains | Qt::MatchRecursive,1);
                if(ui->treeWidget->findItems(temp,Qt::MatchContains | Qt::MatchRecursive,1).length()!=0&&ui->treeWidget->findItems(temp,Qt::MatchContains | Qt::MatchRecursive,1)[0]->parent()==ui->treeWidget->topLevelItem(0))
                {
                    QJsonObject json;
                    json.insert("Allow", mname);
                    json.insert("Pic",mtou);
                    QJsonDocument jsonDoc(json);
                    QByteArray ba = jsonDoc.toJson();
                    qDebug()<<"11109";
                    this->server->writeDatagram(ba,ta,6666);
                }else if(ui->treeWidget->findItems(temp,Qt::MatchContains | Qt::MatchRecursive,1)[0]->parent()!=ui->treeWidget->topLevelItem(1)) {


                    QMessageBox message(QMessageBox::Warning,"Information","Really to add "+object.value("fdaddr").toString()+" in your friend's list?",QMessageBox::Yes|QMessageBox::No,NULL);
                    if (message.exec()==QMessageBox::Yes)//确定，加入列表
                    {
                        QTreeWidgetItem *node=new QTreeWidgetItem;
                        QString s=object.value("Pic").toString();
                        node->setIcon(0,QIcon(QPixmap(":"+s)));
                        node->setToolTip(0,s);
                        node->setText(1,temp);
                        node->setText(2,ta.toString());
                        node->setText(3,"Online");
                        ui->treeWidget->topLevelItem(0)->addChild(node);
                        QJsonObject json;
                        json.insert("Allow", mname);
                        json.insert("Pic",mtou);
                        QJsonDocument jsonDoc(json);
                        QByteArray ba = jsonDoc.toJson();
                        this->server->writeDatagram(ba,ta,6666);
                        //    QFile fri("./member.ini");
                        //    if (!fri.open(QIODevice::WriteOnly | QIODevice::Text|))
                        //               return;
                        //   QTextStream out(&fri);
                        //                            out <<  object.value("fdaddr").toString() << ta.toString()<<endl;
                    }
                    else
                    {
                        qDebug()<<"clicked no\n";
                    }
                }}
            //收到表对方允许，添加对方进好友列表
            if(object.contains("Allow")){QString temp=object.take("Allow").toString();
                qDebug()<<ui->treeWidget->findItems(temp,Qt::MatchContains | Qt::MatchRecursive,1);
                if(ui->treeWidget->findItems(temp,Qt::MatchContains | Qt::MatchRecursive,1).length()==0||ui->treeWidget->findItems(temp,Qt::MatchContains | Qt::MatchRecursive,1)[0]->parent()!=ui->treeWidget->topLevelItem(0)){
                    QTreeWidgetItem *node=new QTreeWidgetItem;
                    QString s=object.value("Pic").toString();
                    node->setIcon(0,QIcon(QPixmap(":"+s)));
                    node->setToolTip(0,s);
                    node->setText(1,temp);
                    node->setText(2,ta.toString());
                    node->setText(3,"Online");
                    ui->treeWidget->topLevelItem(0)->addChild(node);
                    //                    QFile file("./member.ini");
                    //                    file.open(QFile::WriteOnly|QFile::Append|QFile::Text);
                    //                    QTextStream in(&file);
                    //                    in<<node->text(0)+","+ta.toString()+",1"<<endl;

                }}

            //找人，出窗口
            if(object.contains("Msg")){if(ui->treeWidget->findItems(object.value("Name").toString(),Qt::MatchContains | Qt::MatchRecursive,1).length()!=0){
                    if(ui->treeWidget->findItems(object.value("Name").toString(),Qt::MatchContains | Qt::MatchRecursive,1)[0]->parent()==ui->treeWidget->topLevelItem(0)){
                        QString tatou=object.value("Pic").toString();
                        if(chatlist.count(ta.toString())){
                            //chatlist.value(ip)
                            chatlist.value(ta.toString())->settext(object.value("Msg").toString(),tatou);

                        }else {//找不到，新建
                            QList<QHostAddress> list1;
                            list1.append(ta);
                            MainWindow* newchat=new MainWindow(list1,nullptr,this,object.value("Name").toString(),"",this);
                            chatlist.insert(ta.toString(),newchat);
                            newchat->show();
                            newchat->settext(object.value("Msg").toString(),tatou);
                            //connect(newchat,SIGNAL(quit(MainWindow*)),this,SLOT(quitchat(MainWindow*)));

                        }

                        //QMessageBox::information(this,"New Msg",dataStr);

                    }}}
            //收到返回信息（头像，名称）
            if(object.contains("Get")){
                QJsonObject json;

                json.insert("Pic", mtou);
                json.insert("Name",mname);
                QJsonDocument jsonDoc(json);
                QByteArray ba = jsonDoc.toJson();

                this->server->writeDatagram(ba,ta,6666);
            }
            //判断对方是否在线
            if(object.contains("Up")){if(ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1).length()!=0&&ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1)[0]->parent()!=NULL){
                    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive,1);
                    qDebug()<<ui->treeWidget->findItems(object.value("Up").toString(),Qt::MatchContains | Qt::MatchRecursive|Qt::MatchCaseSensitive,1)[0]->parent();
                    qDebug()<<"@3"<<endl;
                    clist[0]->setText(3,"Online");
                }else
                {
                    QTreeWidgetItem* child= new QTreeWidgetItem;
                    child->setText(1,object.value("Up").toString());
                    child->setText(2,ta.toString());
                    child->setText(3,"Online");

                    ui->treeWidget->topLevelItem(2)->addChild(child);}
                QJsonObject json;
                json.insert("Up0", mname);
                QJsonDocument jsonDoc(json);
                QByteArray ba = jsonDoc.toJson();
                this->server->writeDatagram(ba,ta,6666);}
            if(object.contains("Up0")){if(ui->treeWidget->findItems(object.value("Up0").toString(),Qt::MatchContains | Qt::MatchRecursive,1).length()!=0){
                    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(object.value("Up0").toString(),Qt::MatchContains | Qt::MatchRecursive,1);
                    clist[0]->setText(3,"Online");}else {QTreeWidgetItem* child= new QTreeWidgetItem;
                    child->setText(1,object.value("Up0").toString());
                    child->setText(2,ta.toString());
                    child->setText(3,"Online");
                    ui->treeWidget->topLevelItem(2)->addChild(child);
                }}
            if(object.contains("GMsg")){
                QString tatou=object.value("Pic").toString();
                if(gchatlist.count(ta.toString())){
                    //chatlist.value(ip)
                    gchatlist.value(ta.toString())->settext(object.value("GMsg").toString(),tatou);

                }
            }
            if(object.contains("Down")){if(ui->treeWidget->findItems(object.value("Up0").toString(),Qt::MatchContains | Qt::MatchRecursive,1).length()!=0){
                    QList<QTreeWidgetItem*> clist = ui->treeWidget->findItems(object.value("Down").toString(),Qt::MatchContains | Qt::MatchRecursive,1);
                    clist[0]->setText(3,"Offline");
                }}
        }}
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::closeEvent(QCloseEvent* event){
    //发送离线信息
    QString in;
    QJsonArray json1;
    QJsonObject json22;
    //发送在线信息
    json22.insert("Offline", mname);



    QJsonDocument jsonDoc22(json22);
    QByteArray ba22 = jsonDoc22.toJson();
    dataRecvWS->sendTextMessage(ba22);

    QTreeWidgetItem *root = ui->treeWidget->topLevelItem(0);
    QTreeWidgetItemIterator it(root);
    qDebug()<<"efssefesf"<<endl;
    while (*it)
    {if((*it)->parent()==ui->treeWidget->topLevelItem(0)){
            QJsonObject json2;
            json2.insert("name", (*it)->text(1));
            json2.insert("ip",(*it)->text(2).replace("::ffff:",""));
            json2.insert("label","1");
            json2.insert("pic",(*it)->toolTip(0));
            json1.append(json2);
            //in+= (*it)->text(1)+","+(*it)->text(2)+",1,"++":";}
            if((*it)->parent()==ui->treeWidget->topLevelItem(1))
            {  QJsonObject json2;
                json2.insert("name", (*it)->text(1));
                json2.insert("ip",(*it)->text(2).replace("::ffff:",""));
                json2.insert("label","2");
                json2.insert("pic",(*it)->toolTip(0));
                json1.append(json2);

                in+= (*it)->text(1)+","+(*it)->text(2)+",2,"+(*it)->toolTip(0)+":";}
            qDebug()<<(*it)->text(1);

        }++it;}QString baseUrl = "http://0.0.0.0:5000/mem/";
    QUrl url(baseUrl);
    QJsonDocument jsonDoc2(json1);
    QByteArray ba2 = jsonDoc2.toJson();
    // 表单数据
    QByteArray dataArray;
    dataArray.append("Name="+mname+"&Mem="+ba2);

    QJsonObject json2;
    //发送在线信息
    json2.insert("Disconnect", mname);



    QJsonDocument json2Doc(json2);
    QByteArray ba12 = jsonDoc2.toJson();
    dataRecvWS->sendTextMessage(ba12);
    // 构造请求
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(url);
    QNetworkAccessManager* m_accessManager;
    m_accessManager = new QNetworkAccessManager(this);
    qDebug()<<dataArray;
    QNetworkReply *pReplay = m_accessManager->post(request, dataArray);

    // 开启一个局部的事件循环，等待响应结束，退出
    QEventLoop eventLoop;
    QObject::connect(m_accessManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    qDebug()<<dataArray;
    QJsonObject json;
    json.insert("Down", mname);
    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();
    this->server->writeDatagram(ba,QHostAddress::Broadcast,6666);
    //    QFile file("./member.ini");
    //    file.open(QFile::WriteOnly|QFile::Truncate|QFile::Text);
    //    QTextStream in(&file);


    //    file.close();

}
//双击新窗口以发送信息
void Dialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()==ui->treeWidget->topLevelItem(0))
    {
        QString tname=item->text(1);
        QString ip=item->text(2);
        if(item->text(3)=="Web Online") {
            ip=tname;
            if(chatlist.count(ip))
            {
    //chatlist.value(ip)->show();
            }
            else
            {QString temp=ip;

                QHostAddress address("192.168.0.1");
                qDebug()<<"WebChat...."<<ip<<endl;
                qDebug()<<address;
                QList<QHostAddress> list1;
                list1.append(address);
                MainWindow *newchat=new MainWindow(list1,nullptr,this,tname,tname,this);
                chatlist.insert(ip,newchat);
                newchat->show();
                //connect(newchat,SIGNAL(quit(MainWindow*)),this,SLOT(quitchat(MainWindow*)));
            }
        }
        qDebug()<<"adawd"<<ip<<endl;
        if(chatlist.count(ip))
        {
//chatlist.value(ip)->show();
        }
        else
        {QString temp=ip;

            QHostAddress address(temp.replace("::ffff:",""));
            qDebug()<<"仙人"<<ip<<endl;
            qDebug()<<address;
            QList<QHostAddress> list1;
            list1.append(address);
            MainWindow *newchat=new MainWindow(list1,nullptr,this,tname,"",this);
            chatlist.insert(ip,newchat);
            newchat->show();
            //connect(newchat,SIGNAL(quit(MainWindow*)),this,SLOT(quitchat(MainWindow*)));
        }
    }
    else
    {
        item->setExpanded(true);
    }
}
//void Dialog::quitchat(MainWindow* m){
//    QString s=chatlist.key(m);
//    chatlist.remove(s);
//}

void Dialog::on_pushButton_clicked()
{
    addfriends *p=new addfriends;
    p->show();

}
void Dialog::on_pushButton_2_clicked()
{
    if(ui->treeWidget->selectedItems().length()==0)return;
    QTreeWidgetItem * currentItem = ui->treeWidget->selectedItems()[0];
    qDebug()<<currentItem->text(1);
    if(currentItem==NULL)
    {
        return;
    }
    //如果没有父节点就直接删除
    if(currentItem->parent()==NULL)
    {
        return;
    }
    else
    {
        //如果有父节点就要用父节点的takeChild删除节点
        delete currentItem->parent()->takeChild(ui->treeWidget->currentIndex().row());
    }

}

void Dialog::on_pushButton_3_clicked()
{
    QList<QTreeWidgetItem*> selectedItemList = ui->treeWidget->selectedItems();
    QString memlist;QList<QHostAddress> group1;
    QMap<QString,QString> memmap;
    if(selectedItemList.length()==0) return;
    foreach(QTreeWidgetItem* it,selectedItemList){
        if(it->parent()!=NULL){
            memlist+=it->text(2).replace("::ffff:","")+":"+it->text(1)+":"+it->toolTip(0)+",";
            memmap.insert(it->toolTip(0),it->text(1));
            group1.append(QHostAddress(it->text(2)));
        }}
    MainWindow* chat;
    chat =new MainWindow(group1,&memmap,this,"Group Chat","",this);

    foreach(QTreeWidgetItem* it,selectedItemList){
        QJsonObject json;
        json.insert("Groupadd",memlist);

        QJsonDocument jsonDoc(json);
        QByteArray ba = jsonDoc.toJson();
        this->server->writeDatagram(ba,QHostAddress(it->text(2).replace("::ffff:","")),6666);
        gchatlist.insert(it->text(2),chat);

    }
    chat->show();
}

void Dialog::on_pushButton_4_clicked()
{
     if(ui->treeWidget->selectedItems().length()==0)return;
     QTreeWidgetItem * currentItem = ui->treeWidget->selectedItems()[0];
     //QTreeWidgetItem ccurrentItem = ui->treeWidget->selectedItems()[0];
     qDebug()<<currentItem->text(1);
     if(currentItem==NULL)
     {
         return;
     }
     //如果没有父节点就直接删除
     if(currentItem->parent()==NULL)
     {
         return;
     }
     else
     {
         //如果有父节点就要用父节点的takeChild删除节点
          currentItem->parent()->removeChild(currentItem);
     }
     qDebug()<<"sb"<<endl;
     //qDebug()<<ui->treeWidget->topLevelItem(1)->text(1)<<endl;
     ui->treeWidget->topLevelItem(1)->addChild(currentItem);
}
