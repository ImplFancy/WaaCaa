#pragma once

class SystemEvents
{
public:
    enum PredefEvents {
        MESSAGE_TRAY = 0xFF + 0x01,
    };

    enum RetType {
        OK = 0x10,
        FAILED
    };

    static unsigned int ToSystemEvent(unsigned int userEvent);

    static unsigned int ToUserEvent(unsigned int sysEvent);


    static RetType SendOneMessage(unsigned int message, const char *pParam, const char *lParam);
};
