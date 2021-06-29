#include "snackbar.h"
#include "snackbar_p.h"
#include "../base/theme.h"
#include "../effect/shadoweffect.h"
#include "../buttons/flatbutton.h"
#include "../base/painterhighqualityenabler.h"
#include "../animation/slideanimation.h"
#include <QSize>
#include <QHBoxLayout>
#include <QEvent>
#include <QFont>
#include <QFontMetrics>


#include <QDebug>

Ui::SnackBarPrivate::SnackBarPrivate(Ui::SnackBar *q)
    : q_ptr(q)
    , m_shadowEffect(nullptr)
    , m_backgroundColor(QColor(0x58554D))
    , m_textColor(Ui::Theme::instance()->color(Ui::Color::white))
    , m_icon(QIcon())
    , m_iconSize(24, 24)
    , m_radius(5)
    , m_clickDismiss(true)
    , m_textFlag(Qt::TextSingleLine | Qt::AlignCenter)
    , m_slideAnimation(nullptr)
    , m_animationDuration(300)
    , m_showDuration(2000)
{

}

Ui::SnackBarPrivate::~SnackBarPrivate()
{
    delete m_shadowEffect;
    delete m_actionBtn;
}

void Ui::SnackBarPrivate::init(const QIcon &icon, const QString &text)
{
    Q_Q(SnackBar);
    m_shadowEffect = new Ui::ShadowEffect(q);
    q->setGraphicsEffect(m_shadowEffect);
    m_slideAnimation = new Ui::SlideAnimation(q->pos(), q);
    m_slideAnimation->setStartValue(QPoint(0, 0));
    m_slideAnimation->setEndValue(QPoint(0, -50));
    m_slideAnimation->setTargetObject(q);
    m_slideAnimation->setPropertyName(QByteArray("distance"));
    m_slideAnimation->setDuration(300);
    m_slideAnimation->setDirection(QAbstractAnimation::Backward);
    QObject::connect(m_slideAnimation, &QAbstractAnimation::finished, q, &SnackBar::onSlideFinished);

    m_icon = icon;
    m_text = text;
    m_actionBtn = new FlatButton(QString(""), q);
    auto font = m_actionBtn->font();
    font.setBold(true);
    font.setPixelSize(16);
    m_actionBtn->setFont(font);
    m_actionBtn->setForegroundColor(Ui::Theme::instance()->color("primary"));
    m_actionBtn->hide();
    m_actionCallback = [](){};
    QObject::connect(m_actionBtn, &QAbstractButton::pressed, q, &SnackBar::onActionPressed);
}

Ui::SnackBar::SnackBar(const QString &text, QWidget *parent)
    : QWidget (parent)
    , d_ptr(new SnackBarPrivate(this))
{
    d_func()->init(QIcon(), text);
    prepareSlidePos();
}

Ui::SnackBar::SnackBar(const QIcon &icon, const QString &text, QWidget *parent)
    : QWidget (parent)
    , d_ptr(new SnackBarPrivate(this))
{
    d_func()->init(icon, text);
    prepareSlidePos();
}

Ui::SnackBar::~SnackBar()
{

}

Ui::SnackBar *Ui::SnackBar::setText(const QString &text)
{
    Q_D(SnackBar);
    if (!d->m_text.compare(text)) {
        return this;
    }
    d->m_text = text;
    update();
    return this;
}

QString Ui::SnackBar::text() const
{
    Q_D(const SnackBar);
    return d->m_text;
}

Ui::SnackBar *Ui::SnackBar::setTextFlag(int flag, bool enabled)
{
    Q_D(SnackBar);
//    if (!enabled) {
//        d->m_textFlag &= flag;
//        update();
//        return this;
//    }

//    if (!(d->m_textFlag & flag)) {
//        d->m_textFlag |= flag;
//        update();
//    }
//    return this;
    // TODO: refactoring
    if (d->m_textFlag == flag) {
        return this;
    }
    d->m_textFlag = flag;
    return this;
}

int Ui::SnackBar::textFlags() const
{
    Q_D(const SnackBar);
    return d->m_textFlag;
}

Ui::SnackBar *Ui::SnackBar::setBackgroundColor(const QColor &color)
{
    Q_D(SnackBar);
    if (d->m_backgroundColor == color) {
        return this;
    }

    d->m_backgroundColor = color;
    update();
    return this;
}

QColor Ui::SnackBar::backgroundColor() const
{
    Q_D(const SnackBar);
    return d->m_backgroundColor;
}

