#pragma once

#include "utils/Rect.h"
#include "utils/Vec2d.h"

#include <string>
#include <vector>
#include <memory>

class BitmapInterface;

class Toolbar
{
    class Item
    {
    public:
        ~Item();

        enum class Enum : unsigned char
        {
            Pan = 0x01,
            BoxZoom,
            BoxSelect,
            ResetView = 0x10,
            WheelZoom,
            ExportFigure,
            Undef
        };

    protected:
        friend class Toolbar;
        friend class Chart;

        explicit Item(const std::wstring &name,
            BitmapInterface *pBitmap,
            unsigned int posOffset,
            unsigned int iconSize,
            Enum def);

        RectF viewRect;

        // Right-Top corner of windowSize
        virtual RectF GetPosOffset(const unsigned int &topPos, const unsigned int &rightPos);
        //virtual void OnClick() = 0;

        std::wstring m_name;
        BitmapInterface *m_pBitmap;

        unsigned int m_posOffset;
        unsigned int m_iconSize;

        Enum m_type;
    };

    friend class Chart;

    Toolbar();

    bool IsInited();

    void AddIconStart() {
        m_orderTemp = 0u; m_gapTmp = 0u;
    }
    void AddIcon(const std::wstring &name, BitmapInterface *pBitmap,
        Item::Enum def = Item::Enum::Pan);
    void AddSpace() { m_gapTmp += 6u; }
    void AddIconEnd() { }
    std::vector<std::unique_ptr<Item> > m_Items;

    bool m_isInited;

    unsigned int m_orderTemp;
    unsigned int m_gapTmp;

    unsigned int m_iconWidth;
};