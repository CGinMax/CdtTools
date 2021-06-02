#include "iec104frame.h"
#include "../../convert.h"
IEC104Frame::IEC104Frame()
{

}

IEC104Frame::~IEC104Frame()
{

}

IEC104VSQ::IEC104VSQ(uint8_t vsqByte)
{
    m_sq = ((vsqByte >> 7) & 0x01) == 0x01;
    m_num = vsqByte & 0x7F;
}

uint8_t IEC104VSQ::toByte()
{
    uint8_t result = m_sq ? 0x01 : 0x00;
    result = static_cast<uint8_t>(result << 7);
    result |= m_num;
    return result;
}

std::string IEC104VSQ::toString()
{
    std::string result = u8"信息字个数=";
    result += std::to_string(m_num);
    result += ", ";
    result += m_sq ? u8"连续" : u8"不连续";
    return result;
}

IEC104Cause::IEC104Cause(uint8_t reason, uint8_t srcAddr)
{
    m_T = (reason >> 7) & 0x01;
    m_POrN = (reason >> 6) & 0x01;
    m_reason = reason & 0x3F;
    m_srcAddr = srcAddr;
}

std::vector<uint8_t> IEC104Cause::toByte()
{
    std::vector<uint8_t> result;
    uint8_t primaryByte = 0;
    primaryByte |= (m_T << 7);
    primaryByte |= (m_POrN << 6);
    primaryByte |= (m_reason & 0x3F);
    result.push_back(primaryByte);
    result.push_back(m_srcAddr);
    return result;
}

std::string IEC104Cause::toString()
{
    std::string result = u8"传送原因=";
    result += Convert::num2HexString(m_reason);
    return result;
}

IEC104Quality::IEC104Quality(uint8_t quality)
{
    m_valid = ((quality >> 7) & 0x01) == 0x01;
    m_data = quality & 0x7F;
}

IEC104Quality::~IEC104Quality()
{}

uint8_t IEC104Quality::toByte()
{
    uint8_t result = m_valid ? 0x00 : 0x80;
    result |= (m_data & 0x7F);
    return result;
}

std::string IEC104SingleYXQuality::toString()
{
    std::string result = u8"点品质=";
    result += m_valid ? u8"有效" : u8"无效";
    result += m_data == 0x01 ? u8"合" : u8"分";
    return result;
}

std::string IEC104TwiceYXQuality::toString()
{}

std::string IEC104YCQuality::toString()
{
    std::string result = u8"点品质=";
    result += m_valid ? u8"有效" : u8"无效";
    result += m_data == 0x01 ? u8"溢出" : u8"未溢出";
    return result;
}

IEC104Time::IEC104Time()
{}

IEC104Time::~IEC104Time()
{}

std::vector<uint8_t> IEC104Time::toBytes()
{
    std::vector<uint8_t> result;
    result.push_back(m_msec & 0xFF);
    result.push_back((m_msec >> 8) & 0xFF);
    uint8_t byte = m_valid ? 0x00 : 0x80;
    byte |= m_minute;
    result.push_back(byte);
    return result;
}

std::string IEC104Time::toString()
{
    std::string result = u8"短时标, 分钟=";
    result += std::to_string(m_minute);
    result += u8", 秒=";
    result += std::to_string(m_msec / 1000);
    result += u8", 毫秒=";
    result += std::to_string(m_msec % 1000);
    result += m_valid ? u8", 有效" : u8", 无效";
    return result;
}

IEC104LongTime::IEC104LongTime()
{}

std::vector<uint8_t> IEC104LongTime::toBytes()
{
    auto result = IEC104Time::toBytes();
    result.push_back(m_hour & 0x1F);
    uint8_t weekday = 0;
    weekday |= (m_week << 5);
    weekday |= (m_day & 0x1F);
    result.push_back(weekday);
    result.push_back(m_mounth & 0x0F);
    result.push_back(m_year & 0x7F);
    return result;
}

std::string IEC104LongTime::toString()
{
    std::string result = u8"长时标, ";
    result += std::to_string(m_year) + "-" + std::to_string(m_mounth) + "-" + std::to_string(m_day) + " ";
    result += std::to_string(m_hour) + ":" + std::to_string(m_minute) + ":" + std::to_string(m_msec / 1000) + "." + std::to_string(m_msec % 1000) + " ";
    result += u8"星期" + std::to_string(m_week);
    return result;
}

IEC104Info::IEC104Info()
{}

IEC104Info::~IEC104Info()
{}

IEC104YCInfo::IEC104YCInfo()
{}

IEC104YCInfo::~IEC104YCInfo()
{}

std::vector<uint8_t> IEC104YCInfo::toBytes()
{}

std::string IEC104YCInfo::toString()
{}

IEC104YXInfo::IEC104YXInfo()
{}

IEC104YXInfo::~IEC104YXInfo()
{}

std::vector<uint8_t> IEC104YXInfo::toBytes()
{}

std::string IEC104YXInfo::toString()
{}
