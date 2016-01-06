#pragma once

struct LinearBuffer
{
    struct Static
    {
        Static(const char *start, unsigned int length)
            :m_buffer(start), m_length(length)
        {
        }
        Static(const Static& b) { m_buffer = b.m_buffer; m_length = b.m_length; }
        const char * Buffer() const { return m_buffer; }
        unsigned int Length() const { return m_length; }
        const char *m_buffer;
        unsigned int m_length;
    };



    LinearBuffer(
        unsigned int initMaxLength = 1 << 9u,
        unsigned int maxMaxLength = 1 << 25u);

    ~LinearBuffer();

    LinearBuffer(const LinearBuffer& b) = delete;

    Static Sub(unsigned int offset) const {
        return Static(m_buffer + offset, m_currLength - offset);
    }

    Static Sub(unsigned int offset, unsigned int length) const {
        // assert (offset + length < m_currlength);
        return Static(m_buffer + offset, length);
    }

    bool Append(const char *srcBuffer, unsigned int srcBufferLength);

    bool AppendOneByte(const char oneByte);

    const char * Buffer() const { return m_buffer; }

    const unsigned int Length()  const { return m_currLength; }

    void Clear() { m_currLength = 0; }


private:
    const char *m_buffer;

    unsigned int m_currLength, m_maxLength, m_maxMaxLength;

};