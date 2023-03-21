#ifndef CHEBYSHEVIFILTER_H
#define CHEBYSHEVIFILTER_H

#pragma once

#include "Iir.h"
#include "filter.h"

class ChebyshevIFilter
{
public:
    ChebyshevIFilter(FilterType type,
                     FilterOrder order,
                     double samplingFreq,
                     double cutoffFreq,
                     double rippleDb);

    ChebyshevIFilter(FilterType type,
                     FilterOrder order,
                     double samplingFreq,
                     double rippleDb,
                     double centerFreq,
                     double widthFreq);

    ~ChebyshevIFilter() = default;

    void filterData(double *data, unsigned ns);

private:
    FilterType mType;
    FilterOrder mOrder;
    double mSamplingFreq;   // for LowPass, HighPass, BandPass, BandStop filter
    double mCutoffFreq;     // for LowPass, HighPass filter
    double mRippleDb;       // for LowPass, HighPass, BandPass, BandStop filter
    double mCenterFreq;     // for BandPass, BandStop filter
    double mWidthFreq;      // for BandPass, BandStop filter

    Iir::ChebyshevI::LowPass<12> mLowPass;
    Iir::ChebyshevI::HighPass<12> mHighPass;
    Iir::ChebyshevI::BandPass<12> mBandPass;
    Iir::ChebyshevI::BandStop<12> mBandStop;
};

#endif // CHEBYSHEVIFILTER_H
