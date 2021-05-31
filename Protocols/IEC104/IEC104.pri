# iec104
QT -= gui
CONFIG += c++11

HEADERS += \
    $$PWD/iec104protocol.h \
    $$PWD/frame/iec104frame.h \
    $$PWD/frame/iec104enums.h \
    $$PWD/frame/iec104apci.h

SOURCES += \
    $$PWD/iec104protocol.cpp \
    $$PWD/frame/iec104frame.cpp \
    $$PWD/frame/iec104apci.cpp


