#include "chebyshevI.h"

ChebyshevIFilter::ChebyshevIFilter(ChebyshevIType type,
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
    case ChebyshevIType::LowPass:
        mLowPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mRippleDb);
        break;
    case ChebyshevIType::HighPass:
        mHighPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCutoffFreq, mRippleDb);
        break;
    default:
        break;
    }
}

ChebyshevIFilter::ChebyshevIFilter(ChebyshevIType type,
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
    case ChebyshevIType::BandPass:
        mBandPass.setup(static_cast<int>(mOrder), mSamplingFreq, mCenterFreq, mWidthFreq, mRippleDb);
        break;
    case ChebyshevIType::BandStop:
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
        case ChebyshevIType::LowPass:
            data[i] = mLowPass.filter(data[i]);
            break;
        case ChebyshevIType::HighPass:
            data[i] = mHighPass.filter(data[i]);
            break;
        case ChebyshevIType::BandPass:
            data[i] = mBandPass.filter(data[i]);
            break;
        case ChebyshevIType::BandStop:
            data[i] = mBandStop.filter(data[i]);
            break;
        default:
            break;
        }
    }
}
