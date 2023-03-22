#ifndef BUTTERWORTHFILTER_H
#define BUTTERWORTHFILTER_H

#pragma once

#include "Iir.h"
#include "filter.h"

class ButterworthFilter
{
public:
    ButterworthFilter(int type,
                      int order,
                      double samplingFreq,
                      double cutoffFreq);

    ButterworthFilter(int type,
                      int order,
                      double samplingFreq,
                      double centerFreq,
                      double widthFreq);

    ~ButterworthFilter() = default;

    void filterData(double *data, unsigned ns);

private:
    int mType;
    int mOrder;
    double mSamplingFreq;   // for LowPass, HighPass, BandPass, BandStop filter
    double mCutoffFreq;     // for LowPass, HighPass filter
    double mCenterFreq;     // for BandPass, BandStop filter
    double mWidthFreq;      // for BandPass, BandStop filter

    Iir::Butterworth::LowPass<12> mLowPass;
    Iir::Butterworth::HighPass<12> mHighPass;
    Iir::Butterworth::BandPass<12> mBandPass;
    Iir::Butterworth::BandStop<12> mBandStop;
};

#endif // BUTTERWORTHFILTER_H
