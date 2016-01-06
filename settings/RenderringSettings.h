#pragma once

class RenderringSettings
{
public:

    bool IsImmediateMode();

    static RenderringSettings &Get();
};