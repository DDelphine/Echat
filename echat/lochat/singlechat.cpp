#include "singlechat.h"
#include <QtWidgets>
#include <QGroupBox>
#include <QVBoxLayout>
#include "widget.h"

sngChat::sngChat(QWidget *parent,Qt::WindowFlags f, QString friends)
    : QToolBox(parent, f)
{
    QString fd;
    setWindowTitle(tr("eChat"));
    // friends list
    fdList = friends.split("|");

    QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *layout=new QVBoxLayout(groupBox);
    layout->setMargin(20);
    layout->setAlignment(Qt::AlignLeft);

    QStringList::iterator iter;
    for(iter=fdList.begin(); iter!=fdList.end(); ++iter)
    {
        fd = (*iter);
        //qDebug()<<fd<<endl;

        QToolButton *btn = new QToolButton;
        btn->setObjectName(fd);
        btn->setText(tr("*  ")+fd);
        button.insert(fd, btn);
        connect(button[fd], SIGNAL(clicked()), this, SLOT(showChat()));
        button[fd]->setAutoRaise(true);
        layout->addWidget(button[fd]);
    }

    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addStretch();//插入一个占位符
    this->addItem((QWidget*)groupBox,tr("\n >>>>>friends"));

   // group
    QToolButton *groupBtn = new QToolButton;
    groupBtn->setText(tr("group chat"));
    groupBtn->setAutoRaise(true);
    connect(groupBtn, SIGNAL(clicked()), this, SLOT(groupChat()));

    QGroupBox *groupBox1 = new QGroupBox;
    QVBoxLayout *layout1=new QVBoxLayout(groupBox1);
    layout1->setSpacing(10);
    layout1->setContentsMargins(10, 10, 10, 10);
   // layout1->addWidget(linegroup);
    layout1->addWidget(groupBtn);
    layout1->addStretch();//插入一个占位符
    this->addItem((QWidget*)groupBox1,tr("\n >>>>>group"));
    this->setFrameShape(Box);
}
void sngChat::showChat()
{
    QString usr = sender()->objectName();
    if(!chatWindowList.contains(usr))
    {
        Widget *chatWindow = new Widget(0, getUsr(), toIP);
        chatWindowList.insert(usr, chatWindow);
        connect(chatWindow, SIGNAL(destroy(QString)), this, SLOT(destroy(QString)));

        chatWindow->getTcpClient(tcpClient);
        connect(this, SIGNAL(setOnline(QString, bool)), chatWindow, SLOT(setOnline(QString, bool)));
        connect(this, SIGNAL(closeAll()), chatWindow, SLOT(close()));
        chatWindow->getInfo(getUsr(), usr);
        chatWindow->setWindowTitle("chat with " + usr);

        chatWindow->show();
    }
    if(msgList.contains(usr))// show message
    {
        qDebug()<<"show message : "<<msgList[usr]<<endl;
        QStringList tmp = msgList[usr].split("|");
        chatWindowList[usr]->getmsgBrowser()->setTextColor(Qt::blue);
        chatWindowList[usr]->getmsgBrowser()->append("[ " + usr + " ]" + tmp[0]);
        chatWindowList[usr]->getmsgBrowser()->setTextColor(Qt::black);
        chatWindowList[usr]->getmsgBrowser()->append("    " + tmp[1]);
        button[usr]->setText(tr("*  ")+ usr);
        msgList.remove(usr);// clear message
    }
}
void sngChat::closeEvent(QCloseEvent * event)
{
    if (!(QMessageBox::information(this,tr("Quit Window"),
                                   tr("Do you really want to log out?"),tr("Yes"),tr("No"))))
    {
        QByteArray message;
        QDataStream out(&message, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_8);
        out<<tr("quit")<<uname;
        tcpClient->write(message);
        emit closeAll();
        event->accept();
    }
    else
        event->ignore();
}
void sngChat::getMsgList(QString msg)
{
    QStringList tmp = msg.split("|");
    msgList.insert(tmp[0], tmp[1] + "|" + tmp[2]);
    button[tmp[0]]->setText(tr("*  ")+ tmp[0]+tr("  !!!!message"));
}
void sngChat::usrInfo(QString usr, QString IP)
{
    uname = usr;
    uIP = IP;
}
QString sngChat::getUsr()
{
    return uname;
}
void sngChat::getTcpClient(QTcpSocket *socket)
{
    tcpClient = socket;
}
void sngChat::getoIP(QString IP)
{
    toIP = IP;
}
void sngChat::groupChat()
{
    groupchat = new lochat(0);
    groupchat->getUname(getUsr());
    groupchat->show();
}
void sngChat::destroy(QString usr)
{
    if(chatWindowList.contains(usr))
    {
        chatWindowList.remove(usr);
    }
}
sngChat::~sngChat()
{

}
