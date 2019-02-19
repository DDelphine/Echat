#include "tools.h"
#include <QDebug>

// read usrInfo from file
QMap<QString, QString> usrInfo(QString file)
{
   QFile fp(file);
   if(!fp.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       //qDebug()<<"Can't open the file!"<<endl;
       exit(0);
   }
   QMap<QString, QString> usrList;
   QString line;
   QTextStream in(&fp);

   while(!in.atEnd())
   {
       line = in.readLine();
       line = line.replace(QRegExp("\\s+"), "");
       QStringList tmp = line.split("|||");
       usrList.insert(tmp[0], tmp[1]);
   }
   fp.close();
   return usrList;
}

// save new sigin usr
bool addUsr(QString usr, QString IP, QString file)
{
    QFile fp(file);
    if(!fp.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug()<<"open failed";
        return false;
    }
    QTextStream out(&fp);
    out<<usr<<"|||"<<IP<<"\n";
    return true;
}
