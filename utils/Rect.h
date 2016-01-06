#pragma once

template<typename T>
struct Rect
{
    Rect(const T &t, const T &b, const T &l, const T &r) {
        top = t;
        bottom = b;
        left = l;
        right = r;
    }

    T AbsWidth() const {
        T width = Width();
        if (width < 0) width *= -1;
        return width;
    }

    T AbsHeight() const {
        T height = Height();
        if (height < 0) height *= -1;
        return height;
    }

    T Width() const {
        return right - left;
    }
    
    T Height() const {
        return bottom - top;
    }

    Rect Scale(const float &scale) const {
        T centX = (left + right) / 2;
        T centY = (top + bottom) / 2;

        return Rect(centY - Height()*scale / 2, centY + Height()*scale / 2, centX - Width()*scale / 2, centX + Width()*scale / 2);
    }

    bool operator==(const Rect &b) const {
        return (top == b.top && bottom == b.bottom && left == b.left && right == b.right);
    }

    bool operator!=(const Rect &b) const {
        return !(*this == b);
    }

    Rect()
    {
        top = bottom = left = right = (T)(0);
    }


    T top;
    T bottom;
    T left;
    T right;
};

typedef Rect<float> RectF;