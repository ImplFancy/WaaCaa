#include "LinearBuffer.h"

#include <stdlib.h>
#include <string>

LinearBuffer::LinearBuffer(unsigned int initMaxLength /*= 1 << 9u*/, unsigned int maxMaxLength /*= 1 << 10u*/)
    :m_maxLength(initMaxLength),
    m_maxMaxLength(maxMaxLength),
    m_currLength(0u)
{
    m_buffer = (const char *)malloc(initMaxLength);
}

LinearBuffer::~LinearBuffer()
{
    free((void *)m_buffer);
}

bool LinearBuffer::Append(const char *srcBuffer, unsigned int srcBufferLength)
{
    if (m_currLength + srcBufferLength > m_maxMaxLength) {
        return false;
    }
    else if (m_currLength + srcBufferLength > m_maxLength) {
        m_buffer = (const char *)realloc((void *)m_buffer, m_maxMaxLength);
    }
    memcpy((void *)(m_buffer + m_currLength), srcBuffer, srcBufferLength);
    m_currLength += srcBufferLength;

    return true;
}

bool LinearBuffer::AppendOneByte(const char oneByte)
{
    Append(&oneByte, 1);

    return true;
}
