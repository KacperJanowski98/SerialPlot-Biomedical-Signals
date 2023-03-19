#ifndef BUTTERWORTHFILTER_H
#define BUTTERWORTHFILTER_H

#include "Iir.h"

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

class ButterworthFilter
{
public:
    ButterworthFilter(ButterworthType type,
                      FilterOrder order,
                      double samplingFreq,
                      double maxFreq);

    ButterworthFilter(ButterworthType type,
                      FilterOrder order,
                      double samplingFreq,
                      double centerFreq,
                      double widthFreq);

    ~ButterworthFilter() = default;

    void filterData(double *data, unsigned ns);

private:
    bool flag;
    ButterworthType mType;
    FilterOrder mOrder;
    double mSamplingFreq;   // for LowPass, HighPass filter
    double mMaxFreq;        // for LowPass, HighPass filter
    double mCenterFreq;     // for BandPass, BandStop filter
    double mWidthFreq;      // for BandPass, BandStop filter

    Iir::Butterworth::LowPass<12> mLowPass;
    Iir::Butterworth::HighPass<12> mHighPass;
    Iir::Butterworth::BandPass<12> mBandPass;
    Iir::Butterworth::BandStop<12> mBandStop;
};

#endif // BUTTERWORTHFILTER_H
