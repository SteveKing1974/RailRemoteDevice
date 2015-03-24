#ifndef REMOTEIDENTIFY_H
#define REMOTEIDENTIFY_H

#include <QUdpSocket>

class RemoteIdentify : public QUdpSocket
{
    Q_OBJECT

public:
    RemoteIdentify();
    ~RemoteIdentify();
private slots:
    void readPendingDatagrams();
};

#endif // REMOTEIDENTIFY_H
