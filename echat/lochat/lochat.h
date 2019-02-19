#ifndef LOCHAT_H
#define LOCHAT_H

#include <QDialog>
#include <QtNetwork/QHostAddress>
#include <QTextEdit>
#include <QTextBrowser>
#include <QtNetwork/QUdpSocket>
#include <QDialogButtonBox>
#include <QtWidgets>

class lochat : public QDialog
{
    Q_OBJECT
public:
    lochat(QWidget *parent = 0);
    ~lochat();
    void getUname(QString usr);

private slots:
    void multSender();
    void multReciever();
    void clear();
private:
    QHostAddress groupAddress;
    QTextBrowser *msgBrowser;
    QTextEdit *msgEdit;
    QDialogButtonBox *buttonBox;
    QPushButton *send;
    QPushButton *quit;
    QUdpSocket *udpSocketR;
    QUdpSocket *udpSocketS;
    qint16 port;
    QString uname;

};

#endif // LOCHAT_H
