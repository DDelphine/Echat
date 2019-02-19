#include "lochat.h"
#include <QGridLayout>
#include <QtWidgets>
#include <QtNetwork/QHostAddress>

lochat::lochat(QWidget *parent)
    : QDialog(parent)
{
    // layout
    msgBrowser = new QTextBrowser;
    msgEdit = new QTextEdit;

    send = new QPushButton(tr("&send"));
    quit = new QPushButton(tr("&quit"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(send, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quit, QDialogButtonBox::RejectRole);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(msgBrowser,0,0);
    mainLayout->addWidget(msgEdit,1,0);
    mainLayout->addWidget(buttonBox,2,0);
    mainLayout->setRowStretch(0,5);
    mainLayout->setRowStretch(1,1);

    setLayout(mainLayout);
    setWindowTitle(tr("lochat"));
    // events
    groupAddress = QHostAddress("239.255.43.21");
    port = 45454;
    // recieve message
    udpSocketR = new QUdpSocket(this);
    udpSocketR->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    udpSocketR->joinMulticastGroup(groupAddress);
    connect(udpSocketR, SIGNAL(readyRead()),this, SLOT(multReciever()));
    // send message
    udpSocketS = new QUdpSocket(this);
    connect(send, SIGNAL(clicked()), this, SLOT(multSender()));
    connect(send, SIGNAL(clicked()), this, SLOT(clear()));
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));

}

void lochat::multSender()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    QString msg = msgEdit->toPlainText();
    out<<uname<<msg;

    udpSocketS->writeDatagram(data, data.length(), groupAddress, port);
}
void lochat::multReciever()
{
    while(udpSocketR->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocketR->pendingDatagramSize());
        udpSocketR->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString uname;
        QString msg;
        in>>uname>>msg;
        msgBrowser->setTextColor(Qt::blue);
        msgBrowser->append("[ " +uname+" ] "+ time);
        msgBrowser->setTextColor(Qt::black);
        msgBrowser->append("    " +msg);
    }
}
void lochat::getUname(QString usr)
{
    uname = usr;
}
void lochat::clear()
{
    msgEdit->clear();
}
lochat::~lochat()
{

}
