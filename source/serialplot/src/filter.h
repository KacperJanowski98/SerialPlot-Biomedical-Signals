#pragma once

enum class FilterVersion
{
    Butterworth = 0,
    ChebyshevI = 1,
    ChebyshevII = 2
};

enum class FilterType
{
    LowPass = 0,
    HighPass = 1,
    BandPass = 2,
    BandStop = 3
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
    Order10 = 10
};
