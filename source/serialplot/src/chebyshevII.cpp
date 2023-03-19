#include "chebyshevII.h"

ChebyshevIIFilter::ChebyshevIIFilter(ChebyshevIIType type,
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
    case ChebyshevIIType::LowPass:
        mLowPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mStopBandDb);
        break;
    case ChebyshevIIType::HighPass:
        mHighPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mStopBandDb);
        break;
    default:
        break;
    }
}

ChebyshevIIFilter::ChebyshevIIFilter(ChebyshevIIType type,
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
    case ChebyshevIIType::BandPass:
        mBandPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq, mStopBandDb);
        break;
    case ChebyshevIIType::BandStop:
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
        case ChebyshevIIType::LowPass:
            data[i] = mLowPass.filter(data[i]);
            break;
        case ChebyshevIIType::HighPass:
            data[i] = mHighPass.filter(data[i]);
            break;
        case ChebyshevIIType::BandPass:
            data[i] = mBandPass.filter(data[i]);
            break;
        case ChebyshevIIType::BandStop:
            data[i] = mBandStop.filter(data[i]);
            break;
        default:
            break;
        }
    }
}
