#include "nrudpprotocol.h"
#include "nrudpwfstrategy.h"
#include "nrudpmintorstrategy.h"
#include <algorithm>

NrUdpProtocol::NrUdpProtocol(const QSharedPointer<NetworkBase>& network, const QSharedPointer<SettingData>& settingData)
    : ProtocolBase(network, settingData)
    , m_sendCounter(0)
    , m_isRunYK(false)
{
    connect(m_network.data(), &NetworkBase::disconnected, this, &NrUdpProtocol::onDisconnected, Qt::BlockingQueuedConnection);

}

NrUdpProtocol::~NrUdpProtocol()
{

}

void NrUdpProtocol::initStrategy()
{
    if (m_settingData->m_stationType == eStationType::WF) {
        m_strategy = new NrUdpWFStrategy(this);
    } else {
        m_strategy = new NrUdpMintorStrategy(this);
    }
    connect(this, &ProtocolBase::sendYk, m_strategy, &StrategyBase::sendYK);
}

void NrUdpProtocol::run()
{
    m_strategy->uploadTiming();
}

void NrUdpProtocol::parseRecvData()
{
    // buffer中有数据开始解析
    while (m_recvBuffer.size() > 0)
    {
        if (parseToFrame() != NrUdpFrame::CompleteFrame) {
            break;
        }
    }
}

NrUdpFrame::eNrParseResult NrUdpProtocol::parseToFrame()
{
    NrUdpFrame frame;
    int index = 0;
    QString error;
    NrUdpFrame::eNrParseResult result = frame.parseBytesToFrame(m_recvBuffer, index, error);
    if (result == NrUdpFrame::CompleteFrame) {
        m_recvBuffer.remove(0, index);
        m_frameQueue.enqueue(frame);
    }
    else if (!error.isEmpty()) {
        qInfo("%s", qPrintable(error));
    }
    return result;
}

void NrUdpProtocol::processFrame()
{
    while (!m_frameQueue.isEmpty()) {
        NrUdpFrame frame = m_frameQueue.dequeue();
        switch (frame.cmdCode) {
        case NrUdp::Yx:
            showMessageBuffer(eMsgType::eMsgRecv, QStringLiteral("接收到遥测帧，正在处理..."), frame.allFrameBytes);
            yxResponse(frame.infoData);
            break;
        case NrUdp::Yc | NrUdp::VirtualYx:
        {
            if (m_settingData->m_stationType == eStationType::WF) {
                showMessageBuffer(eMsgType::eMsgRecv, QStringLiteral("接收到遥测帧，正在处理..."), frame.allFrameBytes);
                ycResponse(frame.infoData);
            }
            else {
                showMessageBuffer(eMsgType::eMsgRecv, QStringLiteral("接收到虚遥信，正在处理..."), frame.allFrameBytes);
                vyxResponse(frame.infoData);
            }
        }
            break;
        case NrUdp::RequestYk:
        case NrUdp::AllowYk:
        case NrUdp::ForbidYk:
            showMessageBuffer(eMsgType::eMsgRecv, QStringLiteral("接收到遥控帧，正在处理..."), frame.allFrameBytes);
            m_strategy->ykResponse(frame.infoData);
            break;

        case NrUdp::DeviceStatus:
            break;
        }
    }
}

void NrUdpProtocol::send(const NrUdpFrame &frame)
{

    if (frame.allFrameBytes.size() > 0) {
        emit write(frame.allFrameBytes);
    }
}

void NrUdpProtocol::yxResponse(QByteArray &infoData)
{
    int nSeq = 0; // 遥信点号
    if (!m_settingData->m_ptCfg->m_globalDiList->isEmpty()) {
        nSeq = m_settingData->m_ptCfg->m_globalDiList->first()->io();
    }
    for (auto &byte : infoData)
    {
        for (int i = 0; i < 8; i++) {
            uint8_t yxValue = 0;// 遥信值1bit
            yxValue = (byte >> i) & 0x01;

            auto di = m_settingData->m_ptCfg->findDiById(nSeq);
            if (di) {
                di->setValue(yxValue > 0);
            }
            else {
                qInfo("未找到遥信点，点号=%d", nSeq);
            }

            nSeq++;
        }
    }
}

void NrUdpProtocol::ycResponse(QByteArray &infoData)
{
    int nSeq = infoData.at(1);
    nSeq = (nSeq << 8) | infoData.at(0);
    for (int i = 2; i < infoData.size(); i += 4) {
        int ycValue = infoData.at(i + 3);
        ycValue = ycValue << 8 | infoData.at(i + 2);
        ycValue = ycValue << 8 | infoData.at(i + 1);
        ycValue = ycValue << 8 | infoData.at(i);

        auto ai = m_settingData->m_ptCfg->findAiById(nSeq);
        if (ai) {
            ai->setValue(ycValue);
        }
        else {
            qInfo("未找到遥测点，点号=%d", nSeq);
        }
        nSeq++;
    }

}

void NrUdpProtocol::ykRequest(int ptId, bool offon)
{
    NrUdpFrame frame;
    frame.length = 0x000C;
    frame.cmdCode = 0x02;
    frame.infoData.append(static_cast<uint8_t>(ptId) & 0xFF);
    frame.infoData.append(static_cast<uint8_t>(ptId >> 8) & 0xFF);
    frame.infoData.append(offon ? 0x01 : 0x00);
    frame.setDataBuf();
    send(frame);

    emit sendYKMsg(QStringLiteral("发送点%1的遥控%2请求指令").arg(ptId).arg(offon ? QStringLiteral("合") : QStringLiteral("分")));
}

