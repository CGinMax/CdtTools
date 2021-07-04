#include "switchbutton_internal.h"
#include "switchbutton.h"
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QPainter>
#include <QDebug>

SwitchButtonThumb::SwitchButtonThumb(SwitchButton *parent)
    : QWidget(parent)
    , m_switchBtn(parent)
    , m_shift(0)
    , m_offset(0)
{
    Q_ASSERT(parent);

    auto effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(6);
    effect->setColor(QColor(0, 0, 0, 80));
    effect->setOffset(QPointF(0, 1));
    setGraphicsEffect(effect);

    parent->installEventFilter(this);
}

SwitchButtonThumb::~SwitchButtonThumb()
{
}

void SwitchButtonThumb::setShift(qreal shift)
{
    if (qFuzzyCompare(m_shift, shift)) {
        return ;
    }

    m_shift = shift;
    updateOffset();
}

void SwitchButtonThumb::setThumbWidth(qreal thumbWidth)
{
    m_thumbWidth = thumbWidth;
    update();
}

bool SwitchButtonThumb::eventFilter(QObject *obj, QEvent *event)
{
    const QEvent::Type type = event->type();
    if (type == QEvent::Resize || type == QEvent::Move) {
        setGeometry(m_switchBtn->thumbRect());
        updateOffset();
    }

    return QWidget::eventFilter(obj, event);
}

void SwitchButtonThumb::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);

    QPen pen(QBrush(QColor(Qt::white)), m_switchBtn->penWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    QRectF r;
    QRect thumbRect;
    if (m_switchBtn->orientation() == Qt::Horizontal) {
        thumbRect = m_switchBtn->thumbRect();
        int w = thumbRect.height() - (m_shift * (thumbRect.height() - pen.width() * 2)) - pen.width() * 2;
        int h = thumbRect.height() - pen.width() * 2;
        r = QRectF(m_offset, pen.width(), w, h);
//        r = QRectF(m_offset, m_switchBtn->thumbMargins().top(), m_thumbWidth, qMax(static_cast<int>(m_thumbWidth), height() - m_switchBtn->thumbMargins().top() - m_switchBtn->thumbMargins().bottom()));

    } else {
//        r = QRectF(5, 5 + m_offset, width(), m_thumbWidth);
    }

//    painter.drawEllipse(r);
    painter.drawRoundedRect(r, thumbRect.width(), thumbRect.height());

    if (!m_switchBtn->isEnabled()) {
        pen.setColor(m_switchBtn->disabledColor());
        brush.setColor(m_switchBtn->disabledColor());
        painter.setPen(pen);
        painter.drawEllipse(r);
    }

}

void SwitchButtonThumb::updateOffset()
{
    const QSize s(Qt::Horizontal == m_switchBtn->orientation()
                  ? size() : size().transposed());
    auto thumbRect = m_switchBtn->thumbRect();

    m_offset = m_switchBtn->penWidth() * (1 - m_shift) + m_shift * static_cast<qreal>(m_switchBtn->trackRect().width() - m_switchBtn->trackRect().height() / 2);
    update();
}

SwitchButtonTrack::SwitchButtonTrack(SwitchButton *parent)
    : QWidget (parent)
    , m_switchBtn(parent)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);
}

SwitchButtonTrack::~SwitchButtonTrack()
{

}

void SwitchButtonTrack::setTrackColor(const QColor &color)
{
    m_trackColor = color;
    update();
}

bool SwitchButtonTrack::eventFilter(QObject *obj, QEvent *event)
{
    const QEvent::Type type = event->type();

    if (QEvent::Resize == type || QEvent::Move == type) {
        setGeometry(m_switchBtn->trackRect());
    }
    return QWidget::eventFilter(obj, event);
}

void SwitchButtonTrack::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush;
    if (m_switchBtn->isEnabled()) {
        brush.setColor(m_trackColor);
        painter.setOpacity(1);
    } else {
        brush.setColor(m_switchBtn->disabledColor());
        painter.setOpacity(0.6);
    }
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);

    if (Qt::Horizontal == m_switchBtn->orientation()) {
        const int h = height()/2;
        painter.save();
        painter.setOpacity(1);
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(m_switchBtn->rect());
        painter.restore();
        painter.drawRoundedRect(rect(), h/*/2*/, h/*/2*/);
    } else {
//        const int w = width()/2;
//        const QRect r(w/2, 0, w, height());
//        painter.drawRoundedRect(r.adjusted(4, 14, -4, -14), w/2, w/2);
    }
}
