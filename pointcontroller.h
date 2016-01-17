#ifndef POINTCONTROLLER_H
#define POINTCONTROLLER_H

#include <QString>

class PointController
{
public:
    enum PointDirection {
        ePointLeft,
        ePointRight
    };

    PointController();
    PointController(const QString& name, int powerLine, int controlLine);

    void setDirection(PointController::PointDirection dir) const;

private:
    const QString m_Name;
    const int m_PowerLine;
    const int m_ControlLine;
};

#endif // POINTCONTROLLER_H
