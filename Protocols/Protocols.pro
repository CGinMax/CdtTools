#-------------------------------------------------
#
# Project created by QtCreator 2021-05-14T10:50:37
#
#-------------------------------------------------

QT -= gui
QT += core
TARGET = Protocols
TEMPLATE = lib

CONFIG += c++11

DEFINES += PROTOCOLS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        iprotocols.cpp \
    convert.cpp

HEADERS += \
        iprotocols.h \
        protocols_global.h \ 
    convert.h

include(YBProtocol/YBProtocol.pri)
include(IEC104/IEC104.pri)
CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug
}
else {
    DESTDIR = $$PWD/../bin/release
}

QMAKE_RPATHDIR += ./
QMAKE_RPATHDIR += ./lib
QMAKE_RPATHDIR += $${DESTDIR}
QMAKE_RPATHDIR += ./../lib
QMAKE_RPATHDIR += $${DESTDIR}/../lib
