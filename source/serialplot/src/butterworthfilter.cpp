#include "butterworthfilter.h"

ButterworthFilter::ButterworthFilter(FilterType type,
                                       FilterOrder order,
                                       double samplingFreq,
                                       double cutoffFreq)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mCutoffFreq(cutoffFreq)
{
    switch (mType) {
    case FilterType::LowPass:
        mLowPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq);
        break;
    case FilterType::HighPass:
        mHighPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq);
        break;
    default:
        break;
    }
}

ButterworthFilter::ButterworthFilter(FilterType type,
                                     FilterOrder order,
                                     double samplingFreq,
                                     double centerFreq,
                                     double widthFreq)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mCenterFreq(centerFreq)
    , mWidthFreq(widthFreq)
{
    switch (mType) {
    case FilterType::BandPass:
        mBandPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq);
        break;
    case FilterType::BandStop:
        mBandStop.setupN(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq);
        break;
    default:
        break;
    }
}

void ButterworthFilter::filterData(double *data, unsigned ns)
{
    for (unsigned i = 0; i < ns; i++)
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
