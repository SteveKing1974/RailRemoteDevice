#include "hardwarecontroller.h"
#include "wiringPi.h"
#include "mcp23s17.h"

const int k_IOExtenderA = 128;
const int k_IOExtenderB = k_IOExtenderA + 16;
const int k_IOExtenderC = k_IOExtenderB + 16;

HardwareController::HardwareController(QObject *parent) : QObject(parent)
{
    wiringPiSetup () ;
    mcp23s17Setup(k_IOExtenderA, 0, 0);
    mcp23s17Setup(k_IOExtenderB, 0, 1);
    mcp23s17Setup(k_IOExtenderC, 0, 2);

    for (int i=0; i<16; i++)
    {
        pinMode (k_IOExtenderA + i, OUTPUT) ;
        digitalWrite (k_IOExtenderA + i, HIGH);
    }


    m_Points.insert("innerStationSwitch", new PointController("innerStationSwitch", k_IOExtenderA, k_IOExtenderA + 1));
    m_Points.insert("innerSwitchLeft", new PointController("innerSwitchLeft", k_IOExtenderA + 2, k_IOExtenderA + 3));
    m_Points.insert("innerSwitchRight", new PointController("innerSwitchRight", k_IOExtenderA + 4, k_IOExtenderA + 5));
    m_Points.insert("leftSidingSwitch", new PointController("leftSidingSwitch", k_IOExtenderA + 6, k_IOExtenderA + 7));
    m_Points.insert("leftSidingSwitch2", new PointController("leftSidingSwitch2", k_IOExtenderA + 8, k_IOExtenderA + 9));
    m_Points.insert("outerSwitchLeft", new PointController("outerSwitchLeft", k_IOExtenderA + 10, k_IOExtenderA + 11));
    m_Points.insert("outerSwitchRight", new PointController("outerSwitchRight", k_IOExtenderA + 12, k_IOExtenderA + 13));
    m_Points.insert("rightSidingSwitch2", new PointController("rightSidingSwitch2", k_IOExtenderA + 14, k_IOExtenderA + 15));

    m_Points.insert("stationInnerLoopSwitchLeft", new PointController("stationInnerLoopSwitchLeft", k_IOExtenderB, k_IOExtenderB + 1));
    m_Points.insert("stationInnerLoopSwitchRight", new PointController("stationInnerLoopSwitchRight", k_IOExtenderB + 2, k_IOExtenderB + 3));
    m_Points.insert("stationOuterEntrance", new PointController("stationOuterEntrance", k_IOExtenderB + 4, k_IOExtenderB + 5));
    m_Points.insert("stationOuterLoopSwitchLeft", new PointController("stationOuterLoopSwitchLeft", k_IOExtenderB + 6, k_IOExtenderB + 7));
    m_Points.insert("stationOuterSidingPoints", new PointController("stationOuterSidingPoints", k_IOExtenderB + 8, k_IOExtenderB + 9));
    m_Points.insert("stationOuterToInner", new PointController("stationOuterToInner", k_IOExtenderB + 10, k_IOExtenderB + 11));
}

HardwareController::~HardwareController()
{
    qDeleteAll(m_Points);
}

QStringList HardwareController::allPoints() const
{
    return m_Points.keys();
}

const PointController *HardwareController::getController(const QString &name) const
{
    static PointController dummy;

    if (m_Points.contains(name))
    {
        return m_Points.value(name);
    }

    return &dummy;
}

