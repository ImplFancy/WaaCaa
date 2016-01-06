#include "RenderringSettings.h"



bool RenderringSettings::IsImmediateMode()
{
    return true;
}

RenderringSettings & RenderringSettings::Get()
{
    static RenderringSettings settings;
    return settings;
}
