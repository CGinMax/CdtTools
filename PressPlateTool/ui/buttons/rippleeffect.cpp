#include "rippleeffect.h"
#include "rippleanimation.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

Ui::RippleEffect::RippleEffect(QWidget *parent)
    : QWidget(parent)
    , m_rippleStyle(RippleStyle::PositionedRipple)
    , m_clip(false)
{
    if (parent) {
        parent->installEventFilter(this);
    }
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
}

Ui::RippleEffect::~RippleEffect()
{
    for (auto& ripple : m_ripples) {
        ripple->stop();
        delete ripple;
    }
    m_ripples.clear();
}

void Ui::RippleEffect::addRipple(RippleAnimation *ripple)
{
    ripple->setOverlay(this);
    ripple->start();
    m_ripples.append(ripple);
}

void Ui::RippleEffect::addRipple(const QPoint &center, qreal radius)
{
    auto ripple = new RippleAnimation(center);
    ripple->setRadiusEndValue(radius);
    addRipple(ripple);
}

void Ui::RippleEffect::removeRipple(RippleAnimation *ripple)
{
    if (!ripple) {
        return ;
    }
    if (m_ripples.removeOne(ripple)) {
        delete ripple;
        update();
    }

}

void Ui::RippleEffect::setRippleStyle(Ui::RippleStyle style)
{
    m_rippleStyle = style;
}

void Ui::RippleEffect::setClipping(bool enabled)
{
    m_clip = enabled;
    update();
}

void Ui::RippleEffect::setClipPath(const QPainterPath &path)
{
    m_clipPath = path;
    update();
}

void Ui::RippleEffect::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_clip) {
        painter.setClipPath(m_clipPath);
    }

    for (auto& ripple : m_ripples) {
        const qreal radius = ripple->radius();
        const QPointF center = ripple->center();
        painter.setOpacity(ripple->opacity());
        painter.setBrush(ripple->brush());
        painter.drawEllipse(center, radius, radius);
    }
}

bool Ui::RippleEffect::event(QEvent *event)
{
    if (!parent()) {
        return QWidget::event(event);
    }

    switch (event->type()) {
    case QEvent::ParentChange:
        if (parent()) {
            parent()->installEventFilter(this);
            setGeometry(overlayGeometry());
        }
        break;

    case QEvent::ParentAboutToChange:
        if (parent()) {
            parent()->removeEventFilter(this);
        }
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

bool Ui::RippleEffect::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Move:
    case QEvent::Resize:
        setGeometry(overlayGeometry());
        break;
    case QEvent::MouseButtonPress: {
        if (m_rippleStyle == RippleStyle::NoRipple) {
            break;
        }
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint pos = mouseEvent->pos();
        qreal radius = this->width() > this->height() ? this->width() / 2 : this->height() / 2;


        if (m_rippleStyle == RippleStyle::CenteredRipple) {
            pos = this->rect().center();
        }

        auto ripple = new RippleAnimation(pos, this, parent());
        ripple->setOpacity(0.5);
        ripple->setRadius(radius);
        ripple->setBrush(Qt::gray);

        addRipple(ripple);
    }
        break;

    default:
        break;
    }

    return QWidget::eventFilter(watched, event);
}

QRect Ui::RippleEffect::overlayGeometry() const
{

    auto widget = qobject_cast<QWidget*>(parent());
    if (!widget) {
        return QRect();
    }
    return widget->rect();
}
