#include "Dataset.h"

#include <limits>

// Ref: ElemDataType
unsigned char Dataset::s_NbBytesOfElemDataType[11] = {
    0, // _ElemFloat64 = 0x01, // double
    8, // _ElemFloat32,
    4, // ...
    8,
    8,
    4,
    4,
    2, // short
    2, // unsigned short
    1,
    1
};

Dataset::Dataset(const Dimension &dim, const ArrangeType &arrangeType, const ElemDataType &elemtype, const void *srcDataBuffer, unsigned int dataLengthInBytes) :
    m_dim(dim),
    m_arrangeType(arrangeType),
    m_eleType(elemtype)
{
    m_dataVec.clear();

    auto len = dataLengthInBytes / s_NbBytesOfElemDataType[(unsigned char)m_eleType];
    if (m_dim == Dimension::_1D) {
        if (m_eleType == ElemDataType::_ElemFloat64) {
            const double *pFloat = (const double *)srcDataBuffer;
            unsigned int stepOfOneValue = 1u;
            for (auto index = 0u; index < len; index++) {
                m_dataVec.push_back(Vec2dFloat(index, *pFloat));
                pFloat++;
            }
        }
        else if (m_eleType == ElemDataType::_ElemFloat32) {
            const float *pFloat = (const float *)srcDataBuffer;
            unsigned int stepOfOneValue = 1u;
            for (auto index = 0u; index < len; index++) {
                m_dataVec.push_back(Vec2dFloat(index, *pFloat));
                pFloat++;
            }
        }
    }
    else if (m_dim == Dimension::_2D) {
        len /= 2;
        if (m_eleType == ElemDataType::_ElemFloat32) {
            const float *pFloat = (const float *)srcDataBuffer;
            unsigned int stepOfOneValue;
            if (m_arrangeType == ArrangeType::_Compact) {
                for (auto index = 0u; index < len; index++) {
                    m_dataVec.push_back(Vec2dFloat(*pFloat, *(pFloat + 1)));
                    pFloat += 2;
                }
            }
            else if (m_arrangeType == ArrangeType::_Seperate) {
                for (auto index = 0u; index < len; index++) {
                    m_dataVec.push_back(Vec2dFloat(*pFloat, *(pFloat + len)));
                    pFloat += 1;
                }
            }
        }
    }

    if (!m_dataVec.empty()) {
        maxX = maxY = std::numeric_limits<float>::min();
        minX = minY = std::numeric_limits<float>::max();

        for (auto &v : m_dataVec) {
            if (v.x < minX) { minX = v.x; }
            if (v.y < minY) { minY = v.y; }
            if (v.x > maxX) { maxX = v.x; }
            if (v.y > maxY) { maxY = v.y; }
        }
    }
    
    m_ID = s_DatasetId++;
}

Dataset::~Dataset()
{
    m_dataVec.clear();
}

Dataset::Dimension Dataset::Dim()
{
    return m_dim;
}

Dataset::ArrangeType Dataset::arrangeT()
{
    return m_arrangeType;
}

Dataset::ElemDataType Dataset::ElemType()
{
    return m_eleType;
}

unsigned int Dataset::s_DatasetId = 1u;