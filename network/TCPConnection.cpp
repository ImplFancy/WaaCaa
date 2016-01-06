#include "TCPConnection.h"
#include "utils/SystemEvents.h"
#include "window/WindowService.h"

TCPConnection::TCPConnection() :
    m_reqBuffer(), m_respBuffer(1 << 8)
{

}

TCPConnection::~TCPConnection()
{

}

void TCPConnection::AppendRequestBuffer(const char* receiveBuffer, unsigned int receiveBufferLength)
{
    m_reqBuffer.Append(receiveBuffer, receiveBufferLength);
}

void TCPConnection::ProcessRequestAndReply()
{
    // Clear respBuffer
    m_respBuffer.Clear();
    m_respBuffer.Append(Protocol::s_header.c_str(), Protocol::s_header.length());

    if (Protocol::IsValidMessage(m_reqBuffer)) {
        auto messageBufferOffset = Protocol::s_header.length() + 10/*version+reserve*/ + 2/*message type*/ + 4/*message length*/;
        auto ret = SystemEvents::SendOneMessage(
            Protocol::GetRequestMajorAndSubType(m_reqBuffer),
            (const char *)&(m_reqBuffer.Sub(
                messageBufferOffset,
                m_reqBuffer.Length() - messageBufferOffset)),
            (const char *)&(m_respBuffer));
    }
    else {
        // Invalid
    }

    // append tail
    m_respBuffer.Append(Protocol::s_tail.c_str(), Protocol::s_tail.length());

    // Clear reqBuffer
    m_reqBuffer.Clear();
}

const char * TCPConnection::GetResponseHeader()
{
    return m_respBuffer.Buffer();
}

const unsigned int TCPConnection::GetResponseLength()
{
    return m_respBuffer.Length();
}
