#pragma once

// Operating System
#ifdef _WIN32
#define TARGET_WINDOWS
#elif defined _DARWIN
#define TARGET_MACOSX
#elif defined _LINUX
#define TARGET_LINUX
#endif // _WIN32

class OS
{
public:
    enum Type
    {
        OS_UNKNOWN = 0x00,
        OS_WIN32 = 0x01,
        OS_MACX ,
        OS_LINUX
    };
    static Type s_Type;
};

class WindowInterface;
class GraphicContextInterface;
class SystemTrayInterface;

class AbstractFactory
{
public:
    virtual GraphicContextInterface *CreateGraphicContext() = 0;

    virtual WindowInterface *CreateOneWindow() = 0;

    virtual SystemTrayInterface * CreateSystemTray() = 0;
};


class FactoryWin32 : public AbstractFactory
{
public:
    virtual GraphicContextInterface * CreateGraphicContext() override;

    virtual WindowInterface * CreateOneWindow() override;

    virtual SystemTrayInterface * CreateSystemTray() override;
};




class System
{
public:
    static AbstractFactory *Factory();

private:
    static AbstractFactory *s_pAbstractFactory;
};