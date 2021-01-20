#ifndef CLIENTPAGE_H
#define CLIENTPAGE_H

#include <QWidget>
#include "../enums.h"
#include "../common/threadpool.h"
#include "../protocol/protocolbase.h"
#include "../network/networkbase.h"
#include "../network/tcpclient.h"
#include "cdtworkwidget.h"

class ClientPage : public QWidget
{
    Q_OBJECT
public:
    explicit ClientPage(const QSharedPointer<SettingData> &ptCfg, QWidget *parent = nullptr);
    ~ClientPage();

    bool start();
    void stop();
signals:

public slots:
private:
    QSharedPointer<NetworkBase> m_tcpClient;
    QSharedPointer<SettingData> m_settingData;
    QScopedPointer<CDTWorkWidget> m_centerWidget;
};
#endif // CLIENTPAGE_H
