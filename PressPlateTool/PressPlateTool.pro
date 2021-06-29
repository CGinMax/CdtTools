#-------------------------------------------------
#
# Project created by QtCreator 2021-04-28T09:04:46
#
#-------------------------------------------------

QT       += core gui serialport widgets network
TARGET = PressPlateTool
TEMPLATE = app
CONFIG += warn_on

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += fontAwesomeFree
CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui


INCLUDEPATH += $$PWD/../spdlog/include
DEPENDPATH += $$PWD/../spdlog/include

INCLUDEPATH += $$PWD/../Communication/
DEPENDPATH += $$PWD/../Communication/


INCLUDEPATH += $$PWD/../Protocols/
DEPENDPATH += $$PWD/../Protocols/


CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug
}
else{
    DESTDIR = $$PWD/../bin/release
}
    LIBS += -L$${DESTDIR} -lspdlog
    LIBS += -L$${DESTDIR} -lCommunication
    LIBS += -L$${DESTDIR} -lProtocols


TRANSLATIONS += $$PWD/PressPlateTool_en.ts
TRANSLATIONS += $$PWD/PressPlateTool_zh.ts

include($$PWD/ui/ui.pri)
include($$PWD/protocol/protocol.pri)
include($$PWD/common/common.pri)
include($$PWD/../QtAwesome/QtAwesome/QtAwesome.pri)
include($$PWD/../asyncfuture/asyncfuture.pri)

RESOURCES += \
    resources/resources.qrc


QMAKE_RPATHDIR += ./
QMAKE_RPATHDIR += ./lib
QMAKE_RPATHDIR += $${DESTDIR}
QMAKE_RPATHDIR += ./../lib
QMAKE_RPATHDIR += $${DESTDIR}/../lib
