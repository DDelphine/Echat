#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QUdpSocket>
#include "singlechat.h"
#include <QDialogButtonBox>

class sngChat;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent, QString usr, QString IP);
    ~Widget();
    void getInfo(QString from, QString to);
    void getTcpClient(QTcpSocket *socket);
    void showMsg(QString msg);
    void wait(int s);
    QTextBrowser *getmsgBrowser();
private:
    QUdpSocket *udpSocketR;
    QUdpSocket *udpSocketS;
    QTcpSocket *tcpclient;
    qint16 port;
    QDialogButtonBox *buttonBox;
    QPushButton *send;
    QPushButton *exit;
    QTextBrowser *msgBrowser;
    QTextEdit *msgEdit;
    QMap<QString,int> portList;
    QString toIP;
    QString Usr;
    QString toUsr;
    bool isOnline;
signals:
    void destroy(QString usr);
private slots:
    void sendMsg();
    void recieveMsg();
    void setOnline(QString uname, bool status);
    void closeInfo();
};

#endif // WIDGET_H
