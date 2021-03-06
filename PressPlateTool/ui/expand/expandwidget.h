#ifndef EXPANDWIDGET_H
#define EXPANDWIDGET_H

#include <QWidget>
#include <QScrollArea>

class GatherData;
class GatherController;
class QVBoxLayout;
class ExpandWidgetItem;
class PortParam;
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
    void removeExpandItem(ExpandWidgetItem* item);
    void removeAllItem();
    int indexOf(ExpandWidgetItem* item);

    QList<ExpandWidgetItem*> itemList() const;

    void addExpandItems(const QList<QSharedPointer<GatherData>>& dataList);

    static ExpandWidgetItem* createExpandWidget(const PortParam& portParam, int index, int radius = 8);
signals:
    void itemChanged(GatherController* controller);

public slots:
    void onNotifySelected(ExpandWidgetItem* item);
    void onDeleteItem();

protected:
    Ui::ExpandWidget* ui;
    QList<ExpandWidgetItem*> m_itemList;
    ExpandWidgetItem* m_checkItem;
};

#endif // EXPANDWIDGET_H
