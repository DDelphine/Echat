#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDebug>
#include <QNetworkInterface>
#include "singlechat.h"

class sngChat;
class login : public QDialog
{
    Q_OBJECT
public:
    login(QWidget *parent=0);
    ~login();
    void checkin();
    QTcpSocket *tcpClient;
    void wait(int);
    QString getIPaddress();
private:
    sngChat *chatlist;
    QLabel *logLabel;
    QLineEdit *usrname;
    QPushButton *loginBtn;
    QPushButton *signBtn;
    QDialogButtonBox *buttonBox;
    QMap<QString, QString> usrList;
    QLabel *statusLabel;
    QLabel *status;
    QLabel *serverLabel;
    QLineEdit *serverIP;
    QString sIP;
    QString uname;
    qint16 port;
    bool ack;
    QString friends;
signals:
    void usrInfo(QString usr, QString IP);
private slots:
    void signIN();
    void logIN();
    void connect2Server();
    void sndMsg();
    void readMsg();
};
#endif // LOGIN_H
