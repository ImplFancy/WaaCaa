#pragma once

class StringUtils
{
public:
    static StringUtils FromFloat(const float number);
    wchar_t *GetContent();

private:
    wchar_t m_content[10];
};