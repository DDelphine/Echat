#ifndef SIGLECHAT_H
#define SIGLECHAT_H
#include <QDialog>
#include <QHostAddress>
#include <QList>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QToolBox>
#include <QTcpSocket>
#include <QCloseEvent>
#include <QFrame>
#include "widget.h"
#include "lochat.h"

class Widget;
class lochat;

class sngChat : public QToolBox
{
    Q_OBJECT
public:
    sngChat(QWidget *parent=0,Qt::WindowFlags f=0, QString friends="");
    ~sngChat();
public:
    QString getUsr();
    void getTcpClient(QTcpSocket *socket);
    void getoIP(QString IP);
    void getMsgList(QString);
    void closeEvent( QCloseEvent * event );
private slots:
    void showChat();
    void usrInfo(QString usr, QString IP);
    void groupChat();
    void destroy(QString usr);
signals:
    void setOnline(QString uname, bool status);
    void closeAll();
private:
    QMap<QString, QString> msgList;
    QMap<QString, Widget *> chatWindowList;
    QTcpSocket *tcpClient;
    QStringList fdList;
    QString uname;
    QString uIP;
    QMap<QString,QToolButton *> button;
    QLabel *usrLabel;
    QLabel *line;
    QString toIP;
    lochat *groupchat;
};
#endif // SIGLECHAT_H
