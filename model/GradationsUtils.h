#pragma once

#include <cmath>

class Gradations
{
public:
    static float calcAbsStep(const float &zoomFactor, const float &bestGridInPixel = 100.f);

    static float calcAbsStepPixel(const float &zoomFactor);

    static float calcStartPixel(const float &startPixel, const float &zoomFactor);

    static float calcEndPixel(const float &endPixel, const float &zoomFactor);
};