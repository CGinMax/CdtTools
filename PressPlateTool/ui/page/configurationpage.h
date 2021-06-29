#ifndef CONFIGURATIONPAGE_H
#define CONFIGURATIONPAGE_H

#include <QWidget>

namespace Ui {
class ConfigurationPage;
}

class PortParam;
class ConfigurationPage : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigurationPage(QWidget *parent = nullptr);
    ~ConfigurationPage() override;

    int gatherTime() const;
    void setGatherTime(int time);

    int sensorTime() const;
    void setSensorTime(int time);

    void setPortParam(const PortParam& param);
    PortParam portParam() const;

signals:
    void notifySave();
private:
    Ui::ConfigurationPage *ui;
};

#endif // CONFIGURATIONPAGE_H
