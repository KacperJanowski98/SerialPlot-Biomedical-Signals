#ifndef CHEBYSHEVIIFILTER_H
#define CHEBYSHEVIIFILTER_H

#pragma once

#include "Iir.h"
#include "filter.h"

class ChebyshevIIFilter
{
public:
    ChebyshevIIFilter(FilterType type,
                      FilterOrder order,
                      double samplingFreq,
                      double cutoffFreq,
                      double stopBandDb);

    ChebyshevIIFilter(FilterType type,
                      FilterOrder order,
                      double samplingFreq,
                      double stopBandDb,
                      double centerFreq,
                      double widthFreq);

    ~ChebyshevIIFilter() = default;

    void filterData(double *data, unsigned ns);

private:
    FilterType mType;
    FilterOrder mOrder;
    double mSamplingFreq;   // for LowPass, HighPass, BandPass, BandStop filter
    double mCutoffFreq;     // for LowPass, HighPass filter
    double mStopBandDb;     // for LowPass, HighPass, BandPass, BandStop filter
    double mCenterFreq;     // for BandPass, BandStop filter
    double mWidthFreq;      // for BandPass, BandStop filter

    Iir::ChebyshevII::LowPass<12> mLowPass;
    Iir::ChebyshevII::HighPass<12> mHighPass;
    Iir::ChebyshevII::BandPass<12> mBandPass;
    Iir::ChebyshevII::BandStop<12> mBandStop;
};

#endif // CHEBYSHEVIIFILTER_H
