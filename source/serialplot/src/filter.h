#pragma once

enum class ButterworthType
{
    LowPass,
    HighPass,
    BandPass,
    BandStop
};

enum class ChebyshevIType
{
    LowPass,
    HighPass,
    BandPass,
    BandStop
};

enum class ChebyshevIIType
{
    LowPass,
    HighPass,
    BandPass,
    BandStop
};

enum class RBJType
{
    LowPass,
    HighPass,
    BandPass1,
    BandPass2,
    BandStop,
    IIRNotch,
};

enum class FilterOrder
{
    Order1 = 1,
    Order2 = 2,
    Order3 = 3,
    Order4 = 4,
    Order5 = 5,
    Order6 = 6,
    Order7 = 7,
    Order8 = 8,
    Order9 = 9,
    Order10 = 10,
};
