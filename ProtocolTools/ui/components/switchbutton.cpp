#include "switchbutton.h"
#include "switchbutton_p.h"
#include <QApplication>
#include <QPainter>
#include <QEvent>
#include <QSize>
#include <QStateMachine>
#include <QSignalTransition>
#include <QPropertyAnimation>
#include "switchbutton_internal.h"

#include <QDebug>

SwitchButtonPrivate::SwitchButtonPrivate(SwitchButton* q)
    : q_ptr(q)
{}

SwitchButtonPrivate::~SwitchButtonPrivate()
{}

void SwitchButtonPrivate::init()
{
    Q_Q(SwitchButton);
    q->setCheckable(true);
    q->setChecked(false);
    q->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    track = new SwitchButtonTrack(q);
    thumb = new SwitchButtonThumb(q);
    trackMargins = QMargins(2, 5, 2, 5);
    thumbMargins = QMargins(6, 3, thumb->height() / 2 - 2, 3);

    stateMachine = new QStateMachine(q);
    offState = new QState();
    onState = new QState();
    orientation = Qt::Horizontal;
    disabledColor = QColor(Qt::gray);
    activeColor = QColor("#39E893");
    inactiveColor = QColor("#FF1015");
    trackColor = QColor("#FF1015");
    penWidth = 4;

    stateMachine->addState(offState);
    stateMachine->addState(onState);
    stateMachine->setInitialState(offState);

    offState->assignProperty(thumb, "shift", 0);
    onState->assignProperty(thumb, "shift", 1);

    offState->assignProperty(thumb, "thumbWidth", thumb->rect().height() / 2);
    onState->assignProperty(thumb, "thumbWidth", 0);

    QSignalTransition* transition;
    QPropertyAnimation* animation;

    // on animation transition
    transition = new QSignalTransition(q, &QAbstractButton::toggled);
    transition->setTargetState(onState);
    offState->addTransition(transition);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("shift");
    animation->setTargetObject(thumb);
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("trackColor");
    animation->setTargetObject(track);
    animation->setDuration(150);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("thumbColor");
    animation->setTargetObject(thumb);
    animation->setDuration(150);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("thumbWidth");
    animation->setTargetObject(thumb);
    animation->setDuration(200);
    transition->addAnimation(animation);

    // off
    transition = new QSignalTransition(q, &QAbstractButton::toggled);
    transition->setTargetState(offState);
    onState->addTransition(transition);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("shift");
    animation->setTargetObject(thumb);
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("trackColor");
    animation->setTargetObject(track);
    animation->setDuration(150);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("thumbColor");
    animation->setTargetObject(thumb);
    animation->setDuration(150);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(q);
    animation->setPropertyName("thumbWidth");
    animation->setTargetObject(thumb);
    animation->setDuration(200);
    transition->addAnimation(animation);


    setupProperties();
    stateMachine->start();
}

void SwitchButtonPrivate::setupProperties()
{
    Q_Q(SwitchButton);

    if (q->isEnabled()) {
        const qreal shift = thumb->shift();
        if (qFuzzyCompare(shift, 1)) {
            thumb->setThumbColor(q->activeColor());
            track->setTrackColor(q->activeColor().lighter(110));
        } else if (qFuzzyCompare(1+shift, 1)) {
            thumb->setThumbColor(q->inactiveColor());
            track->setTrackColor(q->trackColor());
        }
    }

    offState->assignProperty(track, "trackColor", q->trackColor().lighter(110));
    onState->assignProperty(track, "trackColor", q->activeColor().lighter(110));

    offState->assignProperty(thumb, "thumbColor", q->inactiveColor());
    onState->assignProperty(thumb, "thumbColor", q->activeColor());

    q->update();
}

SwitchButton::SwitchButton(QWidget *parent)
    : QAbstractButton(parent)
    , d_ptr(new SwitchButtonPrivate(this))
{
    d_ptr->init();
}

SwitchButton::~SwitchButton()
{

}

