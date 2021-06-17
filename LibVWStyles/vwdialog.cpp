#include <QMouseEvent>
#include <QPushButton>
#include <QToolButton>
#include "vwdialog.h"

// *************************** VWDialogData *********************************************
namespace VWDialogData
{
    static const char* s_propertyTitleWidget = "vwdialog_titlewidget";
    inline QWidget* titleWidget(VWDialog* dialog) {
        return qvariant_cast<QWidget* >(dialog->property(s_propertyTitleWidget));
    }
}


// *************************** VWDialog *************************************************
VWDialog::VWDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags), PlatformFeature()
{
    setAttribute(Qt::WA_Hover, true);
}

VWDialog::VWDialog(QDialogPrivate &dd, QWidget *parent, Qt::WindowFlags f) : QDialog(dd, parent, f), PlatformFeature()
{
    setAttribute(Qt::WA_Hover, true);
}

VWDialog::~VWDialog()
{

}

void VWDialog::resizeEvent(QResizeEvent *event)
{
    PlatformFeature::resizeEventWrapper(this);
    return QDialog::resizeEvent(event);
}

void VWDialog::mousePressEvent(QMouseEvent *event)
{
    if (auto titleWidget = VWDialogData::titleWidget(this)) {
        PlatformFeature::mousePressEventWrapper(this, event, titleWidget, [=](QWidget* child) {
            if (!qobject_cast<QPushButton* >(child) && !qobject_cast<QToolButton* >(child))
                return true;
            return false;
        });
    }
    return QDialog::mousePressEvent(event);
}

void VWDialog::moveEvent(QMoveEvent* event)
{
    Q_UNUSED(event);
}

void VWDialog::mouseReleaseEvent(QMouseEvent *event)
{
    PlatformFeature::mouseReleaseEventWrapper();
    QDialog::mouseReleaseEvent(event);
}

bool VWDialog::event(QEvent* event)
{
    PlatformFeature::eventWrapper(this, event);
    return QDialog::event(event);
}

bool VWDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (auto titleWidget = VWDialogData::titleWidget(this)) {
        if (PlatformFeature::nativeEventWrapper(this, eventType, message, result, titleWidget, [=](QWidget* child) { return !qobject_cast<QPushButton* >(child) && !qobject_cast<QToolButton* >(child); }))
            return true;
    }

    return QDialog::nativeEvent(eventType, message, result);
}

bool VWDialog::eventFilter(QObject* watched, QEvent* event)
{
    if (PlatformFeature::eventFilterWrapper(this, event))
        return true;
    return QDialog::eventFilter(watched, event);
}

void VWDialog::setTitleWidget(QWidget *titleWidget)
{
    Q_ASSERT_X(titleWidget != nullptr, "VWDialog::setTitleWidget", "titleWidget cannot be nullptr");
    if (!titleWidget)
        return;

    setProperty(VWDialogData::s_propertyTitleWidget, QVariant::fromValue(titleWidget));
    titleWidget->installEventFilter(this);
    titleWidget->setMouseTracking(true);

    PlatformFeature::initialize(this);
}
