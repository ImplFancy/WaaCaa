#pragma once

class SubServiceInterface 
{
public:
    virtual ~SubServiceInterface() {};

    virtual bool Initialize() = 0;
    virtual bool DeInitialize() = 0;
};