Ui::SnackBar *Ui::SnackBar::setTextColor(const QColor &color)
{
    Q_D(SnackBar);
    if (d->m_textColor == color) {
        return this;
    }

    d->m_textColor = color;
    update();
    return this;

}

QColor Ui::SnackBar::textColor() const
{
    Q_D(const SnackBar);
    return d->m_textColor;
}

Ui::SnackBar *Ui::SnackBar::setIcon(const QIcon &icon)
{
    Q_D(SnackBar);
    d->m_icon = icon;
    update();
    //TODO: resize
    return this;
}

QIcon Ui::SnackBar::icon() const
{
    Q_D(const SnackBar);
    return d->m_icon;
}

Ui::SnackBar *Ui::SnackBar::setIconSize(const QSize &size)
{
    Q_D(SnackBar);
    if (d->m_iconSize == size) {
        return this;
    }
    d->m_iconSize = size;
    update();
    // TODO: resize
    return this;
}

QSize Ui::SnackBar::iconSize() const
{
    Q_D(const SnackBar);
    return d->m_iconSize;
}

Ui::SnackBar *Ui::SnackBar::setRadius(int radius)
{
    Q_D(SnackBar);
    if (d->m_radius == radius) {
        return this;
    }
    d->m_radius = radius;
    update();
    return this;
}

int Ui::SnackBar::radius() const
{
    Q_D(const SnackBar);
    return d->m_radius;
}

Ui::SnackBar *Ui::SnackBar::setClickToDismiss(bool enabled)
{
    Q_D(SnackBar);
    d->m_clickDismiss = enabled;


    return this;
}

bool Ui::SnackBar::clickToDismiss() const
{
    Q_D(const SnackBar);
    return d->m_clickDismiss;
}

Ui::SnackBar *Ui::SnackBar::setDistance(const QPoint &pos)
{
    move(pos);
    return this;
}


Ui::SnackBar *Ui::SnackBar::setAction(const QString &text, Fn<void ()> &&callback)
{
    Q_D(SnackBar);
    d->m_actionBtn->setText(text);
    d->m_actionCallback = std::forward<Fn<void()>>(callback);
    text.isEmpty() ? d->m_actionBtn->hide() : d->m_actionBtn->show();
    return this;
}

Ui::SnackBar *Ui::SnackBar::setEnabledShadow(bool enabled)
{
    Q_D(SnackBar);
    if (d->m_shadowEffect->isEnabled() == enabled) {
        return this;
    }

    d->m_shadowEffect->setEnabled(enabled);
    return this;
}

bool Ui::SnackBar::isShadowEnabled() const
{
    Q_D(const SnackBar);
    return d->m_shadowEffect->isEnabled();
}

Ui::SnackBar *Ui::SnackBar::setAnimationDuration(int duration)
{
    Q_D(SnackBar);
    if (d->m_animationDuration == duration) {
        return this;
    }
    d->m_animationDuration = duration;
    return this;
}

int Ui::SnackBar::animationDuration() const
{
    Q_D(const SnackBar);
    return d->m_animationDuration;
}

Ui::SnackBar *Ui::SnackBar::setShowDuration(int duration)
{
    Q_D(SnackBar);
    d->m_showDuration = duration;
    return this;
}

int Ui::SnackBar::showDuration() const
{
    Q_D(const SnackBar);
    return d->m_showDuration;
}

QSize Ui::SnackBar::sizeHint() const
{
    Q_D(const SnackBar);
    QSize textSize(fontMetrics().size(textFlags(), text()));

    int h = textSize.height() + 16;
    int w = 8 + 8 + textSize.width();
    if (!icon().isNull()) {
        w += iconSize().width();
    }
    if (!d->m_actionBtn->text().isEmpty()) {
        w += d->m_actionBtn->width();
    }
    return {w,h};
}

void Ui::SnackBar::setEnabledAnimation(bool enabled)
{
    Q_UNUSED(enabled)
}

void Ui::SnackBar::showBar(const QPoint& startValue, const QPoint& endValue)
{
    Q_D(SnackBar);
    if (d->m_slideAnimation->direction() == QAbstractAnimation::Forward) {
        return ;
    }

    prepareSlidePos();

    d->m_slideAnimation->stop();
    QVariant sv = startValue.isNull() ? d->m_slideAnimation->startValue() : startValue;
    QVariant ev = endValue.isNull() ? d->m_slideAnimation->endValue() : endValue;
    d->m_slideAnimation->setStartValue(sv);
    d->m_slideAnimation->setEndValue(ev);
    d->m_slideAnimation->setDirection(QAbstractAnimation::Forward);
    d->m_slideAnimation->setDuration(animationDuration());
    d->m_slideAnimation->start();
}

