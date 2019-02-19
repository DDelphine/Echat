#ifndef TOOLS_H
#define TOOLS_H
#include <QFile>
#include <QMap>
#include <QTextStream>
QMap<QString, QList<QString>> usrInfo(QString file);
bool addUsr(QString usr, QString IP, QString file);

#endif // TOOLS_H
