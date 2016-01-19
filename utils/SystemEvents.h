#pragma once

class SystemEvents
{
public:
    enum PredefEvents {
        MESSAGE_TRAY = 0x0500 + 0x01,
    };

    enum RetType {
        OK = 0x10,
        FAILED
    };

    static unsigned int ToSystemEvent(unsigned short userEvent);

    static unsigned short ToUserEvent(unsigned int sysEvent);


    static RetType SendOneMessage(unsigned int message, const char *pParam, const char *lParam);
};
