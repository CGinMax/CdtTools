#ifndef YBTABLEVIEW_H
#define YBTABLEVIEW_H

#include <QListView>

class QVBoxLayout;
class ListViewItem;
class YBSensorData;

class YBTableView : public QWidget
{
    Q_OBJECT
public:
    explicit YBTableView(QWidget *parent = nullptr);
    ~YBTableView() override;

    int sensorCount() const;

    void setListItemAddr(int index, int addr);
    int getListItemAddr(int index);

    void setListItemStatus(int index, uint8_t curStatus, uint8_t configedStatus);

    void setListItemVersion(int index, const QString& hardVer, const QString& softVer, const QString& productDesc);

    void setListItemConfigedStatus(int index, uint8_t status);

    void addYBSensor(YBSensorData* data);

    void resetYBSensors(const QList<YBSensorData*>& dataList);

    void deleteYBSensor(int first, int last);
    void deleteAllYBSensors();
signals:
    void setSensorAddr(int index, int addr);
    void querySensorStatus(int index, int addr);
    void querySensorVersion(int index, int addr);
    void changeSensorStatus(int index, int addr, int status);

public slots:
    void onNotifyDelete();

protected:
    QVBoxLayout* m_layout;
    QList<ListViewItem*> m_itemList;
};

#endif // YBTABLEVIEW_H
