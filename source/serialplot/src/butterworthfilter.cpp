#include "butterworthfilter.h"

ButterworthFilter::ButterworthFilter(ButterworthType type,
                                       FilterOrder order,
                                       double samplingFreq,
                                       double maxFreq)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mMaxFreq(maxFreq)
{
    switch (mType) {
    case ButterworthType::LowPass:
        mLowPass.setup(static_cast<int>(mOrder), mSamplingFreq, mMaxFreq);
        break;
    case ButterworthType::HighPass:
        mHighPass.setup(static_cast<int>(mOrder), mSamplingFreq, mMaxFreq);
        break;
    default:
        break;
    }
}

ButterworthFilter::ButterworthFilter(ButterworthType type,
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
    case ButterworthType::BandPass:
        mBandPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq);
        break;
    case ButterworthType::BandStop:
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
        data[i] = mLowPass.filter(data[i]);
        switch (mType) {
        case ButterworthType::LowPass:
            data[i] = mLowPass.filter(data[i]);
            break;
        case ButterworthType::HighPass:
            data[i] = mHighPass.filter(data[i]);
            break;
        case ButterworthType::BandPass:
            data[i] = mBandPass.filter(data[i]);
            break;
        case ButterworthType::BandStop:
            data[i] = mBandStop.filter(data[i]);
            break;
        default:
            break;
        }
    }
}
