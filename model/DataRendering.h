#pragma once

class DataRendering
{
public:
    enum class Shape : unsigned char
    {
        NotDefined = 0x00,
        CircleDot = 0x01,
        Line,
        SquareDot,
        DashLine,
        _,
        NumbersOfShape
    };

private:
    friend class Dataset;

    DataRendering() :
        m_shape(Shape::NotDefined),
        m_visible(false)
    {

    }

    bool m_visible;
    Shape m_shape;
    unsigned int m_colorIndex; // TODO: use this field
};