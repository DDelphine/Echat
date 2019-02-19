#include "login.h"
#include <QGridLayout>

login::login(QWidget *parent)
    : QDialog(parent)
{   // interface
    port = 6666;
    ack = false;
    statusLabel = new QLabel(tr("STATUS: "));
    status = new QLabel(" ");
    serverLabel = new QLabel(tr("ServerIP: "));
    serverIP = new QLineEdit(tr("127.0.0.1"));
    logLabel = new QLabel(tr("Usrname: "));
    usrname = new QLineEdit;
    loginBtn = new QPushButton;
    loginBtn->setText(tr("LOGIN IN"));
    signBtn = new QPushButton;
    signBtn->setText(tr("SIGN IN"));
    QGridLayout *lgn = new QGridLayout(this);
    lgn->addWidget(statusLabel,0,0);
    lgn->addWidget(status,0,1);
    lgn->addWidget(serverLabel,1,0);
    lgn->addWidget(serverIP,1,1);
    lgn->addWidget(logLabel,2,0);
    lgn->addWidget(usrname,2,1);

    //lgn->addWidget();
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(loginBtn, QDialogButtonBox::ActionRole);
    buttonBox->addButton(signBtn, QDialogButtonBox::ActionRole);
    lgn->addWidget(buttonBox,3,1);
    setLayout(lgn);

    // signals
    connect(signBtn, SIGNAL(clicked()), this, SLOT(connect2Server()));
    connect(loginBtn, SIGNAL(clicked()), this, SLOT(connect2Server()));

    connect(signBtn, SIGNAL(clicked()), this, SLOT(signIN()));
    connect(loginBtn, SIGNAL(clicked()), this, SLOT(logIN()));

}
void login::connect2Server()
{
    sIP = serverIP->text();
    uname = usrname->text();
    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    tcpClient->connectToHost(QHostAddress(sIP), port, QTcpSocket::ReadWrite);
    connect(tcpClient, SIGNAL(connected()), this, SLOT(sndMsg()));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readMsg()));
}

void login::signIN()
{
    wait(10);
    qDebug()<<"SIGIN "<<endl;
    signBtn->setEnabled(false);
    status->setText(tr("SIGNED IN"));
    if(ack == true)    checkin();
    else
    {
        QMessageBox::information(this,tr("Warning"),tr("lost connection"));
        qDebug()<<"lost connections!"<<endl;
    }
}
void login::logIN()
{
    wait(10);
    qDebug()<<"LOGIN "<<endl;
    loginBtn->setEnabled(false);
    status->setText(tr("LOGIN"));
    if(ack == true)    checkin();
    else
    {
        QMessageBox::information(this,tr("Warning"),tr("lost connection"));
        qDebug()<<"lost connections!"<<endl;
    }
}
void login::sndMsg()
{
    qDebug()<<uname<<sIP<<endl;
    qDebug()<<"send Message!"<<endl;
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8);
    QString IP = getIPaddress();

    out<<tr("login")<<uname<<IP;
    tcpClient->write(message);
    tcpClient->flush();
}
void login::checkin()
{
    qDebug()<<"check in!!!"<<endl;
    this->hide();
    chatlist = new sngChat(0,0,friends);
    chatlist->setAttribute(Qt::WA_DeleteOnClose);
    // when chatlist close, close the login.
    connect(chatlist, SIGNAL(destroyed()), this, SLOT(close()));
    connect(this, SIGNAL(usrInfo(QString,QString)), chatlist, SLOT(usrInfo(QString,QString)));
    // pass tcpclient to chatlist
    chatlist->getTcpClient(tcpClient);
    chatlist->resize(250,650);
    chatlist->show();
    emit usrInfo(uname, sIP);
}
void login::readMsg()
{
    QString aIP;
    // read comming message.
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_5_8);
    QString msgTp;
    in>>msgTp;
    qDebug()<<"Message Type : "<<msgTp<<endl;
    if(msgTp=="friends")
    {
        in>>friends;
       // qDebug()<<friends;
        ack = true;
    }
    if(msgTp=="offline")
    {
       chatlist->getoIP(tr(""));
       QString aUsr;
       in>>aUsr;
       qDebug()<<aUsr<<" is offline."<<endl;
       emit chatlist->setOnline(aUsr,false);
    }
   if(msgTp=="online")
    {
        in>>aIP;
        QStringList tmp = aIP.split("|");
        chatlist->getoIP(tmp[1]);
        emit chatlist->setOnline(tmp[0],true);
        //qDebug()<<aIP<<endl;
    }
   if(msgTp=="message")
    {
        QString message;
        in>>message;
        qDebug()<<tr("message is: ")<<message<<endl;
        chatlist->getMsgList(message);
    }
}
void login::wait(int s)
{
    QTime t;
    t.start();
    while(t.elapsed()<10*s)
    QCoreApplication::processEvents();
}
QString login::getIPaddress() {
    QString IPv4addr;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for(int i = 0; i< ipAddressList.size(); ++i)
    {
        if(ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address())
        {
            IPv4addr = ipAddressList.at(i).toString();
            break;
        }
    }
    if (IPv4addr.isEmpty())
        IPv4addr = QHostAddress(QHostAddress::LocalHost).toString();

    return IPv4addr;
}
login::~login()
{

}
