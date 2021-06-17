QT += core gui widgets
QT += core-private gui-private widgets-private

TEMPLATE = lib
DEFINES += LIBVWSTYLES_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 区分arm跟desktop
XSPEC = $$[QMAKE_XSPEC]
equals(XSPEC, devices/linux-buildroot-g++){
    DEFINES += X11_DEPRECATED
}

SOURCES += \
    libvwstyles.cpp \
    masklayer.cpp \
    vwdialog.cpp \
    vwmainwindow.cpp \
    vwmainwidget.cpp \
    shadoweffect.cpp

HEADERS += \
    libvwstyles_global.h \
    libvwstyles.h \
    masklayer.h \
    vwdialog.h \
    vwmainwindow.h \
    vwmainwidget.h \
    shadoweffect.h

RESOURCES += \
    libvwstyles.qrc

FORMS += \
    masklayer.ui

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

CONFIG(release, debug|release) {
    DESTDIR = $$PWD/../bin/release
}
else:CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../bin/debug
}

include($$PWD/platformfeature/platformfeature.pri)
