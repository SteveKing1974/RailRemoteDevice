#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QObject>
#include <QHash>
#include <QString>

class QTcpServer;
class QSignalMapper;

class CommandBase;

class CommandServer : public QObject
{
    Q_OBJECT

public:
    CommandServer();
    virtual ~CommandServer();

private slots:
    void newConnection();
    void dataAvailable(QObject* socket);

private:
    QTcpServer* m_pServer;
    QSignalMapper* m_pSignalMap;
    QHash<QString, CommandBase*> m_CommandTable;

};

#endif // COMMANDSERVER_H
