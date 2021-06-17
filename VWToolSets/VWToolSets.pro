
QT       += core gui serialport widgets network
TARGET = VWToolSets
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11 warn_on

include(ui/ui.pri)

SOURCES += \
        main.cpp \
    vwtoolsetswindow.cpp

HEADERS += \
    vwtoolsetswindow.h

FORMS += \
    vwtoolsetswindow.ui

INCLUDEPATH += $$PWD/../spdlog/include
DEPENDPATH += $$PWD/../spdlog/include
INCLUDEPATH += $$PWD/../LibVWStyles
DEPENDPATH += $$PWD/../LibVWStyles

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug
}
else{
    DESTDIR = $$PWD/../bin/release
}
LIBS += -L$${DESTDIR} -lspdlog
LIBS += -L$${DESTDIR} -lLibVWStyles

#TRANSLATIONS += $$PWD/PressPlateTool_en.ts
#TRANSLATIONS += $$PWD/PressPlateTool_zh.ts


QMAKE_RPATHDIR += ./
QMAKE_RPATHDIR += ./lib
QMAKE_RPATHDIR += $${DESTDIR}
QMAKE_RPATHDIR += ./../lib
QMAKE_RPATHDIR += $${DESTDIR}/../lib
