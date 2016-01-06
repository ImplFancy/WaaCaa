#include "Log.h"

#include <iostream>

LogInner& LogInner::operator<<(std::string &logToAppend)
{
    std::cout << logToAppend;
    return *this;
}

LogInner& LogInner::operator<<(const char *logToAppend)
{
    std::cout << logToAppend;
    return *this;
}

LogInner& LogInner::operator<<(int intNumber)
{
    std::cout << intNumber;
    return *this;
}

LogInner Log::inner;

LogInner& Log::v()
{
    return inner;
}

LogInner& Log::v(const char *format, ...)
{
    return inner;
}

LogInner& Log::d()
{
    return inner;
}

LogInner& Log::d(const char *format, ...)
{
    return inner;
}

LogInner& Log::i()
{
    return inner;
}

LogInner& Log::i(const char *format, ...)
{
    return inner;
}

LogInner& Log::w()
{
    return inner;
}

LogInner& Log::w(const char *format, ...)
{
    return inner;
}

LogInner& Log::e()
{
    return inner;
}

LogInner& Log::e(const char *format, ...)
{
    return inner;
}
