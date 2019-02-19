#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include "tools.h"
#include "socketthread.h"
#include "tcpserver.h"

class TcpServer;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private:
    Ui::Widget *ui;
    TcpServer *tcpServer;
    QMap<QString, QString> usr2IP;
    QMap<QString, QList<QString>> usrList;
    QMap<QString, QString> msgList;
    int port;
private slots:
    void updateUI(QString msg);
};

#endif // WIDGET_H
