#include "socketthread.h"

socketThread::socketThread(QWidget *parent, QMap<QString, QString> *uIP,
                           QMap<QString, QList<QString>> *uList,
                           QMap<QString, QString> *mList, int socketDescriptor):
    QThread(parent), socketDescriptor(socketDescriptor)
{
    usr2IP = uIP;
    usrList = uList;
    msgList = mList;
    qDebug()<<"Thread:: "<<QThread::currentThreadId();
    //tcpSever = socket;
}
void socketThread::run()
{
    qDebug()<<"Running a new thread!"<<endl;
    tcpSocketR = new QTcpSocket();
    qDebug()<<"Thread:: "<<QThread::currentThreadId();
    //tcpSocketR = tcpSever->nextPendingConnection();
    if(!tcpSocketR->setSocketDescriptor(socketDescriptor))
    {
        qDebug()<<"error in socket discriptor"<<endl;
    }
    // receive msg
    connect(tcpSocketR, SIGNAL(readyRead()), this, SLOT(readMsg()),Qt::DirectConnection);
    //connect(tcpSocketR, SIGNAL(disconnected()), tcpSocketR,,SLOT(deleteLater()));
    exec();
}
void socketThread::readMsg()
{
    qDebug()<<"Thread:: "<<QThread::currentThreadId();
    qDebug()<<"read message!!!"<<endl;
    QString msgTp;
    QString usr;
    QString IP;
    QDataStream in(tcpSocketR);
    in.setVersion(QDataStream::Qt_5_8);
    in>>msgTp;
    qDebug()<<"Message Type: "<<msgTp<<endl;
    if(msgTp==tr("login"))
    {
        in>>usr>>IP;

        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        emit updateUI(tr("connect") + tr("|") + time + tr("|") + usr);// update UI
        //tcpSocketMap.insert(usr, tcpSocketR);// save usr's socket to map
        // log usr and IP to usr2IP
        if((*usr2IP).contains(usr))
        {
            (*usr2IP)[usr] = IP;
        }
        else
        {
            usr2IP->insert(usr, IP);
        }
        // check whether the user is registered.
        if((*usrList).contains(usr)) // send friend list to usr.
        {
            QString tmp;
            QList<QString>::iterator iter;
            int k=0;
            for(iter=(*usrList)[usr].begin(); iter!=(*usrList)[usr].end(); ++iter)
            {
                if(k==0) tmp = (*iter);
                else tmp += "|" + (*iter);
                ++k;
            }
            //qDebug()<<"send friends list start"<<endl;
            sendMsg(tcpSocketR, tr("friends"));
            sendMsg(tcpSocketR, tmp);
            //qDebug()<<"send friends list ok"<<endl;
            if((*msgList).contains(usr))// have message.
            {
                qDebug()<<"send message to "<<usr<<endl;
                sendMsg(tcpSocketR, tr("message"));
                QString tmp = (*msgList)[usr];
                sendMsg(tcpSocketR, tmp);
                qDebug()<<"send Message OK!"<<endl;
                (*msgList).remove(usr);// clear message.
            }
        }
        else // register and save
        {
            QList<QString> tmp;
            (*usrList).insert(usr, tmp);
        }
     }
    if(msgTp=="alive")
    {
        in>>usr;
        if((*usr2IP).contains(usr))
        {
            sendMsg(tcpSocketR, tr("online"));
            QString tmp = usr + "|" + (*usr2IP)[usr];
            sendMsg(tcpSocketR, tmp);
        }
        else
        {
            sendMsg(tcpSocketR, tr("offline"));
            sendMsg(tcpSocketR, usr);
        }
    }
    if(msgTp=="message")
    {
        QString message;
        in>>message;
        qDebug()<<"message is : "<<message<<endl;
        QStringList tmp = message.split('|');
        (*msgList)[tmp[1]] = tmp[0] + "|" + tmp[2] + "|" + tmp[3];
    }
    if(msgTp=="quit")
    {
        QString uname;
        in>>uname;
        (*usr2IP).remove(uname);
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        emit updateUI(tr("quit") + tr("|") + time + tr("|") + uname);// update UI
    }
}
void socketThread::sendMsg(QTcpSocket *tcpSocket, QString msg)
{
    QByteArray block; // sending usrList
    QDataStream out(&block,QIODevice::WriteOnly);
    out<<msg;
    tcpSocket->write(block);
}
socketThread::~socketThread()
{

}
