#include "remoteidentify.h"

#include <QHostAddress>

RemoteIdentify::RemoteIdentify() : QUdpSocket(0)
{
    bind(7755, QUdpSocket::ShareAddress);

    connect(this, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

RemoteIdentify::~RemoteIdentify()
{

}

void RemoteIdentify::readPendingDatagrams()
{
    qDebug() << "Got one";
    while (hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        qDebug() << "Sending back" << sender << senderPort;
        writeDatagram("hello", sender, 7756);
    }
}
