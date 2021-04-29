#ifndef EXPANDWIDGET_H
#define EXPANDWIDGET_H

#include <QWidget>
#include <QScrollArea>

class QVBoxLayout;
class ExpandWidgetItem;

namespace Ui {
class ExpandWidget;
}

class ExpandWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExpandWidget(QWidget *parent = nullptr);
    ~ExpandWidget() override;
    void addExpandItem(ExpandWidgetItem* item);
    void insertExpandItem(int index, ExpandWidgetItem* item);

    int indexOf(ExpandWidgetItem* item);

signals:

public slots:

protected:
    Ui::ExpandWidget* ui;
    QList<ExpandWidgetItem*> m_itemList;
};

#endif // EXPANDWIDGET_H
