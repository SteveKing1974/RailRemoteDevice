#include "commandserver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class CommandBase {
public:
    virtual QJsonDocument parseCommand(const QJsonDocument& cmd)
    {
        QJsonDocument doc;
        QJsonObject obj;

        obj.insert("Command", QJsonValue("Unknown"));
        doc.setObject(obj);

        return doc;
    }
};

class GetStateCommand : public CommandBase {
public:
    QJsonDocument parseCommand(const QJsonDocument& cmd)
    {
        QJsonDocument doc;
        QJsonObject obj;
        QJsonArray data;

        data.push_back(QJsonValue("0;Outer Top;0;0,1"));
        data.push_back(QJsonValue("1;Outer Bottom;0;0,1"));
        data.push_back(QJsonValue("2;Inner Top;0;0,2"));
        data.push_back(QJsonValue("3;Inner Bottom;0;0,1,2"));
        data.push_back(QJsonValue("4;Station Outer;0;0,1,2,3"));
        data.push_back(QJsonValue("5;Run Around;0;0,1,2,3"));
        data.push_back(QJsonValue("6;Station Inner;0;0,1,2,3"));

        data.push_back(QJsonValue("7;Left Siding Bottom;0;0,1,2,3"));
        data.push_back(QJsonValue("8;Left Siding Middle;0;0,1,2,3"));
        data.push_back(QJsonValue("9;Left Siding Top;0;0,1,2,3"));

        data.push_back(QJsonValue("10;Right Hand Loading;0;0,1,2,3"));

        data.push_back(QJsonValue("11;Right Siding Lower;0;0,1,2,3"));
        data.push_back(QJsonValue("12;Right Siding Middle;0;0,1,2,3"));
        data.push_back(QJsonValue("13;Right Siding Bottom;0;0,1,2,3"));

        obj.insert("Command", QJsonValue("GetState"));
        obj.insert("State", QJsonValue(data));
        doc.setObject(obj);

        return doc;
    }
};

class SetStateCommand : public CommandBase {
public:
    QJsonDocument parseCommand(const QJsonDocument& cmd)
    {
        QJsonDocument doc;
        QJsonObject obj;
        QJsonArray data;

        data.push_back(QJsonValue("0;Outer Top;0;0,1"));
        data.push_back(QJsonValue("1;Outer Bottom;0;0,1"));
        data.push_back(QJsonValue("2;Inner Top;0;0,2"));
        data.push_back(QJsonValue("3;Inner Bottom;0;0,1,2"));
        data.push_back(QJsonValue("4;Station Outer;0;0,1,2,3"));
        data.push_back(QJsonValue("5;Run Around;0;0,1,2,3"));
        data.push_back(QJsonValue("6;Station Inner;0;0,1,2,3"));

        data.push_back(QJsonValue("7;Left Siding Bottom;0;0,1,2,3"));
        data.push_back(QJsonValue("8;Left Siding Middle;0;0,1,2,3"));
        data.push_back(QJsonValue("9;Left Siding Top;0;0,1,2,3"));

        data.push_back(QJsonValue("10;Right Hand Loading;0;0,1,2,3"));

        data.push_back(QJsonValue("11;Right Siding Lower;0;0,1,2,3"));
        data.push_back(QJsonValue("12;Right Siding Middle;0;0,1,2,3"));
        data.push_back(QJsonValue("13;Right Siding Bottom;0;0,1,2,3"));

        obj.insert("Command", QJsonValue("GetState"));
        obj.insert("State", QJsonValue(data));
        doc.setObject(obj);

        return doc;
    }
};


CommandServer::CommandServer()
{
    m_pServer = new QTcpServer(this);
    m_pSignalMap = new QSignalMapper(this);

    m_pServer->listen(QHostAddress::Any, 7757);

    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(m_pSignalMap, SIGNAL(mapped(QObject*)), this, SLOT(dataAvailable(QObject*)));

    m_CommandTable.insert("GetState", new GetStateCommand);
    m_CommandTable.insert("SetState", new SetStateCommand);
}

CommandServer::~CommandServer()
{
    qDeleteAll(m_CommandTable);
}

void CommandServer::newConnection()
{
    QTcpSocket* pSock = m_pServer->nextPendingConnection();

    if (pSock)
    {
        connect(pSock, SIGNAL(readyRead()), m_pSignalMap, SLOT(map()));
        m_pSignalMap->setMapping(pSock, pSock);
    }
}

void CommandServer::dataAvailable(QObject *socket)
{
    QTcpSocket* pSock = qobject_cast<QTcpSocket*>(socket);
    QDataStream strm(pSock);

    char* s = 0;
    uint l = 0;
    strm.readBytes(s, l);
    QByteArray data(s, l);
    delete [] s;

    QJsonDocument cmdDoc = QJsonDocument::fromBinaryData(data);
    QJsonObject cmdObj = cmdDoc.object();

    QJsonDocument returnDoc = m_CommandTable.value(cmdObj.value("Command").toString())->parseCommand(cmdDoc);

    data = returnDoc.toBinaryData();
    strm.writeBytes(data.data(), data.length());

}

