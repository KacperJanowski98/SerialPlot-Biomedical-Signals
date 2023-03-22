#include "chebyshevII.h"

ChebyshevIIFilter::ChebyshevIIFilter(int type,
                                     int order,
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
    case 0:
        mLowPass.setup(mOrder, mSamplingFreq, mCutoffFreq, mStopBandDb);
        break;
    case 1:
        mHighPass.setup(mOrder, mSamplingFreq, mCutoffFreq, mStopBandDb);
        break;
    default:
        break;
    }
}

ChebyshevIIFilter::ChebyshevIIFilter(int type,
                                     int order,
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
    case 2:
        mBandPass.setup(mOrder, mSamplingFreq, mCenterFreq, mWidthFreq, mStopBandDb);
        break;
    case 3:
        mBandStop.setup(mOrder, mSamplingFreq, mCenterFreq, mWidthFreq, mStopBandDb);
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
