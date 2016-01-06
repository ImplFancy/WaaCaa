#include "System.h"

OS::Type OS::s_Type =
#ifdef TARGET_WINDOWS
OS_WIN32;
#elif defined (TARGET_MACOSX)
OS_MACX;
#elif defined(TARGET_LINUX)
OS_LINUX;
#else
OS_UNKNOWN;
#endif // TARGET_WINDOWS


#ifdef TARGET_WINDOWS

#include "rendering/GraphicContextDirect2d.h"
#include "window/WindowWin32.h"
#include "entry/SystemTrayWin32.h"

GraphicContextInterface * FactoryWin32::CreateGraphicContext()
{
    return new GraphicContextDirect2d();
}

WindowInterface * FactoryWin32::CreateOneWindow()
{
    return new WindowWin32();
}

SystemTrayInterface * FactoryWin32::CreateSystemTray()
{
    return new SystemTrayWin32();
}

#endif


AbstractFactory * System::Factory()
{
    if (s_pAbstractFactory == nullptr) {
#ifdef TARGET_WINDOWS
        s_pAbstractFactory = new FactoryWin32();
#endif
    }
    return s_pAbstractFactory;
}

AbstractFactory * System::s_pAbstractFactory = nullptr;