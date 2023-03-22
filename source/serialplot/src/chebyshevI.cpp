#include "chebyshevI.h"

ChebyshevIFilter::ChebyshevIFilter(int type,
                                   int order,
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
    case 0:
        mLowPass.setup(mOrder, mSamplingFreq, mCutoffFreq, mRippleDb);
        break;
    case 1:
        mHighPass.setup(mOrder, mSamplingFreq, mCutoffFreq, mRippleDb);
        break;
    default:
        break;
    }
}

ChebyshevIFilter::ChebyshevIFilter(int type,
                                   int order,
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
    case 2:
        mBandPass.setup(mOrder, mSamplingFreq, mCenterFreq, mWidthFreq, mRippleDb);
        break;
    case 3:
        mBandStop.setup(mOrder, mSamplingFreq, mCenterFreq, mWidthFreq, mRippleDb);
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
