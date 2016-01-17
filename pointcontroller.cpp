#include "pointcontroller.h"

#include "wiringPi.h"

#include <QDebug>
PointController::PointController() :
    m_Name(),
    m_PowerLine(-1),
    m_ControlLine(-1)
{

}

PointController::PointController(const QString &name, int powerLine, int controlLine) :
    m_Name(name),
    m_PowerLine(powerLine),
    m_ControlLine(controlLine)
{
    digitalWrite (m_PowerLine, HIGH);
}

void PointController::setDirection(PointController::PointDirection dir) const
{
    if (m_Name.isEmpty() || m_PowerLine<0 || m_ControlLine<0)
    {
        return;
    }

    const int val = (dir == ePointLeft) ? HIGH : LOW;
    qDebug() << "Set control:" << val;
    digitalWrite (m_ControlLine, val);
    delay (500);
    qDebug() << "Set power low";
    digitalWrite (m_PowerLine, LOW);
    delay(500);
    qDebug() << "Set power high";
    digitalWrite (m_PowerLine, HIGH);
}

