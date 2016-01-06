#include "ChartSettings.h"

#include "RenderringSettings.h"


unsigned int ChartSettings::GetMinWindowWidth()
{
    return 200u;
}

unsigned int ChartSettings::GetMinWindowHeight()
{
    return 100u;
}

unsigned int ChartSettings::GetMinZoomWidth()
{
    return 10u;
}

unsigned int ChartSettings::GetMinZoomHeight()
{
    return 10u;
}

bool ChartSettings::isDataAnimative()
{
    return true;
}

bool ChartSettings::isOneDatasetMode()
{
    return m_isOneDatasetMode;
}

bool ChartSettings::useEasingMode()
{
    if (!RenderringSettings::Get().IsImmediateMode()) return false; return true;
}

void ChartSettings::setOneDatasetMode(const bool &b)
{
    m_isOneDatasetMode = b;
}

ChartSettings & ChartSettings::Get()
{
    static ChartSettings settings;
    return settings;
}

ChartSettings::ChartSettings() :
    m_isOneDatasetMode(false)
{

}