void NrUdpProtocol::vyxResponse(QByteArray &infoData)
{
    int nSeq = 0; // 虚遥信点号
    if (!m_settingData->m_ptCfg->m_globalVDiList->isEmpty()) {
        nSeq = m_settingData->m_ptCfg->m_globalVDiList->first()->io();
    }
    for (auto &byte : infoData)
    {
        for (int i = 0; i < 8; i++) {
            uint8_t yxValue = 0;// 虚遥信值1bit
            yxValue = (byte >> i) & 0x01;

            auto vdi = m_settingData->m_ptCfg->findVDiById(nSeq);
            if (vdi) {
                vdi->setValue(yxValue > 0);
            }
            else {
                qInfo("未找到虚遥信点，点号=%d", nSeq);
            }

            nSeq++;
        }
    }
}

void NrUdpProtocol::sendAllDi()
{
    if (m_settingData->m_ptCfg->m_globalDiList->isEmpty()) {
        return ;
    }
    auto frame = buildYXFrame(0x01, *m_settingData->m_ptCfg->m_globalDiList);
    showMessageBuffer(eMsgType::eMsgSend, QStringLiteral("发送全遥信"), frame.allFrameBytes);
    send(frame);
}

void NrUdpProtocol::sendAllAi()
{
    NrUdpFrame frame;
    frame.cmdCode = 0x05;
    int startIo = m_settingData->m_ptCfg->m_globalAiList->first()->io();
    frame.infoData.append(static_cast<uint8_t>(startIo) & 0xFF);
    frame.infoData.append(static_cast<uint8_t>(startIo >> 8) & 0xFF);
    for (auto& ai : *m_settingData->m_ptCfg->m_globalAiList) {
        int ycValue = ai->value();
        for (int i = 0; i < 4; i++) {
            frame.infoData.append(static_cast<uint8_t>(ycValue >> (i * 8)) & 0xFF);
        }
    }
    frame.length = 9 + frame.infoData.size();
    frame.setDataBuf();
    showMessageBuffer(eMsgType::eMsgSend, QStringLiteral("发送全遥测"), frame.allFrameBytes);
    send(frame);
}

void NrUdpProtocol::sendVDi()
{
    if (m_settingData->m_ptCfg->m_globalDiList->isEmpty()) {
        return ;
    }
    auto frame = buildYXFrame(0x05, *m_settingData->m_ptCfg->m_globalVDiList);
    showMessageBuffer(eMsgType::eMsgSend, QStringLiteral("发送全遥信"), frame.allFrameBytes);
    send(frame);
}

void NrUdpProtocol::uploadDiAi()
{
    if (m_sendCounter > 10000) {
        sendAllDi();
        sendAllAi();
        m_sendCounter = 0;
    }
}

void NrUdpProtocol::uploadVDi()
{
    if (m_sendCounter > 10000) {
        sendVDi();
        m_sendCounter = 0;
    }
}

NrUdpFrame NrUdpProtocol::buildYXFrame(uint8_t cmdCode, QList<DiData *> &ptList)
{
    NrUdpFrame frame;
    frame.cmdCode = cmdCode;

    QList<uint8_t> combineByteList;
    uchar val = 0;
    int index = 0;
    for (const auto& di: ptList) {
        if (index % 8 == 0 && index != 0) {
             combineByteList.append(val);
            val = 0;
            index = 0;
        }
        bool diVal = di->value();
        if (diVal) {
            val |= (1 << index % 8);
        }
        index++;
    }
    if (index % 8 != 0) {
        combineByteList.append(val);
    }
    std::for_each(combineByteList.begin(), combineByteList.end(), [&frame](uint8_t byte){
        frame.infoData.append(byte);
    });
//    int offset = combineByteList.count() % 4;
//    if (offset != 0) {
//        for (int i = 0; i < 4 - offset; i++) {
//            combineByteList.append(0);
//        }
//    }

    frame.length = static_cast<uint16_t>(9 + frame.infoData.size());
    frame.setDataBuf();
    return frame;
}


void NrUdpProtocol::onReverseYx(int ptId, bool allow)
{
    if (allow) {
        auto di = m_settingData->m_ptCfg->findDiById(ptId);
        auto changeMsg = di->value() ? QStringLiteral("合->分") : QStringLiteral("分->合");
        di->setValue(!di->value());
        sendAllDi();

        qInfo("点%d遥控变位完成", ptId);
        emit sendYKMsg(QStringLiteral("允许点%1发生遥控变位，%2").arg(ptId).arg(changeMsg));
    }
    else {
        emit sendYKMsg(QStringLiteral("禁止点%1发生遥控变位").arg(ptId));
    }
    m_isRunYK = false;
}

void NrUdpProtocol::onReadyRead()
{
    auto bytes = m_network->readAll();

    if (!bytes.isEmpty()) {
        m_recvBuffer.append(bytes);
        //处理数据
        parseRecvData();
    }
    // 处理帧
    processFrame();
}

void NrUdpProtocol::onDisconnected()
{
    stop();
}

void NrUdpProtocol::onTimeout()
{
    m_sendCounter += 100;
    ProtocolBase::onTimeout();
}