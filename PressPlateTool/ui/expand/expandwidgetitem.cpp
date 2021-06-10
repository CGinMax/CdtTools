#include "expandwidgetitem.h"
#include "expandwidgetitem_p.h"
#include <QEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScopedPointer>
#include <QPropertyAnimation>
#include <QDebug>

ExpandWidgetItemPrivate::ExpandWidgetItemPrivate(ExpandWidgetItem* q)
    : q_ptr(q)
    , m_borderColor(QColor(Qt::blue))
    , m_backgroundColor(QColor(0xFAFBFB))
    , m_borderRadius(0)
{

}

ExpandWidgetItemPrivate::~ExpandWidgetItemPrivate()
{

}

void ExpandWidgetItemPrivate::init()
{
}

ExpandWidgetItem::ExpandWidgetItem(ExpandTile *tile, GatherController *controller, QWidget *parent)
    : QWidget(parent)
    , d_ptr(new ExpandWidgetItemPrivate(this))
    , m_isSelected(false)
    , m_tile(tile)
    , m_contentArea(new QWidget(this))
    , m_shadowEffect(new Ui::ShadowEffect(this))
    , m_controller(controller)
{
    d_ptr->init();
    m_tile->setParent(this);
    setGraphicsEffect(m_shadowEffect);
    m_shadowEffect->setEnabled(false);
    setMaximumWidth(280);

    auto mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_tile, 0, 0);
    mainLayout->addWidget(m_contentArea, 1, 0);

    auto contentLayout = new QVBoxLayout(m_contentArea);
    contentLayout->setSpacing(0);

    connect(tile, &ExpandTile::deleteItem, this, &ExpandWidgetItem::deleteItem);

}

ExpandWidgetItem::~ExpandWidgetItem()
{

}

void ExpandWidgetItem::setContentWidget(QWidget *widget)
{
    // 重置content里的控件
    auto nowWidget = m_contentArea->layout()->takeAt(0);
    if (nowWidget) {
        m_contentArea->layout()->removeItem(nowWidget);
        delete nowWidget;
    }
    m_contentArea->layout()->addWidget(widget);

}

GatherController *ExpandWidgetItem::getController()
{
    return m_controller.data();
}

YBProtocolChannel *ExpandWidgetItem::getProtocol()
{
    return m_controller->protocol();
}

QColor ExpandWidgetItem::color() const
{
    Q_D(const ExpandWidgetItem);
    return d->m_borderColor;
}

void ExpandWidgetItem::setBorderColor(const QColor &borderColor)
{
    Q_D(ExpandWidgetItem);
    d->m_borderColor = borderColor;
    update();
}

QColor ExpandWidgetItem::backgroundColor() const
{
    Q_D(const ExpandWidgetItem);
    return d->m_backgroundColor;
}

void ExpandWidgetItem::setBackgroundColor(const QColor &color)
{
    Q_D(ExpandWidgetItem);
    d->m_backgroundColor = color;
    update();
}

int ExpandWidgetItem::borderRadius() const
{
    Q_D(const ExpandWidgetItem);
    return d->m_borderRadius;
}

void ExpandWidgetItem::setBorderRadius(int radius)
{
    Q_D(ExpandWidgetItem);
    d->m_borderRadius = radius;
    update();
}

bool ExpandWidgetItem::isSelected() const
{
    return m_isSelected;
}

void ExpandWidgetItem::setIsSelected(bool isSelected)
{
    m_isSelected = isSelected;
    m_shadowEffect->setEnabled(isSelected);
    update();
}

bool ExpandWidgetItem::event(QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        emit notifySelected(this);
    }
    return QWidget::event(event);
}

void ExpandWidgetItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    Q_D(ExpandWidgetItem);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(d->m_backgroundColor);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), d->m_borderRadius, d->m_borderRadius);
    QWidget::paintEvent(event);
}
