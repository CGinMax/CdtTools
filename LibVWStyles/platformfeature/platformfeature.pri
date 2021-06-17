INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/platformfeature.h

win32 {
    SOURCES += $$PWD/platformfeature_win.cpp

    DEFINES += X11_DEPRECATED

    LIBS += -ldwmapi
    LIBS += -luser32
}

unix:!mac {

    SOURCES += $$PWD/platformfeature_linux.cpp

# 区分arm跟desktop
XSPEC = $$[QMAKE_XSPEC]
equals(XSPEC, devices/linux-buildroot-g++) {
    DEFINES += X11_DEPRECATED
} else {
    HEADERS += $$PWD/x11/shape.h \
            $$PWD/x11/xutil.h

    SOURCES += $$PWD/x11/xutil.cpp

    QT += x11extras
    LIBS += -lX11 -lXext
}
}
