#ifndef IEC104PROTOCOL_H
#define IEC104PROTOCOL_H

#include "../iprotocols.h"

class PROTOCOLSSHARED_EXPORT IEC104Protocol : public IProtocols
{
public:
    IEC104Protocol();
    ~IEC104Protocol() override;
};

#endif // IEC104PROTOCOL_H
