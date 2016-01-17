#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include "pointcontroller.h"

class HardwareController : public QObject
{
    Q_OBJECT
public:
    explicit HardwareController(QObject *parent = 0);
    ~HardwareController();

    QStringList allPoints() const;

    const PointController* getController(const QString& name) const;

signals:

public slots:

private:
    QHash<QString, PointController*> m_Points;
};

#endif // HARDWARECONTROLLER_H
