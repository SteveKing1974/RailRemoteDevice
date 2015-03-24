#include <QCoreApplication>
#include <QDebug>

#include "remoteidentify.h"
#include "commandserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Hello world";

    RemoteIdentify id;
    CommandServer s;

    return a.exec();
}
