#ifndef IEC104ENUMS_H
#define IEC104ENUMS_H

#include "../../protocols_global.h"

enum class eIEC104USType{
    STest = 0x01,                  // S测试帧
    UStartSendCmd = 0x07,          // 启动U帧
    UStartSendCmdConfirm = 0x0B,  // 启动U帧确认
    UStopSendCmd = 0x13,           // 停止U帧
    UStopSendCmdConfirm = 0x23,   // 停止U帧确认
    UTestCmd = 0x43,               // U帧测试帧
    UTestCmdConfirm = 0x83        // U帧测试确认帧
};

#endif // IEC104ENUMS_H
