#include "NetworkService.h"
#include "TCPServer.h"
#include "settings/NetworkSettings.h"
#include "utils/Common.h"

#include <thread>


NetworkService::NetworkService()
{
}


NetworkService::~NetworkService()
{
}

bool NetworkService::Initialize()
{
    m_pTCPServer.reset(new TCPServer(NetworkSettings::Get().GetLocalPort()));
    m_workThread.reset(new std::thread(std::bind(&TCPServer::Process, m_pTCPServer)));

    return true;
}

bool NetworkService::DeInitialize()
{
    if (m_workThread->joinable()) {
        m_pTCPServer->m_bStop = true;
        m_workThread->join();

        // TODO: considering m_workThread->join() blocked for a long time
    }

    m_pTCPServer.reset();
    m_workThread.reset();

    return true;
}