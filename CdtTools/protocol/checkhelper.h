#ifndef CHECKHELPER_H
#define CHECKHELPER_H
#include "sysenumtype.h"

class CheckHelper
{
public:
    CheckHelper();

    static unsigned short CheckCRC16(unsigned char buff[], int offset, int len);

    static unsigned char CheckCRC8(unsigned char buff[], int offset, int len);

    static unsigned short CheckSum16(unsigned char buff[], int offset, int len);

    static unsigned char CheckSum8(unsigned char buff[], int offset, int len);

    static int GetByteSize(eValType valType);

    static unsigned char CRC8Array[256];

};

#endif // CHECKHELPER_H
