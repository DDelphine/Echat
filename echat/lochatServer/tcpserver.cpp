#include "tcpserver.h"
#include "socketthread.h"

TcpServer::TcpServer(QObject *parent, QMap<QString, QString> *uIP,
                     QMap<QString, QList<QString>> *uList,
                     QMap<QString, QString> *mList): QTcpServer(parent)
{
    usr2IP = uIP;
    usrList = uList;
    msgList = mList;
}
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"new connections"<<endl;
    socketThread *thread = new socketThread(0, usr2IP, usrList, msgList, socketDescriptor);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(updateUI(QString)), this, SIGNAL(updateUI(QString)));
    thread->start();
   // socket->moveToThread(thread);
}
