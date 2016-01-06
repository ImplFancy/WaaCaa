#pragma once

// RequestType: 0x01 ~ 0xFF
enum class RequestMajorType : unsigned char
{
    A = 0x01,
    B = 0x02,
    C = 0x03,
    D = 0x04
};

enum class RequestSubTypeA : unsigned char
{
    // Type A: WaaCaa about
    CreateOneChart = 0x01,
    CloseOneChart,
    CloseAllChart,
};

enum class RequestSubTypeB : unsigned char
{
    // Type B: Chart about
    SetChartTitle = 0x10,
    SetChartUiTheme,
};

enum class RequestSubTypeC : unsigned char
{
    // Type C: Data about
    _1D,
    _2D
};

enum class RequestSubTypeD : unsigned char
{
    SimpleDraw
};
