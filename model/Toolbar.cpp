#include "Toolbar.h"



Toolbar::Toolbar() :
    m_isInited(false),
    m_orderTemp(0u),
    m_gapTmp(0u)
{
    m_Items.clear();
    m_iconWidth = 22u;
}

bool Toolbar::IsInited()
{
    return m_isInited;
}

void Toolbar::AddIcon(const std::wstring &name, BitmapInterface *pBitmap,
    Item::Enum def)
{
    m_Items.push_back(std::unique_ptr<Item>(new Item(name, pBitmap,
        m_orderTemp++ * m_iconWidth /* pix width */ * 1.3 + m_gapTmp, m_iconWidth, def)));

    m_isInited = true;
}

Toolbar::Item::~Item()
{
    if (m_pBitmap != nullptr) {
        delete m_pBitmap;
        m_pBitmap = nullptr;
    }
}

Toolbar::Item::Item(const std::wstring &name, BitmapInterface *pBitmap, unsigned int posOffset, unsigned int iconSize, Enum def) :
    m_name(name), m_pBitmap(pBitmap), m_posOffset(posOffset), m_iconSize(iconSize),
    m_type(def)
{

}

RectF Toolbar::Item::GetPosOffset(const unsigned int &topPos, const unsigned int &rightPos)
{
    return RectF(topPos, topPos + m_iconSize,
        rightPos - m_posOffset - m_iconSize,
        rightPos - m_posOffset);
}
