#include "lochat.h"
#include "singlechat.h"
#include "login.h"
#include <QApplication>
#include "tools.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login w;
    w.resize(300,120);
    w.show();
    return a.exec();
}
