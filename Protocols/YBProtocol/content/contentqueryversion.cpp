#include "contentqueryversion.h"

ContentQueryVersion::ContentQueryVersion(const std::vector<uint8_t> &datas)
    : m_hardwareVer(std::string())
    , m_softwareVer(std::string())
    , m_productDesc(std::string())
    , m_datas(datas)
{
    m_funCode = 0x04;
    if (datas.empty()) {
        return ;
    }
    auto iter = datas.begin();
    uint8_t verLow = *iter++;
    uint8_t verHeigh = *iter++;
    m_hardwareVer = std::to_string(verHeigh) + "." + std::to_string(verLow);
    verLow = *iter++;
    verHeigh = *iter++;
    m_softwareVer = std::to_string(verHeigh) + "." + std::to_string(verLow);
    if (iter == datas.end()) {
        return;
    }

    std::string desc;
    iter++;// 4:0x00
    if (*iter++ == 0x0B) {
        desc = u8"通用款";
    }
    iter++;// 6:0x02
    while (iter != datas.end()) {
        m_productDesc += static_cast<char>(*iter++);
    }

    m_productDesc += desc;
}

std::string ContentQueryVersion::toString(bool isSend)
{
    if (isSend || m_datas.empty()) {
        return std::string();
    }

    std::string result;
    uint8_t verLow = m_datas.at(0);
    uint8_t verHeigh = m_datas.at(1);
    result += u8"硬件版本=" + std::to_string(verHeigh) + "." + std::to_string(verLow);
    verLow = m_datas.at(2);
    verHeigh = m_datas.at(3);
    result += u8", 软件版本=" + std::to_string(verHeigh) + "." + std::to_string(verLow);

    if (!m_productDesc.empty()) {
        result += u8", " + m_productDesc;
    }

    return result;
}

std::vector<uint8_t> ContentQueryVersion::toByteVector()
{
//    return {};
    return m_datas;
}


std::string ContentQueryVersion::hardwareVersion()
{
    return m_hardwareVer;
}

std::string ContentQueryVersion::softwareVersion()
{
    return m_softwareVer;
}

std::string ContentQueryVersion::productDescript()
{
    return m_productDesc;
}
