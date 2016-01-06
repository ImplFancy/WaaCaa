#include "StringUtils.h"


#include <strsafe.h>

StringUtils StringUtils::FromFloat(const float number)
{
    StringUtils out;
    swprintf(out.m_content, 10, L"%8f", number);
    return out;
}

wchar_t * StringUtils::GetContent()
{
    return m_content;
}
