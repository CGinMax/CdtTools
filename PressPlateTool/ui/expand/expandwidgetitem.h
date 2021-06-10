#ifndef EXPANDWIDGETITEM_H
#define EXPANDWIDGETITEM_H

#include <QWidget>
#include <QScopedPointer>
#include <QParallelAnimationGroup>
#include <QScrollArea>

#include "expandtile.h"
#include "gathercontroller.h"

#include "../effect/shadoweffect.h"

class ExpandWidgetItemPrivate;

class ExpandWidgetItem : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ExpandWidgetItem)
public:
    explicit ExpandWidgetItem(ExpandTile* tile, GatherController* controller, QWidget *parent = nullptr);
    ~ExpandWidgetItem() override;

    void setContentWidget(QWidget* widget);

    GatherController* getController();
    YBProtocolChannel* getProtocol();

    QColor color() const;
    void setBorderColor(const QColor &color);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);

    int borderRadius() const;
    void setBorderRadius(int radius);

    bool isSelected() const;
    void setIsSelected(bool isSelected);

signals:
    void notifySelected(ExpandWidgetItem* item);
    void deleteItem();
public slots:

protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    const QScopedPointer<ExpandWidgetItemPrivate> d_ptr;
    bool m_isSelected;
    ExpandTile* m_tile;
    QWidget* m_contentArea;
    Ui::ShadowEffect* m_shadowEffect;
private:
    QScopedPointer<GatherController> m_controller;
};

#endif // EXPANDWIDGETITEM_H
