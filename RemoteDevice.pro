#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T16:27:27
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = RemoteDevice
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    remoteidentify.cpp \
    commandserver.cpp

*pi*: {
    target.path = /home/pi/Rail
    INSTALLS += target

    INCLUDEPATH += ../wiringPi/wiringPi/

    SOURCES +=  \
        ../wiringPi/wiringPi/mcp23s17.c \
        ../wiringPi/wiringPi/wiringPiSPI.c \
        ../wiringPi/wiringPi/wiringPi.c \
        ../wiringPi/wiringPi/softPwm.c \
        ../wiringPi/wiringPi/softTone.c \
        ../wiringPi/wiringPi/piHiPri.c

    HEADERS += \
        ../wiringPi/wiringPi/mcp23s17.h \
        ../wiringPi/wiringPi/wiringPiSPI.h \
        ../wiringPi/wiringPi/wiringPi.h \
        ../wiringPi/wiringPi/softPwm.h \
        ../wiringPi/wiringPi/softTone.h
}

HEADERS += \
    remoteidentify.h \
    commandserver.h



