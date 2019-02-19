#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include<QThread>
#include<QTcpSocket>
#include<QTcpServer>
#include <QDateTime>
#include<QWidget>
#include "widget.h"

namespace Ui {
class Widget;
}
class socketThread: public QThread
{
    Q_OBJECT
public:
    socketThread(QWidget *parent=0, QMap<QString, QString> *uIP=NULL,
                               QMap<QString, QList<QString>> *uList=NULL,
                               QMap<QString, QString> *mList=NULL,int socketDescriptor=0);
    ~socketThread();
public:
    //QMap<QString, QTcpSocket> tcpSocketMap;
    QMap<QString, QString> *usr2IP;
    QMap<QString, QList<QString>> *usrList;
    QMap<QString, QString> *msgList;
signals:
    void quit();
    void updateUI(QString);
private slots:
    void readMsg();
private:
    qintptr socketDescriptor;
    QTcpSocket *tcpSocketR;
private:
    void sendMsg(QTcpSocket *tcpSocket, QString msg);
protected:
    virtual void run();
};
#endif // SOCKETTHREAD_H
