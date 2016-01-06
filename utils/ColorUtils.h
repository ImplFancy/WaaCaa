#pragma once

#include <vector>

struct WaaColour
{
    explicit WaaColour(unsigned int rgb) :
        m_rgb(rgb), m_alpha(1.f)
    {

    }

    explicit WaaColour(unsigned int rgb, float alphaValue) :
        m_rgb(rgb), m_alpha(alphaValue)
    { 
        
    };

    WaaColour SetAlpha(float alpha) {
        return WaaColour(m_rgb, alpha);
    }

    unsigned int m_rgb;
    float m_alpha;
};


class ColorTheme
{
public:

    static ColorTheme &GetDefaultTheme()
    {
        static ColorTheme flatUI(WaaColour(0xecf0f1), WaaColour(0x2c3e50), WaaColour(0xc0392b), WaaColour(0xbdc3c7));
        if (!flatUI.m_inited) {
            flatUI.m_DataContentList.push_back(WaaColour(0x2980b9));
            flatUI.m_DataContentList.push_back(WaaColour(0x27ae60));
            flatUI.m_DataContentList.push_back(WaaColour(0xf39c12));
            flatUI.m_DataContentList.push_back(WaaColour(0x8e44ad));
            flatUI.m_DataContentList.push_back(WaaColour(0x2c3e50));
            flatUI.m_inited = true;
        }

        return flatUI;
    }

    ColorTheme(WaaColour clearColor, WaaColour defaultPenColor, WaaColour warning, WaaColour bgGray) :
        m_ClearColor(clearColor), m_DefaultPenColor(defaultPenColor), m_Warning(warning), m_BackgroundGray(bgGray), m_inited(false)
    {
    }

    WaaColour m_ClearColor;
    WaaColour m_BackgroundGray;
    WaaColour m_DefaultPenColor;
    std::vector<WaaColour> m_DataContentList;
    WaaColour m_Warning;

    bool m_inited;
};