# ui.pri

QT += gui core widgets network serialport

CONFIG += c++11

HEADERS += \
    $$PWD/expand/expandwidgetitem.h \
    $$PWD/expand/expandwidgetitem_p.h \
    $$PWD/expand/expandtile.h \
    $$PWD/expand/expandwidget.h \
    $$PWD/expand/tileicon.h \
    $$PWD/expand/gatheroperwidget.h \
    $$PWD/tables/listviewitem.h \
    $$PWD/gatherdetailpage.h \
    $$PWD/tables/ybtableview.h \
    $$PWD/tables/listheader.h \
    $$PWD/buttons/fabcircularmenu.h \
    $$PWD/editableeventfilter.h \
    $$PWD/expand/gathercontroller.h \
    $$PWD/dialogs/serialportdialog.h \
    $$PWD/buttons/rippleanimation.h \
    $$PWD/page/tablepage.h \
    $$PWD/buttons/fabbutton.h \
    $$PWD/notificationbtn.h \
    $$PWD/buttons/iconbutton.h \
    $$PWD/buttons/abstractbutton.h \
    $$PWD/buttons/abstractbutton_p.h \
    $$PWD/buttons/rippleeffect.h \
    $$PWD/base/style.h \
    $$PWD/base/theme.h \
    $$PWD/effect/shadoweffect.h \
    $$PWD/buttons/raisebutton.h \
    $$PWD/buttons/raisebutton_p.h \
    $$PWD/base/painterhighqualityenabler.h \
    $$PWD/buttons/flatbutton.h \
    $$PWD/buttons/flatbutton_p.h \
    $$PWD/buttons/outlinebutton.h \
    $$PWD/buttons/outlinebutton_p.h \
    $$PWD/buttons/iconbutton_p.h \
    $$PWD/buttons/fabbutton_p.h \
    $$PWD/base/faicon.h \
    $$PWD/notification/snackbar.h \
    $$PWD/notification/snackbar_p.h \
    $$PWD/base/base_type.h \
    $$PWD/animation/slideanimation.h \
    $$PWD/page/gatherpage.h \
    $$PWD/dialogs/sensoradddialog.h \
    $$PWD/dialogs/sensoroperationdlg.h \
    $$PWD/page/serialportpage.h \
    $$PWD/page/configurationpage.h

SOURCES += \
    $$PWD/expand/expandwidgetitem.cpp \
    $$PWD/expand/expandtile.cpp \
    $$PWD/expand/expandwidget.cpp \
    $$PWD/expand/tileicon.cpp \
    $$PWD/expand/gatheroperwidget.cpp \
    $$PWD/tables/listviewitem.cpp \
    $$PWD/gatherdetailpage.cpp \
    $$PWD/tables/ybtableview.cpp \
    $$PWD/tables/listheader.cpp \
    $$PWD/buttons/fabcircularmenu.cpp \
    $$PWD/editableeventfilter.cpp \
    $$PWD/expand/gathercontroller.cpp \
    $$PWD/dialogs/serialportdialog.cpp \
    $$PWD/buttons/rippleanimation.cpp \
    $$PWD/page/tablepage.cpp \
    $$PWD/buttons/fabbutton.cpp \
    $$PWD/notificationbtn.cpp \
    $$PWD/buttons/iconbutton.cpp \
    $$PWD/buttons/abstractbutton.cpp \
    $$PWD/buttons/rippleeffect.cpp \
    $$PWD/base/style.cpp \
    $$PWD/base/theme.cpp \
    $$PWD/effect/shadoweffect.cpp \
    $$PWD/buttons/raisebutton.cpp \
    $$PWD/base/painterhighqualityenabler.cpp \
    $$PWD/buttons/flatbutton.cpp \
    $$PWD/buttons/outlinebutton.cpp \
    $$PWD/base/faicon.cpp \
    $$PWD/notification/snackbar.cpp \
    $$PWD/animation/slideanimation.cpp \
    $$PWD/page/gatherpage.cpp \
    $$PWD/dialogs/sensoradddialog.cpp \
    $$PWD/dialogs/sensoroperationdlg.cpp \
    $$PWD/page/serialportpage.cpp \
    $$PWD/page/configurationpage.cpp

FORMS += \
    $$PWD/expand/expandwidget.ui \
    $$PWD/expand/expandtile.ui \
    $$PWD/expand/gatheroperwidget.ui \
    $$PWD/tables/listviewitem.ui \
    $$PWD/gatherdetailpage.ui \
    $$PWD/tables/listheader.ui \
    $$PWD/dialogs/serialportdialog.ui \
    $$PWD/page/tablepage.ui \
    $$PWD/page/gatherpage.ui \
    $$PWD/dialogs/sensoradddialog.ui \
    $$PWD/dialogs/sensoroperationdlg.ui \
    $$PWD/page/serialportpage.ui \
    $$PWD/page/configurationpage.ui
