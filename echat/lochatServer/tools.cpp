#include "tools.h"
#include <QDebug>
#include <QDir>
// read usrInfo from file
QMap<QString, QList<QString>> usrInfo(QString file)
{
   QFile fp(file);
   if(!fp.exists())
   {
       qDebug()<<"File not exits!"<<endl;
       exit(0);
   }
   if(!fp.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       qDebug()<<"Can't open the file!"<<endl;
       exit(0);
   }
   QMap<QString, QList<QString>> usrList;
   QString line;
   QTextStream in(&fp);

   while(!in.atEnd())
   {
       line = in.readLine();
       line = line.replace(QRegExp("\\s+"), "");
       QStringList tmp = line.split("|||");
       QStringList fd = tmp[1].split("|");
       QList<QString> friendList;
       for(int i=0; i<fd.length(); i++)
       {
           friendList<<fd[i];
           //qDebug()<<fd[i];
       }
       usrList.insert(tmp[0], friendList);
   }
   fp.close();
   return usrList;
}

// save new sigin usr
bool addUsr(QString usr, QList<QString>friends, QString file)
{
    QFile fp(file);
    if(!fp.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug()<<"open failed";
        return false;
    }
    QList<QString>::iterator iter;
    QString tmp;
    int k=0;
    for(iter=friends.begin(); iter!=friends.end(); ++iter)
    {
        if(k==0) tmp = (*iter);
        else tmp += "|" + (*iter);
        ++k;
    }
    QTextStream out(&fp);
    out<<usr<<"|||"<<tmp<<"\n";
    return true;
}
