#include "butterworthfilter.h"

ButterworthFilter::ButterworthFilter(int type,
                                       int order,
                                       double samplingFreq,
                                       double cutoffFreq)
    : mType(type)
    , mOrder(order)
    , mSamplingFreq(samplingFreq)
    , mCutoffFreq(cutoffFreq)
{
    switch (mType) {
    case 0:
        mLowPass.setup(mOrder, mSamplingFreq, mCutoffFreq);
        break;
    case 1:
        mHighPass.setup(mOrder, mSamplingFreq, mCutoffFreq);
        break;
    default:
        break;
    }
}

ButterworthFilter::ButterworthFilter(int type,
                                     int order,
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
    case 2:
        mBandPass.setup(mOrder, mSamplingFreq, mCenterFreq, mWidthFreq);
        break;
    case 3:
        mBandStop.setupN(mOrder, mSamplingFreq, mCenterFreq, mWidthFreq);
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
        case 0:
            data[i] = mLowPass.filter(data[i]);
            break;
        case 1:
            data[i] = mHighPass.filter(data[i]);
            break;
        case 2:
            data[i] = mBandPass.filter(data[i]);
            break;
        case 3:
            data[i] = mBandStop.filter(data[i]);
            break;
        default:
            break;
        }
    }
}
