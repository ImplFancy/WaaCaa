#pragma once

#include "utils/LinearBuffer.h"
#include "utils/Vec2d.h"

#include <vector>

class Dataset
{
public:
    enum class Dimension : unsigned char
    {
        _1D = 0x01,
        _2D
    };

    enum class ArrangeType : unsigned char
    {
        _Seperate = 0x01,
        _Compact
    };

    enum class ElemDataType : unsigned char
    {
        _ElemFloat64 = 0x01, // double
        _ElemFloat32,
        _ElemInt64,
        _ElemUInt64,
        _ElemInt32,
        _ElemUInt32,
        _ElemInt16, // short
        _ElemUInt16, // unsigned short
        _ElemInt8,
        _ElemUInt8
    };

    static unsigned char s_NbBytesOfElemDataType[11];

    Dataset(const Dimension &dim, const ArrangeType &arrangeType, const ElemDataType &elemtype, const void *srcDataBuffer, unsigned int dataLengthInBytes);

    Dataset(const Dataset &b) = delete;

    Dataset operator=(const Dataset& b) = delete;

    ~Dataset();

    unsigned int GetNbOfDataPoint() {
        return m_dataVec.size();
    }


    Dimension Dim();

    ArrangeType arrangeT();

    ElemDataType ElemType();
    
    float GetMinX() const { return minX; }

    float GetMinY() const { return minY; }

    float GetMaxX() const { return maxX; }

    float GetMaxY() const { return maxY; }

    void BeginRetrival() { m_currPointIndex = 0u; }

    Vec2dFloat *GetNextPoint() { if (m_currPointIndex < m_dataVec.size()) return &(m_dataVec.at(m_currPointIndex++)); else return nullptr; }

    unsigned int GetId() { return m_ID; }

private:

    unsigned int m_ID;

    Dimension m_dim;

    ArrangeType m_arrangeType;
    
    ElemDataType m_eleType;

    // For Cartesian Chart
    std::vector<Vec2dFloat> m_dataVec;
    unsigned int m_currPointIndex;

    float minX, maxX, minY, maxY;

};

