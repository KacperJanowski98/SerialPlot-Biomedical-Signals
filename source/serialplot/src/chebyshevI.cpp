#include "chebyshevI.h"

ChebyshevIFilter::ChebyshevIFilter(FilterType type,
                                   FilterOrder order,
                                   double samplingFreq,
                                   double cutoffFreq,
                                   double rippleDb)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mCutoffFreq(cutoffFreq)
    , mRippleDb(rippleDb)
{
    switch (mType) {
    case FilterType::LowPass:
        mLowPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mRippleDb);
        break;
    case FilterType::HighPass:
        mHighPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mRippleDb);
        break;
    default:
        break;
    }
}

ChebyshevIFilter::ChebyshevIFilter(FilterType type,
                                   FilterOrder order,
                                   double samplingFreq,
                                   double rippleDb,
                                   double centerFreq,
                                   double widthFreq)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mRippleDb(rippleDb)
    , mCenterFreq(centerFreq)
    , mWidthFreq(widthFreq)
{
    switch (mType) {
    case FilterType::BandPass:
        mBandPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq, mRippleDb);
        break;
    case FilterType::BandStop:
        mBandStop.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq, mRippleDb);
        break;
    default:
        break;
    }
}

void ChebyshevIFilter::filterData(double *data, unsigned ns)
{
    for(unsigned i = 0; i < ns; i++)
    {
        switch (mType) {
        case FilterType::LowPass:
            data[i] = mLowPass.filter(data[i]);
            break;
        case FilterType::HighPass:
            data[i] = mHighPass.filter(data[i]);
            break;
        case FilterType::BandPass:
            data[i] = mBandPass.filter(data[i]);
            break;
        case FilterType::BandStop:
            data[i] = mBandStop.filter(data[i]);
            break;
        default:
            break;
        }
    }
}
