#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include<QTcpSocket>
#include "widget.h"
namespace Ui {
class Widget;
}
class mytcpSocket: public QTcpSocket
{
    Q_OBJECT
public:
    mytcpSocket(QWidget *parent, qintptr p);
    ~mytcpSocket();
private:
    void readMsg();
    void sendMsg(QTcpSocket *tcpSocket, QString msg);
private:
    TcpSocket *tcpSocketR;
    QMap<QString, QString> *usr2IP;
    QMap<QString, QList<QString>> *usrList;
    QMap<QString, QString> *msgList;
    Ui::Widget *ui;

}
#endif // MYTCPSOCKET_H
