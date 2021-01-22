#include "cdtinteracte.h"

CDTInteracte::CDTInteracte(const QSharedPointer<NetworkBase> &network, const QSharedPointer<SettingData> &settingData)
    : CDTProtocol (network, settingData)
{

}

void CDTInteracte::ykResponse(CDTFrame &frame)
{
    InfoFieldEntity firstInfoData = frame.infoFields.front();
    uint8_t funCode = firstInfoData.funCode;
    uint8_t ctrlCode = firstInfoData.dataArray[0];
    // true合,false开
    bool status = ctrlCode == m_settingData->m_ptCfg->m_ykClose;
    int ykAddr = firstInfoData.dataArray[2];
    ykAddr |= firstInfoData.dataArray[3] << 8;

    if (ykAddr > m_settingData->m_ptCfg->m_globalDiList->size())
        return ;

    if (funCode == m_settingData->m_ptCfg->m_ykReqCode) {
        if (!m_isRunYK) {// 接收到遥控选择
            // 直接返回校核
            ykSelectBack(ctrlCode, static_cast<uint8_t>(ykAddr));
            // 进入遥控执行状态
            m_isRunYK = true;
        }
        else {// 接收到遥控执行
            // 退出遥控执行状态
            m_isRunYK = false;
        }
    }
    else if (funCode == m_settingData->m_ptCfg->m_ykAckCode) {
        ykExecute(ctrlCode, static_cast<uint8_t>(ykAddr));
    }
    else if (funCode == m_settingData->m_ptCfg->m_ykExeCode) {
        bool success = firstInfoData.dataArray[1] == m_settingData->m_ptCfg->m_ykUnlock;
        if (success){
            auto di = m_settingData->m_ptCfg->findDiById(ykAddr);
            QString msg;
            if (di) {
                di->setValue(status);
                msg = QStringLiteral("变位成功");
            }
            else {
                msg = QStringLiteral("变位失败，遥信点Id=%1错误").arg(ykAddr);
            }
            emit sendYKMsg(msg);
        }
        else {
            emit sendYKMsg(QStringLiteral("禁止点%1遥控变位操作").arg(ykAddr));
        }
        m_isRunYK = false;

    }

}

void CDTInteracte::ykSelect(uint8_t ctrlCode, uint8_t ptId)
{
    CDTFrame frame = interactYKFrame(m_settingData->m_ptCfg->m_ykReqType, m_settingData->m_ptCfg->m_ykReqCode, ctrlCode, 0xFF, ptId);
    send(frame);
    showMessageBuffer(eMsgType::eMsgSend, QStringLiteral("遥控选择"), frame.toAllByteArray());
    emit sendYKMsg(QStringLiteral("发送点%1的%2操作遥控选择指令").arg(ptId).arg(ctrlCode == eControlLockCode::CloseValidLock ? QStringLiteral("合"):QStringLiteral("分")));
}

void CDTInteracte::ykSelectBack(uint8_t ctrlCode, uint8_t ptId)
{
    CDTFrame frame = interactYKFrame(m_settingData->m_ptCfg->m_ykAckType, m_settingData->m_ptCfg->m_ykAckCode, ctrlCode, 0xFF, ptId);
    send(frame);
    showMessageBuffer(eMsgType::eMsgSend, QStringLiteral("遥控选择回传"), frame.toAllByteArray());
    emit sendYKMsg(QStringLiteral("发送点%1的%2操作遥控选择回传指令").arg(ptId).arg(ctrlCode == eControlLockCode::CloseValidLock ? QStringLiteral("合"):QStringLiteral("分")));
}

void CDTInteracte::ykExecute(uint8_t ctrlCode, uint8_t ptId)
{
    CDTFrame frame = interactYKFrame(m_settingData->m_ptCfg->m_ykReqType, m_settingData->m_ptCfg->m_ykReqCode, ctrlCode, 0xFF, ptId);
    send(frame);
    showMessageBuffer(eMsgType::eMsgSend, QStringLiteral("遥控执行"), frame.toAllByteArray());
    emit sendYKMsg(QStringLiteral("发送点%1的%2操作遥控执行指令").arg(ptId).arg(ctrlCode == eControlLockCode::CloseValidLock ? QStringLiteral("合"):QStringLiteral("分")));
}

void CDTInteracte::ykExecuteBack(uint8_t ctrlCode, uint8_t ptId, bool success)
{
    // AA解锁，55闭锁
    uint8_t valid = success ? m_settingData->m_ptCfg->m_ykUnlock : m_settingData->m_ptCfg->m_ykLock;
    CDTFrame frame = interactYKFrame(m_settingData->m_ptCfg->m_ykAckType, m_settingData->m_ptCfg->m_ykExeCode, ctrlCode, valid, ptId);
    send(frame);
    showMessageBuffer(eMsgType::eMsgSend, QStringLiteral("遥控执行"), frame.toAllByteArray());
    emit sendYKMsg(QStringLiteral("发送点%1的%2遥控变位指令").arg(ptId).arg(success ? QStringLiteral("允许"):QStringLiteral("禁止")));
}

CDTFrame CDTInteracte::interactYKFrame(uint8_t frameType, uint8_t funCode, uint8_t operCode, uint8_t vaild, int ptId)
{
    CDTFrame frame;

    InfoFieldEntity entity;
    entity.fillData(funCode, operCode, vaild, static_cast<uint8_t>(ptId & 0xFF), static_cast<uint8_t>(ptId >> 8 & 0xFF));
    frame.infoFields.append(entity);

    frame.frameControl.fillData(m_settingData->m_ptCfg->m_controlType, frameType, frame.infoFields.count(), 0, 0);
    return frame;
}

void CDTInteracte::startYK(int ptId, bool offon)
{
    if (ptId > m_settingData->m_ptCfg->m_globalDiList->size())
        return ;

    uint8_t code = offon ? m_settingData->m_ptCfg->m_ykClose : m_settingData->m_ptCfg->m_ykOpen;

    ykSelect(code, static_cast<uint8_t>(ptId));
    m_isRunYK = true;
}


