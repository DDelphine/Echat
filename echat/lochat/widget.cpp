#include "widget.h"
#include <QGridLayout>
#include <QDateTime>

Widget::Widget(QWidget *parent, QString usr, QString IP) :
    QWidget(parent)
{
   // setWindowTitle(usr);
    udpSocketR = new QUdpSocket;
    udpSocketS = new QUdpSocket;
    // get IP
    toIP = IP;
    Usr = usr;
    isOnline = false;
    portList.insert("jcyan", 4541);
    portList.insert("mzhdu", 4542);
    portList.insert("qxxie", 4543);
    portList.insert("Bless", 4547);
    portList.insert("GoGo", 4545);
    portList.insert("Duke", 4546);

    port = portList[Usr];
    qDebug()<<port<<endl;
    //port=4544;
    send = new QPushButton;
    send->setText(tr("send"));
    exit = new QPushButton;
    exit->setText(tr("exit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(send, QDialogButtonBox::ActionRole);
    buttonBox->addButton(exit, QDialogButtonBox::RejectRole);


    connect(send, SIGNAL(clicked()), this, SLOT(sendMsg()));
    connect(exit, SIGNAL(clicked()), this, SLOT(close()));
    connect(exit, SIGNAL(clicked()), this, SLOT(closeInfo()));
    if(udpSocketR->bind(port, QAbstractSocket::DontShareAddress))
    {
        qDebug()<<Usr<<" bind to port "<< port<<" success"<<endl;
    }

    connect(udpSocketR, SIGNAL(readyRead()), this, SLOT(recieveMsg()));

    QGridLayout *chatlayout = new QGridLayout(this);
    msgBrowser = new QTextBrowser;
    msgEdit = new QTextEdit;
    chatlayout->addWidget(msgBrowser,0,0);
    chatlayout->addWidget(msgEdit,1,0);
    chatlayout->addWidget(buttonBox,2,0);
    chatlayout->setRowStretch(0,5);
    chatlayout->setRowStretch(1,1);
    setLayout(chatlayout);

}

void Widget::sendMsg()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    QHostAddress addr = QHostAddress(toIP);
    QString msg = msgEdit->toPlainText();
    msgEdit->clear();
    out<<Usr<<msg;

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    msgBrowser->setTextColor(Qt::blue);
    msgBrowser->append("[ " + Usr + " ]" + time);
    msgBrowser->setTextColor(Qt::black);
    msgBrowser->append("    " +msg);

    QByteArray message;
    QDataStream tcpOut(&message, QIODevice::WriteOnly);
    tcpOut.setVersion(QDataStream::Qt_5_8);
    tcpOut<<tr("alive")<<toUsr;
    tcpclient->write(message);
    wait(100);// wait answer from server
    qDebug()<<isOnline<<endl;
    if(isOnline)
    {
        // send data
        qDebug()<<"Send data to "<<toUsr<<endl;
        int toPort = portList[toUsr];
        //toPort = port;
        udpSocketS->writeDatagram(data, data.length(), addr, toPort);
    }
    else // send to server
    {
        qDebug()<<"Send data to server"<<endl;
        QByteArray aMessage;
        QDataStream out(&aMessage, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);
        QString tmp = Usr + "|" + toUsr + "|" + time + "|" + msg;
        out<<tr("message")<<tmp;
        tcpclient->write(aMessage);


    }
}
void Widget::recieveMsg()
{
    while(udpSocketR->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocketR->pendingDatagramSize());
        udpSocketR->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString msg;
        QString tmp;
        in>>tmp>>msg;
        qDebug()<<tmp<<endl;
        if(tmp != Usr)
        {
            msgBrowser->setTextColor(Qt::blue);
            msgBrowser->append("[ " + toUsr + " ] " + time);
            msgBrowser->setTextColor(Qt::black);
            msgBrowser->append("    " + msg);
        }
    }
}
void Widget::getTcpClient(QTcpSocket *socket)
{
    tcpclient = socket;
}
void Widget::showMsg(QString msg)
{
    QStringList tmp = msg.split('|');
    msgBrowser->setTextColor(Qt::blue);
    msgBrowser->append("[ " + toUsr + " ] " + tmp[0]);
    msgBrowser->setTextColor(Qt::black);
    msgBrowser->append("    " + tmp[1]);
}
void Widget::getInfo(QString from, QString to)
{
    Usr = from;
    toUsr = to;
}
void Widget::setOnline(QString uname, bool status)
{
    qDebug()<<toUsr<<":"<<uname<<" status  is "<<status<<endl;
    if(uname==toUsr) isOnline = status;
}
void Widget::wait(int s)
{
    QTime t;
    t.start();
    while(t.elapsed()<10*s)
    QCoreApplication::processEvents();
}
void Widget::closeInfo()
{
    emit destroy(toUsr);
}
QTextBrowser* Widget::getmsgBrowser()
{
    return msgBrowser;
}
Widget::~Widget()
{

}
