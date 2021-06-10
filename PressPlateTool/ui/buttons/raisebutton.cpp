#include "raisebutton.h"
#include "raisebutton_p.h"
#include "../base/painterhighqualityenabler.h"

#include <QtMath>

Ui::RaiseButtonPrivate::RaiseButtonPrivate(Ui::RaiseButton* q)
    : AbstractButtonPrivate(q)
    , m_shadowEffect(nullptr)
    , m_iconAlign(Ui::IconAlign::AlignLeft)
{
}

Ui::RaiseButtonPrivate::~RaiseButtonPrivate()
{
    delete m_shadowEffect;
}

void Ui::RaiseButtonPrivate::init()
{
    Q_Q(RaiseButton);
    m_xradius = 4;
    m_yradius = 4;
    m_shadowEffect = new ShadowEffect(q);
    q->setGraphicsEffect(m_shadowEffect);
}

Ui::RaiseButton::RaiseButton(const QString &text, QWidget *parent)
    : AbstractButton(*new RaiseButtonPrivate(this), parent)
{
    d_func()->init();
    setText(text);
}

Ui::RaiseButton::RaiseButton(const QIcon &icon, QWidget *parent)
    : AbstractButton(*new RaiseButtonPrivate(this), parent)
{
    d_func()->init();
    setIcon(icon);
    setIconSize(QSize(18, 18));
}

Ui::RaiseButton::~RaiseButton()
{

}

void Ui::RaiseButton::setEnabledShadow(bool enabled)
{
    Q_D(RaiseButton);
    d->m_shadowEffect->setEnabled(enabled);
}

bool Ui::RaiseButton::isEnabledShadow() const
{
    Q_D(const RaiseButton);
    return d->m_shadowEffect->isEnabled();
}

QSize Ui::RaiseButton::sizeHint() const
{
    ensurePolished();
    QSize textSize(fontMetrics().size(Qt::TextSingleLine, text()));
    int w = 16 + textSize.width() + 16;
    int h = textSize.height();

    if (!icon().isNull()) {
        w += iconSize().width() + 12;// 12 is icon padding
        h = qMax(h, iconSize().height());
    }

    return {w, h + 8};

}

Ui::RaiseButton::RaiseButton(Ui::RaiseButtonPrivate &d, const QString& text, QWidget *parent)
    : AbstractButton(d, parent)
{
    d_func()->init();
    setText(text);
}

Ui::RaiseButton::RaiseButton(Ui::RaiseButtonPrivate &d, const QIcon &icon, QWidget *parent)
    : AbstractButton(d, parent)
{
    d_func()->init();
    setIcon(icon);
    setIconSize(QSize(16, 16));
}

void Ui::RaiseButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    Q_D(RaiseButton);

    QPainter painter(this);
    PainterHighQualityEnabler hq(painter);
    painter.setPen(Qt::NoPen);
    painter.setBrush(isEnabled() ? backgroundColor() : disabledColor());

    painter.drawRoundedRect(rect(), xradius(), yradius());

    if (d->m_enabledBorder) {
        painter.save();
        QPen pen(d->m_borderColor);
        pen.setWidth(d->m_borderWidth);
        pen.setStyle(Qt::SolidLine);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect(), xradius(), yradius());
        painter.restore();
    }

    if (isEnabled()) {
        painter.setPen(foregroundColor());
    } else {
        painter.setPen(disabledColor());
    }

    if (icon().isNull()) {
        painter.drawText(rect(), Qt::AlignCenter, text());
        return ;
    }

    QSize textSize(fontMetrics().size(Qt::TextSingleLine, text()));


    QRect iconGeometry(QPoint(8, (height() - iconSize().height()) / 2),
                       iconSize());
    QRect textGeometry(QPoint(8, (height() - textSize.height()) / 2),
                       textSize);

    if (d->m_iconAlign == Ui::IconAlign::AlignLeft) {
        textGeometry.translate(iconSize().width() + 8, 0);
    } else {
        iconGeometry.translate(textSize.width() + 8, 0);
    }

    painter.drawText(textGeometry, Qt::AlignCenter, text());

    drawIcon(&painter, iconGeometry);
}

void Ui::RaiseButton::drawIcon(QPainter *painter, const QRect& geometry)
{
    QPixmap iconPixmap = icon().pixmap(iconSize());
    QPainter iconPainter(&iconPixmap);
    iconPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    iconPainter.fillRect(iconPixmap.rect(), isEnabled() ? foregroundColor() : disabledColor());
    painter->drawPixmap(geometry, iconPixmap);
}

