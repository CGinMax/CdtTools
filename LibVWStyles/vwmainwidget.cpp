#include "vwmainwidget.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QToolButton>
// *************************** VWMainWidgetData *****************************************
namespace VWMainWidgetData
{
    static const char* s_propertyTitleWidget = "vwmainwidget_titlewidget";
    inline QWidget* titleWidget(VWMainWidget* mainWidget) {
        return qvariant_cast<QWidget* >(mainWidget->property(s_propertyTitleWidget));
    }
}


VWMainWidget::VWMainWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{

}

VWMainWidget::~VWMainWidget()
{

}

void VWMainWidget::resizeEvent(QResizeEvent *event)
{
    PlatformFeature::resizeEventWrapper(this);
    return QWidget::resizeEvent(event);
}

void VWMainWidget::mousePressEvent(QMouseEvent *event)
{
    if (auto titleWidget = VWMainWidgetData::titleWidget(this)) {
        PlatformFeature::mousePressEventWrapper(this, event, titleWidget, [=](QWidget* child) {
            if (!qobject_cast<QPushButton* >(child) && !qobject_cast<QToolButton* >(child))
                return true;
            return false;
        });
    }
    return QWidget::mousePressEvent(event);
}

void VWMainWidget::moveEvent(QMoveEvent* event)
{
    Q_UNUSED(event);
}

void VWMainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    PlatformFeature::mouseReleaseEventWrapper();
    QWidget::mouseReleaseEvent(event);
}

bool VWMainWidget::event(QEvent* event)
{
    PlatformFeature::eventWrapper(this, event);
    return QWidget::event(event);
}

bool VWMainWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (auto titleWidget = VWMainWidgetData::titleWidget(this)) {
        if (PlatformFeature::nativeEventWrapper(this, eventType, message, result, titleWidget, [=](QWidget* child) { return !qobject_cast<QPushButton* >(child) && !qobject_cast<QToolButton* >(child); }))
            return true;
    }

    return QWidget::nativeEvent(eventType, message, result);
}

bool VWMainWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (auto titleWidget = VWMainWidgetData::titleWidget(this)) {
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
    return QWidget::eventFilter(watched, event);
}

void VWMainWidget::setTitleWidget(QWidget *titleWidget)
{
    Q_ASSERT_X(titleWidget != nullptr, "VWMainWidget::setTitleWidget", "titleWidget cannot be nullptr");
    if (!titleWidget)
        return;

    setProperty(VWMainWidgetData::s_propertyTitleWidget, QVariant::fromValue(titleWidget));
    titleWidget->installEventFilter(this);
    titleWidget->setMouseTracking(true);

    PlatformFeature::initialize(this);
}

