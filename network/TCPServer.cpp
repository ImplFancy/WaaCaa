#include "TCPServer.h"
#include "TCPConnection.h"
#include "utils/Common.h"


#ifdef TARGET_WINDOWS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif // TARGET_WINDOWS



TCPServer::TCPServer(const int port)
{
#ifdef TARGET_WINDOWS
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    m_serverSocket = Create(port, false, 5);

    m_connections.clear();
}

void TCPServer::Process()
{
    if (m_serverSocket == INVALID_SOCKET) return;

    m_bStop = false;
    auto receiveBufferLength = 1 << 10u;
    const char *receiveBuffer = (const char *)malloc(receiveBufferLength);

    for ( ; !m_bStop; ) {
        SOCKET max_fd = m_serverSocket;
        fd_set readFDs;
        struct timeval to = { 1, 0 };
        FD_ZERO(&readFDs);
        FD_SET(m_serverSocket, &readFDs);

        for (auto &conn : m_connections) {
            FD_SET(conn->m_socket, &readFDs);
            if (conn->m_socket > max_fd) {
                max_fd = conn->m_socket;
            }
        }

        // SOCKET Select
        auto ret = select(max_fd + 1, &readFDs, nullptr, nullptr, &to);
        if (ret < 0) {
            // Log::e() << "Select failed";
            // Sleep(1000);
            // m_bStop = true;
        }
        else if (ret > 0) {
            for (auto i = 0; i < m_connections.size() ; i++) {
                int sock = m_connections[i]->m_socket;
                if (!FD_ISSET(sock, &readFDs)) continue;

                bool thisConnectionError = false;

                auto recvLength = 0u;
                for (bool receiveDone = false ; !receiveDone; ) {
                    int nRead = recv(sock, (char *)(receiveBuffer), receiveBufferLength, 0);
                    if (nRead > 0) {
                        m_connections[i]->AppendRequestBuffer(receiveBuffer, nRead);
                    }
                    else if (nRead <= 0) {
                        closesocket(sock);
                        delete m_connections[i];
                        m_connections.erase(m_connections.begin() + i);
                        thisConnectionError = true;
                        receiveDone = true;
                        break;
                    }

                    if (nRead < receiveBufferLength) {
                        receiveDone = true;
                    }
                }

                if (thisConnectionError) continue;

                m_connections[i]->ProcessRequestAndReply();

                // send sth
                send(sock,
                    (char *)m_connections[i]->GetResponseHeader(),
                    m_connections[i]->GetResponseLength(),
                    0);
            }
        }

        if (FD_ISSET(m_serverSocket, &readFDs)) {
            Log::d() << "New connection detected";
            auto newconnection = new TCPConnection();
            int len = sizeof(SOCKADDR);
            newconnection->m_socket = accept(m_serverSocket, (sockaddr *)&newconnection->m_cliaddr, &len);

            if (newconnection->m_socket == INVALID_SOCKET) {
                Log::e() << "Accept of new connection failed: " << errno;
            }
            else {
                Log::i() << "Server: New connection added";
                m_connections.push_back(newconnection);
            }
        }
    }
    
    free((void*)receiveBuffer);
}

TCPServer::~TCPServer()
{
    if (m_serverSocket == INVALID_SOCKET) return;
    closesocket(m_serverSocket);

#ifdef TARGET_WINDOWS
    WSACleanup();
#endif

}

int TCPServer::Create(const int port, const bool bindLocal, const int backlog)
{
    struct sockaddr_storage addr;
    int    sock = -1;

#ifdef TARGET_WINDOWS
    int yes = 1;
    int no = 0;
#else
    unsigned int yes = 1;
    unsigned int no = 0;
#endif

    if (sock < 0 && (sock = socket(PF_INET, SOCK_STREAM, 0)) >= 0) {
        struct timeval tv = { 0, 100 };
        //setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)); // recv timeout
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

        struct sockaddr_in  *s4;
        memset(&addr, 0, sizeof(addr));
        addr.ss_family = AF_INET;
        s4 = (struct sockaddr_in *) &addr;
        s4->sin_port = htons(port);
        if (bindLocal)
            s4->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        else
            s4->sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(sock, (struct sockaddr *) (&addr), sizeof(struct sockaddr_in)) < 0) {
            closesocket(sock);
            Log::e() << "Failed to bind ipv4 serversocket";
            return INVALID_SOCKET;
        }
    }
    else if (sock < 0) {
        Log::e() << "Failed to create serversocket";
        return INVALID_SOCKET;
    }

    if (listen(sock, backlog) < 0) {
        closesocket(sock);
        Log::e() << "Failed to set listen";
        return INVALID_SOCKET;
    }

    return sock;
}
