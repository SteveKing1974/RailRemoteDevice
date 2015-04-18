#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#include <QObject>

class HardwareController : public QObject
{
    Q_OBJECT
public:
    explicit HardwareController(QObject *parent = 0);
    ~HardwareController();

signals:

public slots:
};

#endif // HARDWARECONTROLLER_H
