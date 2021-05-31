#ifndef IEC104FRAME_H
#define IEC104FRAME_H

#include <vector>
#include <list>
#include <memory>
#include <iterator>
#include "iec104enums.h"
#include "iec104apci.h"

class IEC104VSQ;

class PROTOCOLSSHARED_EXPORT IEC104Frame
{
public:
    IEC104Frame();
    ~IEC104Frame();

    const uint8_t m_headByte = 0x68;
    uint8_t m_apduLen;

    std::shared_ptr<IEC104APCI> m_apci;

    uint8_t m_typeId;// type indentify

    IEC104VSQ m_vsq;

    uint16_t m_reason;

    uint16_t m_pubAddr;



};

class IEC104VSQ
{
public:
    explicit IEC104VSQ(uint8_t vsqByte);

    IEC104VSQ(const IEC104VSQ& other) = default;
    IEC104VSQ& operator=(const IEC104VSQ& other) = default;

    uint8_t toByte();

    std::string toString();

    uint8_t m_num;
    uint8_t m_sq;
};

struct IEC104Cause
{

    uint8_t m_reason;
    std::vector<uint8_t> toByte();
    std::string toString();
};

#endif // IEC104FRAME_H
