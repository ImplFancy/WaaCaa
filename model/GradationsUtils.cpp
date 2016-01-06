#include "GradationsUtils.h"



float Gradations::calcAbsStep(const float &zoomFactor, const float &bestGridInPixel /*= 100.f*/)
{
    auto value = fabs(bestGridInPixel / zoomFactor);
    auto digit = floor(log10(fabs(value)));
    auto ret = round(value / pow(10, digit));

    if (ret > 7.5) ret = 10;
    else if (ret > 3.5) ret = 5;
    else if (ret > 1.5) ret = 2;
    else ret = 1;

    ret *= pow(10, digit);

    return fabs(ret);
}

float Gradations::calcAbsStepPixel(const float &zoomFactor)
{
    return fabs(calcAbsStep(zoomFactor) * zoomFactor);
}

float Gradations::calcStartPixel(const float &startPixel, const float &zoomFactor)
{
    auto stepPixel = calcAbsStepPixel(zoomFactor);

    return ceil(startPixel / stepPixel) * stepPixel;
}

float Gradations::calcEndPixel(const float &endPixel, const float &zoomFactor)
{
    auto stepPixel = calcAbsStepPixel(zoomFactor);

    return floor(endPixel / stepPixel) * stepPixel;
}
