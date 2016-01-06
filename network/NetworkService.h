#pragma once

#include "utils/Common.h"
#include "utils/SubServiceInterface.h"

#include <thread>
#include <memory>

class TCPServer;
class NetworkService : public SubServiceInterface
{
private:
    NetworkService();
    ~NetworkService();

public:
    static NetworkService &Instance() {
        static NetworkService g_NetworkService;
        return g_NetworkService;
    }

    virtual bool Initialize() override final;
    virtual bool DeInitialize() override final;

private:
    std::shared_ptr<TCPServer> m_pTCPServer;
    std::shared_ptr<std::thread> m_workThread;
};

