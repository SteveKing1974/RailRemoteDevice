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
    virtual ~CommandBase() {}
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

        QJsonObject points;

        QHash<QString, int> dummyPoints;

        dummyPoints.insert("innerStationSwitch", 1);
        dummyPoints.insert("innerSwitchLeft", 1);
        dummyPoints.insert("innerSwitchRight", 1);

        dummyPoints.insert("leftSidingSwitch", 1);
        dummyPoints.insert("leftSidingSwitch2", 1);
        dummyPoints.insert("outerSwitchLeft", 1);
        dummyPoints.insert("outerSwitchRight", 1);
        dummyPoints.insert("rightSidingSwitch2", 1);
        dummyPoints.insert("stationInnerLoopSwitchLeft", 1);
        dummyPoints.insert("stationInnerLoopSwitchRight", 1);
        dummyPoints.insert("stationOuterEntrance", 1);
        dummyPoints.insert("stationOuterLoopSwitchLeft", 1);
        dummyPoints.insert("stationOuterSidingPoints", 1);
        dummyPoints.insert("stationOuterToInner", 1);


        QHashIterator<QString, int> pIter(dummyPoints);
        while (pIter.hasNext()) {
            pIter.next();
            QJsonObject nextPoint;
            nextPoint.insert("state", pIter.value());
            points.insert(pIter.key(), nextPoint);
        }

        QHash<QString, int> dummyBreaks;
        dummyBreaks.insert("leftSidingBreak", 1);
        dummyBreaks.insert("rightSidingBreak", 1);
        dummyBreaks.insert("stationInnerHeadShuntRightBreak", 1);

        QHashIterator<QString, int> bIter(dummyBreaks);
        QJsonObject breaks;
        while (bIter.hasNext()) {
            bIter.next();
            QJsonObject nextBreak;
            nextBreak.insert("state", bIter.value());
            breaks.insert(bIter.key(), nextBreak);
        }

        QJsonObject topLevel;
        topLevel.insert("points", points);
        topLevel.insert("breaks", breaks);

        obj.insert("Command", QJsonValue("GetState"));
        obj.insert("Data", topLevel);
        doc.setObject(obj);

        return doc;
    }
};

class SetStateCommand : public CommandBase {
public:
    QJsonDocument parseCommand(const QJsonDocument& cmd)
    {
//        QJsonObject cmdObj = cmd.object();
//        if (cmdObj.value("Command").toString()=="GetState")
//        {
//            emit gotState(SectionData::fromJsonDoc(cmdDoc));
//        }

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

    qDebug() << "Listen" << m_pServer->listen(QHostAddress::Any, 7757);

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

    qDebug() << "New connection" << pSock;
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

