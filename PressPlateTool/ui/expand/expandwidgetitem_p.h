#ifndef EXPANDWIDGETITEM_P_H
#define EXPANDWIDGETITEM_P_H

#include <Qt>
#include <QColor>

class ExpandWidgetItem;

class ExpandWidgetItemPrivate
{
public:
    Q_DECLARE_PUBLIC(ExpandWidgetItem)
    Q_DISABLE_COPY(ExpandWidgetItemPrivate)
    explicit ExpandWidgetItemPrivate(ExpandWidgetItem* q);
    ~ExpandWidgetItemPrivate();

    void init();

    ExpandWidgetItem*   const q_ptr;
    QColor              m_borderColor;
    QColor              m_backgroundColor;
    bool                m_isExpanded;
    int                 m_borderRadius;
    int                 m_animationDuration;
};

#endif // EXPANDWIDGETITEM_P_H
