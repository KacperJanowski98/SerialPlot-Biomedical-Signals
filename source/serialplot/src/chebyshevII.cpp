#include "chebyshevII.h"

ChebyshevIIFilter::ChebyshevIIFilter(FilterType type,
                                     FilterOrder order,
                                     double samplingFreq,
                                     double cutoffFreq,
                                     double stopBandDb)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mCutoffFreq(cutoffFreq)
    , mStopBandDb(stopBandDb)
{
    switch (mType) {
    case FilterType::LowPass:
        mLowPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mStopBandDb);
        break;
    case FilterType::HighPass:
        mHighPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mStopBandDb);
        break;
    default:
        break;
    }
}

ChebyshevIIFilter::ChebyshevIIFilter(FilterType type,
                                     FilterOrder order,
                                     double samplingFreq,
                                     double stopBandDb,
                                     double centerFreq,
                                     double widthFreq)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mStopBandDb(stopBandDb)
    , mCenterFreq(centerFreq)
    , mWidthFreq(widthFreq)
{
    switch (mType) {
    case FilterType::BandPass:
        mBandPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq, mStopBandDb);
        break;
    case FilterType::BandStop:
        mBandStop.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq, mStopBandDb);
        break;
    default:
        break;
    }
}

void ChebyshevIIFilter::filterData(double *data, unsigned ns)
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
