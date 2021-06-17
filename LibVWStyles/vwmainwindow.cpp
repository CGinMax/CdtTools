#include <QMouseEvent>
#include <QPushButton>
#include <QToolButton>
#include "vwmainwindow.h"

// *************************** VWMainWindowData *****************************************
namespace VWMainWindowData
{
    static const char* s_propertyTitleWidget = "vwmainwindow_titlewidget";
    inline QWidget* titleWidget(VWMainWindow* mainWindow) {
        return qvariant_cast<QWidget* >(mainWindow->property(s_propertyTitleWidget));
    }
}


// *************************** VWMainWindow *********************************************
VWMainWindow::VWMainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), PlatformFeature()
{

}

VWMainWindow::~VWMainWindow()
{

}

void VWMainWindow::resizeEvent(QResizeEvent *event)
{
    PlatformFeature::resizeEventWrapper(this);
    return QMainWindow::resizeEvent(event);
}

void VWMainWindow::mousePressEvent(QMouseEvent *event)
{
    if (auto titleWidget = VWMainWindowData::titleWidget(this)) {
        PlatformFeature::mousePressEventWrapper(this, event, titleWidget, [=](QWidget* child) {
            if (!qobject_cast<QPushButton* >(child) && !qobject_cast<QToolButton* >(child))
                return true;
            return false;
        });
    }
    return QMainWindow::mousePressEvent(event);
}

void VWMainWindow::moveEvent(QMoveEvent* event)
{
    Q_UNUSED(event);
}

void VWMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    PlatformFeature::mouseReleaseEventWrapper();
    QMainWindow::mouseReleaseEvent(event);
}

bool VWMainWindow::event(QEvent* event)
{
    PlatformFeature::eventWrapper(this, event);
    return QMainWindow::event(event);
}

bool VWMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (auto titleWidget = VWMainWindowData::titleWidget(this)) {
        if (PlatformFeature::nativeEventWrapper(this, eventType, message, result, titleWidget, [=](QWidget* child) { return !qobject_cast<QPushButton* >(child) && !qobject_cast<QToolButton* >(child); }))
            return true;
    }

    return QMainWindow::nativeEvent(eventType, message, result);
}

bool VWMainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (auto titleWidget = VWMainWindowData::titleWidget(this)) {
        if (watched == titleWidget) {
            if (event->type() == QEvent::MouseButtonDblClick) {
                isMaximized() ? this->showNormal() : this->showMaximized();
                return true;
            } else {
                if (PlatformFeature::eventFilterWrapper(this, event))
                    return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void VWMainWindow::setTitleWidget(QWidget *titleWidget)
{
    Q_ASSERT_X(titleWidget != nullptr, "VWMainWindow::setTitleWidget", "titleWidget cannot be nullptr");
    if (!titleWidget)
        return;

    setProperty(VWMainWindowData::s_propertyTitleWidget, QVariant::fromValue(titleWidget));
    titleWidget->installEventFilter(this);
    titleWidget->setMouseTracking(true);

    PlatformFeature::initialize(this);
}
