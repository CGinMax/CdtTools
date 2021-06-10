#include "outlinebutton.h"
#include "outlinebutton_p.h"
#include "../base/theme.h"

Ui::OutlineButtonPrivate::OutlineButtonPrivate(Ui::OutlineButton* q)
    : RaiseButtonPrivate(q)
{

}

Ui::OutlineButtonPrivate::~OutlineButtonPrivate()
{}

void Ui::OutlineButtonPrivate::init()
{
    m_shadowEffect->setEnabled(false);
    m_backgroundColor = QColor(Qt::transparent);
    m_foregroundColor = Theme::instance()->primaryColor();
    m_borderColor = m_foregroundColor;
    m_enabledBorder = true;

}

Ui::OutlineButton::OutlineButton(const QString &text, QWidget *parent)
     : RaiseButton (*new OutlineButtonPrivate(this), text, parent)
{
    d_func()->init();
}

Ui::OutlineButton::~OutlineButton()
{

}
