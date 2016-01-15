#include <QCoreApplication>
#include <QDebug>

#include "remoteidentify.h"
#include "commandserver.h"

#include "wiringPi.h"
#include "mcp23s17.h"

#define LED 0
void toggle(int val)
{
    for (int i=0; i<1; i++)
    {
        digitalWrite (128 + i, val) ;
        qDebug() << i;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int b,c,d,e,f;
    piBoardId(&f, &b, &c, &d, &e);
    qDebug() << "Hello world" << f << b << c << d << e;

    wiringPiSetup () ;

    mcp23s17Setup(128, 0, 0);

    for (int i=0; i<16; i++)
    {
        pinMode (128 + i, OUTPUT) ;
    }

    pinMode (LED, OUTPUT) ;

    for (int i=0; i<5; ++i)
    {
        qDebug() << "High";
        toggle(HIGH) ;	// On
        digitalWrite (LED, HIGH) ;
        delay (500) ;		// mS
        qDebug() << "Low";
        toggle(LOW) ;	// Off
        digitalWrite (LED, LOW) ;
        delay (500) ;
    }

    return 0;

    RemoteIdentify id;
    CommandServer s;

    return a.exec();
}
