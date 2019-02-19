#include "widget.h"
#include "ui_widget.h"
#include <QDir>
#include <QDebug>
#include <QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("lochatServer"));
    QString path = QDir::currentPath();
    //qDebug()<<path<<endl;
    QString file = path + "/../lochatServer/" + "usrList.txt";
    usrList = usrInfo(file);
 /*
    // travel all users
    QMap<QString, QList<QString>>::const_iterator iter;
    QList<QString> tmp;
    QList<QString>::iterator liter;

    for(iter=usrList.constBegin(); iter!=usrList.constEnd(); ++iter)
    {
        qDebug()<<iter.key()<<"'s friends are:";
        tmp = iter.value();
        for(liter=tmp.begin(); liter!=tmp.end(); ++liter)
        {
            qDebug()<<" "<<(*liter);
        }
    }
*/
    port = 6666;
    // tcp
    tcpServer = new TcpServer(0, &usr2IP, &usrList, &msgList);

    if(!tcpServer->listen(QHostAddress::Any, port)||!tcpServer->isListening())
    {
        qDebug()<<"Listening error!!"<<endl;
        qDebug()<<tcpServer->errorString();
        close();
    }
    connect(tcpServer, SIGNAL(updateUI(QString)), this, SLOT(updateUI(QString)));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(close()));

}

void Widget::updateUI(QString msg)
{
    QStringList tmp = msg.split("|");
    if(tmp[0] == "connect")
    {
        ui->lochatServer->setTextColor(Qt::black);
        ui->lochatServer->append(tmp[1]);
        ui->lochatServer->setTextColor(Qt::blue);
        ui->lochatServer->append("<<<<<  " + tmp[2] + " connected.");

    }
    if(tmp[0] == "quit")
    {
        ui->lochatServer->setTextColor(Qt::black);
        ui->lochatServer->append(tmp[1]);
        ui->lochatServer->setTextColor(Qt::blue);
        ui->lochatServer->append("<<<<<  " + tmp[2] + " quit.");

    }
}
Widget::~Widget()
{
    delete ui;
}
