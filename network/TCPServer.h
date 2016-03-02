#pragma once

#include <vector>

class TCPConnection;

class TCPServer 
{
public:
    TCPServer(const int port, /*out*/ bool &isFailed);
    ~TCPServer();

    virtual void Process() ;

    bool m_bStop;

private:
    int Create(const int port, const bool bindLocal, const int backlog);

    std::vector<TCPConnection *> m_connections;
    int m_serverSocket;

};

