#pragma once

#include <string>

class LogInner
{
public:
    LogInner& operator<<(std::string &logToAppend);
    LogInner& operator<<(int intNumber);
    LogInner& operator<<(const char *logToAppend);
};

class Log
{
public:
    Log() {};
    ~Log() {};

    static LogInner inner;

    static LogInner& v();
    static LogInner& d();
    static LogInner& i();
    static LogInner& w();
    static LogInner& e();

    static LogInner& v(const char *format, ...);
    static LogInner& d(const char *format, ...);
    static LogInner& i(const char *format, ...);
    static LogInner& w(const char *format, ...);
    static LogInner& e(const char *format, ...);

};