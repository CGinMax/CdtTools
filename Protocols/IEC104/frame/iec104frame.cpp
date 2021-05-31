#include "iec104frame.h"

IEC104Frame::IEC104Frame()
{

}

IEC104Frame::~IEC104Frame()
{

}

IEC104VSQ::IEC104VSQ(uint8_t vsqByte)
{
    m_sq = (vsqByte >> 7) & 0x01;
    m_num = vsqByte & 0x7F;
}

uint8_t IEC104VSQ::toByte()
{
    uint8_t result = m_sq;
    result = static_cast<uint8_t>(result << 7);
    result |= m_num;
    return result;
}

std::string IEC104VSQ::toString()
{}
