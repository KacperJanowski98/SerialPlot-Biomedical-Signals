#ifndef CHEBYSHEVIFILTER_H
#define CHEBYSHEVIFILTER_H

#pragma once

#include "Iir.h"
#include "filter.h"

class ChebyshevIFilter
{
public:
    ChebyshevIFilter(int type,
                     int order,
                     double samplingFreq,
                     double cutoffFreq,
                     double rippleDb);

    ChebyshevIFilter(int type,
                     int order,
                     double samplingFreq,
                     double rippleDb,
                     double centerFreq,
                     double widthFreq);

    ~ChebyshevIFilter() = default;

    void filterData(double *data, unsigned ns);

private:
    int mType;
    int mOrder;
    double mSamplingFreq;   // for LowPass, HighPass, BandPass, BandStop filter
    double mCutoffFreq;     // for LowPass, HighPass filter
    double mRippleDb;       // for LowPass, HighPass, BandPass, BandStop filter
    double mCenterFreq;     // for BandPass, BandStop filter
    double mWidthFreq;      // for BandPass, BandStop filter

    Iir::ChebyshevI::LowPass<22> mLowPass;
    Iir::ChebyshevI::HighPass<22> mHighPass;
    Iir::ChebyshevI::BandPass<22> mBandPass;
    Iir::ChebyshevI::BandStop<22> mBandStop;
};

#endif // CHEBYSHEVIFILTER_H
