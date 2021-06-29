#include "notificationbtn.h"
#include <QEvent>
#include <QPainter>

NotificationBtn::NotificationBtn(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
    , m_hasNew(true)
{

}

NotificationBtn::~NotificationBtn()
{

}

void NotificationBtn::setHasNewNotification(bool hasNew)
{
    m_hasNew = hasNew;
    update();
}


bool NotificationBtn::event(QEvent *event)
{
    if (event->type() == QEvent::HoverEnter) {
        m_hasNew = false;
        emit showNotification();
    } else if (event->type() == QEvent::HoverLeave) {
        emit hideNotification();
    }
    return QPushButton::event(event);
}

void NotificationBtn::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    if (m_hasNew) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::red);
        painter.drawEllipse(rect().x() + width() - 10,
                            rect().y() + height() - 10,
                            10, 10);

    }
}
