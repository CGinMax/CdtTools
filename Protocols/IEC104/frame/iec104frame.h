#ifndef IEC104FRAME_H
#define IEC104FRAME_H

#include <vector>
#include <list>
#include <memory>
#include <iterator>
#include "iec104enums.h"
#include "iec104apci.h"

class IEC104VSQ
{
public:
    explicit IEC104VSQ(uint8_t vsqByte = 0);

    IEC104VSQ(const IEC104VSQ& other) = default;
    IEC104VSQ& operator=(const IEC104VSQ& other) = default;

    uint8_t toByte();

    std::string toString();

    uint8_t m_num;
    bool m_sq;
};

struct IEC104Cause
{
    explicit IEC104Cause(uint8_t reason = 0, uint8_t srcAddr = 0);
    uint8_t m_T;
    uint8_t m_POrN;
    uint8_t m_reason;
    uint8_t m_srcAddr;

    std::vector<uint8_t> toByte();
    std::string toString();
};

class IEC104Quality
{
public:
    explicit IEC104Quality(uint8_t quality = 0);
    virtual ~IEC104Quality();

    virtual uint8_t toByte();
    virtual std::string toString() = 0;

    bool m_valid;
    uint8_t m_data;
};

class IEC104Time
{
public:
    IEC104Time();
    virtual ~IEC104Time();

    virtual std::vector<uint8_t> toBytes();
    virtual std::string toString();

    uint16_t m_msec;
    bool m_valid;
    uint8_t m_minute;
};

class IEC104Info
{
public:
    IEC104Info();
    virtual ~IEC104Info();

    virtual std::vector<uint8_t> toBytes() = 0;
    virtual std::string toString() = 0;

    int m_addr;

    std::shared_ptr<IEC104Quality> m_quality;
};

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

    IEC104Cause m_cause;

    uint16_t m_pubAddr;

};

class IEC104SingleYXQuality : public IEC104Quality
{
public:
    using IEC104Quality::IEC104Quality;
    std::string toString() override;
};

class IEC104TwiceYXQuality : public IEC104Quality
{
public:
    using IEC104Quality::IEC104Quality;
    std::string toString() override;
};

class IEC104YCQuality : public IEC104Quality
{
public:
    using IEC104Quality::IEC104Quality;
    std::string toString() override;
};

class IEC104LongTime : public IEC104Time
{
public:
    IEC104LongTime();

    std::vector<uint8_t> toBytes() override;

    std::string toString() override;

    uint8_t m_hour;

    uint8_t m_week;

    uint8_t m_day;

    uint8_t m_mounth;

    uint8_t m_year;
};

class IEC104YCInfo : public IEC104Info
{
public:
    IEC104YCInfo();
    ~IEC104YCInfo() override;

    std::vector<uint8_t> toBytes() override;
    std::string toString() override;

    double m_value;
};

class IEC104YXInfo : public IEC104Info
{
public:
    IEC104YXInfo();
    ~IEC104YXInfo() override;

    std::vector<uint8_t> toBytes() override;
    std::string toString() override;

    bool m_value;
};


#endif // IEC104FRAME_H
