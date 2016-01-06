#pragma once

class SystemTrayInterface
{
public:
    virtual void ShowAndUpdate() = 0;
    virtual void ShowMessage() = 0;
    
    static wchar_t *sAppName;
};