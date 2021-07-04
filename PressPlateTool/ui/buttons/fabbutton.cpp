#include "fabbutton.h"
#include "fabbutton_p.h"
#include <QPainterPath>
#include <QSizePolicy>
#include <QEvent>

Ui::FabButtonPrivate::FabButtonPrivate(FabButton* q)
    : IconButtonPrivate (q)
    , m_diameter(56)
    , m_corner(Qt::BottomRightCorner)
    , m_xoffset(8)
    , m_yoffset(8)
{}

Ui::FabButtonPrivate::~FabButtonPrivate()
{}

void Ui::FabButtonPrivate::init()
{
    Q_Q(FabButton);
    m_shadowEffect->setEnabled(true);
    m_enabledHover = true;

    q->setGeometry(q->fabGeometry());
    q->setCheckable(true);
    q->setChecked(false);
    q->setXRadius(q->width())->setYRadius(q->height())->setIconSize(QSize(24, 24));
    q->resize(q->diameter(), q->diameter());

    QObject::connect(q, &QAbstractButton::toggled, q, [=](bool checked){
        if (!checked) {
            q->setOpacity(0.5);
        }
    });
}

Ui::FabButton::FabButton(const QIcon& icon, QWidget *parent)
    : IconButton(*new FabButtonPrivate(this), icon, parent)
{
    if (parent != nullptr) {
        parent->installEventFilter(this);
    }
    d_func()->init();
}


Ui::FabButton::~FabButton()
{

}

Ui::FabButton *Ui::FabButton::setDiameter(int diameter)
{
    Q_D(FabButton);
    if (d->m_diameter == diameter) {
        return this;
    }

    d->m_diameter = diameter;
    update();
    return this;
}

int Ui::FabButton::diameter() const
{
    Q_D(const FabButton);
    return d->m_diameter;
}


Qt::Corner Ui::FabButton::corner() const
{
    Q_D(const FabButton);
    return d->m_corner;
}

Ui::FabButton *Ui::FabButton::setCorner(Qt::Corner corner)
{
    Q_D(FabButton);
    if (d->m_corner == corner) {
        return this;
    }
    d->m_corner = corner;
    setGeometry(fabGeometry());
    update();
    return this;
}

int Ui::FabButton::offsetX() const
{
    Q_D(const FabButton);
    return d->m_xoffset;
}

Ui::FabButton *Ui::FabButton::setOffsetX(int x)
{
    Q_D(FabButton);
    d->m_xoffset = x;
    setGeometry(fabGeometry());
    update();
    return this;
}

int Ui::FabButton::offsetY() const
{
    Q_D(const FabButton);
    return d->m_yoffset;
}

Ui::FabButton *Ui::FabButton::setOffsetY(int y)
{
    Q_D(FabButton);
    d->m_yoffset = y;
    setGeometry(fabGeometry());
    update();

    return this;
}

Ui::FabButton *Ui::FabButton::setOffset(int x, int y)
{
    Q_D(FabButton);
    d->m_xoffset = x;
    d->m_yoffset = y;
    setGeometry(fabGeometry());
    update();
    return this;
}

Ui::FabButton *Ui::FabButton::setOffset(const QSize &size)
{
    setOffset(size.width(), size.height());
    return this;
}

QSize Ui::FabButton::offset() const
{
    return {offsetX(), offsetY()};
}

bool Ui::FabButton::event(QEvent *event)
{
    if (!parent()) {
        return IconButton::event(event);
    }
    switch (event->type())
    {
    case QEvent::ParentChange:
    {
        parent()->installEventFilter(this);
        setGeometry(fabGeometry());
        break;
    }
    case QEvent::ParentAboutToChange:
    {
        parent()->removeEventFilter(this);
        break;
    }
    default:
        break;
    }
    return IconButton::event(event);
}

bool Ui::FabButton::eventFilter(QObject *obj, QEvent *event)
{
    const QEvent::Type type = event->type();
    if (type == QEvent::Move || type == QEvent::Resize) {
        setGeometry(fabGeometry());
        updateRippleClipPath();
    }

    return IconButton::eventFilter(obj, event);
}

QRect Ui::FabButton::fabGeometry() const
{
    QWidget *parent = parentWidget();
    if (!parent) {
        return QRect();
    }

    const int s = diameter();
    // Qt::BottomRightCorner
    QRect rect(parent->width() - (offsetX() + s), parent->height() - (offsetY() + s), s, s);
    switch (corner())
    {
    case Qt::TopLeftCorner:
        rect = QRect(offsetX(), offsetY(), s, s);
        break;
    case Qt::TopRightCorner:
        rect = QRect(parent->width() - (offsetX() + s), offsetY(), s, s);
        break;
    case Qt::BottomLeftCorner:
        rect = QRect(offsetX(), parent->height() - (offsetY() + s), s, s);
        break;
    default:
        break;
    }
    return rect;

}

void Ui::FabButton::updateRippleClipPath()
{
    QPainterPath path;
    QRect square = QRect(0, 0, diameter(), diameter());
    square.moveCenter(rect().center());
    path.addEllipse(square);
    setClipPath(path);
}
