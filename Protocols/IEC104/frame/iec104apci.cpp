#include "iec104apci.h"

IEC104APCI::IEC104APCI()
{

}

IEC104APCI::~IEC104APCI()
{

}

IEC104APCIFrameI::IEC104APCIFrameI(uint16_t sendIndex, uint16_t receiveIndex)
    : m_sendIndex(sendIndex)
    , m_receiveIndex(receiveIndex)
{

}

IEC104APCIFrameI::~IEC104APCIFrameI()
{

}

std::vector<uint8_t> IEC104APCIFrameI::toBytes()
{
    std::vector<uint8_t> result;
    result.push_back(m_sendIndex & 0xFE);
    result.push_back((m_sendIndex >> 8) & 0xFF);
    result.push_back(m_receiveIndex & 0xFE);
    result.push_back((m_receiveIndex >> 8) & 0xFF);
    return result;
}

std::string IEC104APCIFrameI::toString()
{

}

IEC104APCIFrameU::IEC104APCIFrameU(uint8_t ctrlByte)
    : m_ctrlByte(ctrlByte)
{

}

IEC104APCIFrameU::~IEC104APCIFrameU()
{

}

std::vector<uint8_t> IEC104APCIFrameU::toBytes()
{
    std::vector<uint8_t> result(4, 0);
    result[0] = m_ctrlByte;
    return result;
}

std::string IEC104APCIFrameU::toString()
{

}

IEC104APCIFrameS::IEC104APCIFrameS(uint8_t receiveIndex)
    : m_receiveIndex(receiveIndex)
{

}

IEC104APCIFrameS::~IEC104APCIFrameS()
{

}

std::vector<uint8_t> IEC104APCIFrameS::toBytes()
{
    std::vector<uint8_t> result;

    result.push_back(0x01);
    result.push_back(0x00);
    result.push_back(m_receiveIndex & 0xFE);
    result.push_back((m_receiveIndex >> 8) & 0xFF);

    return result;
}

std::string IEC104APCIFrameS::toString()
{

}
