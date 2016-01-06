#pragma once

#include "utils/Common.h"
#include "utils/LinearBuffer.h"
#include "model/Protocol.h"

#ifdef TARGET_WINDOWS
#include <WinSock2.h>
#endif // TARGET_WINDOWS


class TCPConnection
{
private:
    friend class TCPServer;

    TCPConnection();;
    
    ~TCPConnection();;

    //Close();
    void AppendRequestBuffer(const char* receiveBuffer, unsigned int receiveBufferLength);

    // pRespBuffer
    void ProcessRequestAndReply();

    const char * GetResponseHeader();
    const unsigned int GetResponseLength();

    int m_socket;
    struct sockaddr_storage   m_cliaddr;

    LinearBuffer m_reqBuffer, m_respBuffer;
};