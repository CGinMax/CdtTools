#ifndef IEC104APCI_H
#define IEC104APCI_H
#include <string>
#include <vector>
#include "iec104enums.h"

class IEC104APCI
{
public:
    IEC104APCI();
    virtual ~IEC104APCI();

    virtual std::vector<uint8_t> toBytes() = 0;
    virtual std::string toString() = 0;

    const uint8_t m_headByte = 0x68;
    uint8_t m_apduLen;
};

class IEC104APCIFrameI : public IEC104APCI {
public:
    explicit IEC104APCIFrameI(uint16_t sendIndex, uint16_t receiveIndex);
    ~IEC104APCIFrameI() override;

    std::vector<uint8_t> toBytes() override;
    std::string toString() override;

    uint16_t m_sendIndex;
    uint16_t m_receiveIndex;
};

class IEC104APCIFrameU : public IEC104APCI {
public:
    explicit IEC104APCIFrameU(uint8_t ctrlByte);
    ~IEC104APCIFrameU() override;

    std::vector<uint8_t> toBytes() override;
    std::string toString() override;
    uint8_t m_ctrlByte;
};

class IEC104APCIFrameS : public IEC104APCI {
public:
    explicit IEC104APCIFrameS(uint8_t receiveIndex);
    ~IEC104APCIFrameS() override;

    std::vector<uint8_t> toBytes() override;
    std::string toString() override;
    uint8_t m_receiveIndex;
};

#endif // IEC104APCI_H
