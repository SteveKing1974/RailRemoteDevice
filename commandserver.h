#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QObject>

class CommandServer : public QTcpServer
{
public:
    CommandServer();
    ~CommandServer();
};

#endif // COMMANDSERVER_H
