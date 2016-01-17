#include <QCoreApplication>
#include <QDebug>

#include "remoteidentify.h"
#include "commandserver.h"
#include "hardwarecontroller.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HardwareController control;
    RemoteIdentify id;
    CommandServer s(&control);

    return a.exec();
}
