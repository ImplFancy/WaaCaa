#pragma once

class ChartSettings
{
public:

    unsigned int GetMinWindowWidth();

    unsigned int GetMinWindowHeight();

    unsigned int GetMinZoomWidth();

    unsigned int GetMinZoomHeight();

    bool isDataAnimative();

    bool isOneDatasetMode();

    bool useEasingMode();

    void setOneDatasetMode(const bool &b);

    static ChartSettings &Get();

    ChartSettings();

private:
    bool m_isOneDatasetMode;
};