void SwitchButton::setDisabledColor(const QColor &color)
{
    Q_D(SwitchButton);

    d->disabledColor = color;
    d->setupProperties();
}

QColor SwitchButton::disabledColor() const
{
    Q_D(const SwitchButton);
    return d->disabledColor;
}

void SwitchButton::setActiveColor(const QColor &color)
{
    Q_D(SwitchButton);
    d->activeColor = color;
    d->setupProperties();
}

QColor SwitchButton::activeColor() const
{
    Q_D(const SwitchButton);
    return d->activeColor;
}

void SwitchButton::setInactiveColor(const QColor &color)
{
    Q_D(SwitchButton);
    d->inactiveColor = color;
    d->setupProperties();
}

QColor SwitchButton::inactiveColor() const
{
    Q_D(const SwitchButton);
    return d->inactiveColor;
}

void SwitchButton::setTrackColor(const QColor &color)
{
    Q_D(SwitchButton);
    d->trackColor = color;
    d->setupProperties();
}

QColor SwitchButton::trackColor() const
{
    Q_D(const SwitchButton);
    return d->trackColor;

}

void SwitchButton::setOrientation(Qt::Orientation orientation)
{
    Q_D(SwitchButton);
    if (d->orientation == orientation) {
        return;
    }

    d->orientation = orientation;
    updateGeometry();
}

Qt::Orientation SwitchButton::orientation() const
{
    Q_D(const SwitchButton);
    return d->orientation;
}

void SwitchButton::setThumbMargins(const QMargins &margins)
{
    Q_D(SwitchButton);
    d->thumbMargins = margins;
    d->setupProperties();
}

QMargins SwitchButton::thumbMargins() const
{
    Q_D(const SwitchButton);
    return d->thumbMargins;
}

void SwitchButton::setTrackMargins(const QMargins &margins)
{
    Q_D(SwitchButton);
    d->trackMargins = margins;
    d->setupProperties();
}

QMargins SwitchButton::trackMargins() const
{
    Q_D(const SwitchButton);
    return d->trackMargins;
}

void SwitchButton::setPenWidth(int penWidth)
{
    Q_D(SwitchButton);
    d->penWidth = penWidth;
    update();
}

int SwitchButton::penWidth() const
{
    Q_D(const SwitchButton);
    return d->penWidth;
}

QRect SwitchButton::thumbRect()
{
    Q_D(SwitchButton);
    auto rect = trackRect();

//    int min = qMin(rect.height(), rect.width()) - 1;
//    rect.setSize(QSize(min, min));
//    rect.setX(rect.x() - 1);
//    rect.setY(rect.y() - 1);
    return rect.adjusted(1, 1, -1 , -1);
//    return trackRect().adjusted(d->thumbMargins.left(), d->thumbMargins.top()
//                                , -d->thumbMargins.right(), -d->thumbMargins.bottom());
}

QRect SwitchButton::trackRect()
{
    Q_D(SwitchButton);
    int min = qMin(height(), width()) - 8;
    d->trackRect.setSize(QSize(2 * min, min));
    d->trackRect.setX((this->width() - d->trackRect.width()) / 2);
    d->trackRect.setY((this->height() - d->trackRect.height()) / 2);

    return d->trackRect;
//    return rect().adjusted(d->trackMargins.left(), d->trackMargins.top()
//                           , -d->trackMargins.right(), -d->trackMargins.bottom());
}

void SwitchButton::setOffStateWidth(int offWidth)
{
    Q_D(SwitchButton);
    if (qFuzzyIsNull(d->thumb->thumbWidth())) {
        return;
    }
    d->offState->assignProperty(d->thumb, "thumbWidth", offWidth);
}

QSize SwitchButton::sizeHint() const
{
    Q_D(const SwitchButton);

    return Qt::Horizontal == d->orientation
            ? QSize(64, 48)
            : QSize(48, 64);
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);

    painter.drawRect(rect());
}

