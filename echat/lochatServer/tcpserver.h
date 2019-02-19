#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>

class TcpServer: public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent,QMap<QString, QString> *uIP,
                       QMap<QString, QList<QString>> *uList,
                       QMap<QString, QString> *mList);
public:
    QMap<QString, QString> *usr2IP;
    QMap<QString, QList<QString>> *usrList;
    QMap<QString, QString> *msgList;
signals:
    void updateUI(QString msg);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
};
#endif // TCPSERVER_H
