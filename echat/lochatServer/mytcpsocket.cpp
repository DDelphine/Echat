#include"mytcpsocket.h"
mytcpsocket::mytcpsocket(QWidget *parent,QMap<QString, QString> *uIP,
                         QMap<QString, QList<QString>> *uList,
                         QMap<QString, QString> *mList):QTcpSocket(0)
{
    ui = parent;
    usr2IP = uIP;
    usrList = uList;
    msgList = mList;
    tcpSocketR =new QTcpSocket;

}