void Ui::SnackBar::hideBar()
{
    Q_D(SnackBar);
    if (d->m_slideAnimation->direction() == QAbstractAnimation::Backward) {
        return ;
    }

    d->m_slideAnimation->stop();
    d->m_slideAnimation->setDirection(QAbstractAnimation::Backward);
    d->m_slideAnimation->setDuration(animationDuration());
    d->m_slideAnimation->start();
}

bool Ui::SnackBar::isFinished()
{
    Q_D(SnackBar);
    return d->m_slideAnimation->state() == QAbstractAnimation::Running;
}

void Ui::SnackBar::stopWait()
{
    Q_D(SnackBar);
    d->m_timer.stop();
}

void Ui::SnackBar::setSlidePos(const QPoint &pos)
{

    Q_D(SnackBar);
    d->m_slideAnimation->setSourcePos(pos);
}

void Ui::SnackBar::showSnackBar(QWidget *widget, const QString &text, const QIcon &icon, const QString &actionText, Fn<void ()> &&callback)
{
    if (widget == nullptr) {
        qDebug("No parent!");
        return;
    }
    auto topWidget = widget->window();
    auto bar = new Ui::SnackBar(icon, text, topWidget);
    if (!actionText.isEmpty()) {
        bar->setAction(actionText, std::forward<Fn<void()>>(callback));
    }
    connect(bar, &Ui::SnackBar::showFinished, topWidget, [=]{
        delete bar;
    });
    bar->showBar();

}

void Ui::SnackBar::onActionPressed()
{
    Q_D(SnackBar);
    hideBar();
    d->m_actionCallback();
}

void Ui::SnackBar::onSlideFinished()
{
    Q_D(SnackBar);
    if (d->m_slideAnimation->direction() == QAbstractAnimation::Forward) {
        d->m_timer.start(d->m_showDuration, this);
    } else {
        emit showFinished();
    }
}

bool Ui::SnackBar::event(QEvent *event)
{
    if (event->type() == QEvent::Resize || event->type() == QEvent::Move) {
        update();
        updateActionGeometry();
    } else if (event->type() == QEvent::MouseButtonPress) {
        if (clickToDismiss()) {
            stopWait();
            hideBar();
        }
    } else if (event->type() == QEvent::Timer) {
        hideBar();
    }
    return QWidget::event(event);
}

void Ui::SnackBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    PainterHighQualityEnabler hm(painter);

    auto backgroundRect = rect();
    backgroundRect.setSize(sizeHint());

    painter.setPen(textColor());
    painter.setBrush(Qt::NoBrush);

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundColor());
    painter.drawRoundedRect(backgroundRect, radius(), radius());
    painter.restore();

    QSize textSize(fontMetrics().size(textFlags(), text()));

    QRect iconGeometry(QPoint(8, (height() - iconSize().height()) / 2),
                       iconSize());
    QRect textGeometry(QPoint(8, (height() - textSize.height()) / 2),
                       textSize);
    if (!icon().isNull()) {
        textGeometry.translate(iconSize().width() + 8, 0);
    }

    painter.drawText(textGeometry, textFlags(), text());

    if (!icon().isNull()) {
        QPixmap iconPixmap = icon().pixmap(iconSize());
        QPainter iconPainter(&iconPixmap);
        iconPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        iconPainter.fillRect(iconPixmap.rect(), textColor());
        painter.drawPixmap(iconGeometry, iconPixmap);
    }

}

void Ui::SnackBar::prepareSlidePos()
{
    QWidget* widget = parentWidget();
    int topWidth = 1000;
    int topHeight = 1000;
    if (widget != nullptr) {
        topWidth = widget->width();
        topHeight = widget->height();
    } else {
        qDebug("please set parent!");
        return ;
    }

    this->setVisible(true);
    this->resize(sizeHint());
    this->raise();
    QPoint startPos = {(topWidth - this->width()) / 2, topHeight + 2};
    move(startPos.x(), startPos.y());
    setSlidePos(startPos);
}

void Ui::SnackBar::updateGeometry()
{

}

void Ui::SnackBar::updateActionGeometry()
{
    Q_D(SnackBar);
    int y = (height() - d->m_actionBtn->height()) / 2;

    int iconWidth = 0;
    if (!icon().isNull()) {
        iconWidth = iconSize().width();
    }
    int fontWidth = QSize(fontMetrics().size(textFlags(), text())).width();
    int x = 8 + iconWidth + fontWidth;
    d->m_actionBtn->move(x, y);
}
