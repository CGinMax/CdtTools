#ifndef CDTPROTOCOL_H
#define CDTPROTOCOL_H

#include <QQueue>
#include <QSharedPointer>
#include <QMutex>
#include <QTimer>
#include "../cdtframe.h"
#include "../protocolbase.h"
#include "../../network/networkbase.h"
#include "../../enums.h"

class CDTProtocol : public ProtocolBase
{
    Q_OBJECT
public:
    CDTProtocol(const QSharedPointer<NetworkBase>& network, const QSharedPointer<SettingData>& settingData);
    ~CDTProtocol() override;

     void initStrategy() override;

    void run() override;

    // 解析接收数据接口
    void parseRecvData();
    // 解析数据，封装成帧
    eCDTParseResult parseToFrame();

    virtual void processFrame();

    void send(const CDTFrame& frame);

    void sendAllDi();
    void sendAllAi();
    void sendVirtualYX();

    // 遥信
    void yxResponse(QList<InfoFieldEntity> &infoFieldList);

    // 遥测
    void ycResponse(QList<InfoFieldEntity> &infoFieldList);

    void vyxResponse(QList<InfoFieldEntity> &infoFieldList);

    void ykSelect(uint8_t operCode, uint8_t ptNo);

    void ykSelectBack(uint8_t operCode, uint8_t ptNo);

    void yKExecute(uint8_t operCode, uint8_t ptNo);

    void yKCancel(uint8_t operCode, uint8_t ptNo);

    CDTFrame buildYXFrame(uint8_t startFuncode, QList<DiData*>& ptList);


    inline bool isRunYK() const {
        return m_isRunYK;
    }

    inline void setRunYK(bool running) {
        m_isRunYK = running;
    }

    inline PtCfg* getPtCfg() {
        return m_settingData->m_ptCfg.data();
    }


    void uploadDi();

    void uploadAi();

signals:
public slots:
    void onReverseYx(int ptId, bool allow) override;
    void onReadyRead() override;
    void onDisconnected();

protected slots:
    void onTimeout() override;

protected:
    double bcdToValue(int bcdValue);

    uint32_t valueToBCD(double value);


    QMutex m_mtx;
    QByteArray m_recvBuffer;
    QQueue<CDTFrame> m_frameQueue;
    bool m_isRunYK;
    uint m_yxCounter;
    uint m_ycCounter;
};

#endif // CDTPROTOCOL_H