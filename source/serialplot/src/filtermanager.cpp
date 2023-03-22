#include "filtermanager.h"

FilterManager::FilterManager()
{
    mButterworthFilter = nullptr;
    mChebyshevIFilter = nullptr;
    mChebyshevIIFilter = nullptr;

    _filterVersion = 0;
    _filterType = 0;
    _filterOrderB = 0;
    _filterOrderChI = 0;
    _filterOrderChII = 0;
    _samplingFreqB = 0;
    _samplingFreqChI = 0;
    _samplingFreqChII = 0;
    _cutoffFreqB = 0;
    _cutoffFreqChI = 0;
    _cutoffFreqChII = 0;
    _centerFreqB = 0;
    _centerFreqChI = 0;
    _centerFreqChII = 0;
    _widthFreqB = 0;
    _widthFreqChI = 0;
    _widthFreqChII = 0;
    _rippleChI = 0;
    _stopBandChII = 0;
}

FilterManager::~FilterManager()
{
    if (mButterworthFilter != nullptr)
        delete mButterworthFilter;
    if (mChebyshevIFilter != nullptr)
        delete mChebyshevIFilter;
    if (mChebyshevIIFilter != nullptr)
        delete mChebyshevIIFilter;
}

void FilterManager::setupFilter()
{
    if (_filterVersion == 0) // Butterworth
    {
        if (mButterworthFilter != nullptr)
            delete mButterworthFilter;

        if (_filterType == 0 or _filterType == 1)
        {
             mButterworthFilter = new ButterworthFilter(_filterType,
                                                        _filterOrderB,
                                                        _samplingFreqB,
                                                        _cutoffFreqB);
        }
        else if (_filterType == 2 or _filterType == 3)
        {
            mButterworthFilter = new ButterworthFilter(_filterType,
                                                       _filterOrderB,
                                                       _samplingFreqB,
                                                       _centerFreqB,
                                                       _widthFreqB);
        }
    }
    else if (_filterVersion == 1) // ChebyshevI
    {
        if (mChebyshevIFilter != nullptr)
            delete mChebyshevIFilter;

        if (_filterType == 0 or _filterType == 1)
        {
             mChebyshevIFilter = new ChebyshevIFilter(_filterType,
                                                      _filterOrderChI,
                                                      _samplingFreqChI,
                                                      _cutoffFreqChI,
                                                      _rippleChI);
        }
        else if (_filterType == 2 or _filterType == 3)
        {
            mChebyshevIFilter = new ChebyshevIFilter(_filterType,
                                                     _filterOrderChI,
                                                     _samplingFreqChI,
                                                     _rippleChI,
                                                     _centerFreqChI,
                                                     _widthFreqChI);
        }
    }
    else if (_filterVersion == 2) // ChebyshevII
    {
        if (mChebyshevIIFilter != nullptr)
            delete mChebyshevIIFilter;

        if (_filterType == 0 or _filterType == 1)
        {
             mChebyshevIIFilter = new ChebyshevIIFilter(_filterType,
                                                        _filterOrderChII,
                                                        _samplingFreqChII,
                                                        _cutoffFreqChII,
                                                        _stopBandChII);
        }
        else if (_filterType == 2 or _filterType == 3)
        {
            mChebyshevIIFilter = new ChebyshevIIFilter(_filterType,
                                                       _filterOrderChII,
                                                       _samplingFreqChII,
                                                       _stopBandChII,
                                                       _centerFreqChII,
                                                       _widthFreqChII);
        }
    }
}

void FilterManager::filterSignal(double *data, unsigned ns)
{
    if (_filterVersion == 0) // // Butterworth
    {
        mButterworthFilter->filterData(data, ns);
    }
    else if (_filterVersion == 1) // ChebyshevI
    {
        mChebyshevIFilter->filterData(data, ns);
    }
    else if (_filterVersion == 2) // ChebyshevII
    {
        mChebyshevIIFilter->filterData(data, ns);
    }
}

void FilterManager::setFilterVersion(int value)
{
    _filterVersion = value;
}

void FilterManager::setFilterType(int value)
{
    _filterType = value;
}

void FilterManager::setFilterOrderB(int value)
{
    _filterOrderB = value;
}

void FilterManager::setFilterOrderChI(int value)
{
    _filterOrderChI = value;
}

void FilterManager::setFilterOrderChII(int value)
{
    _filterOrderChII = value;
}

void FilterManager::setSamplingFreqB(double value)
{
    _samplingFreqB = value;
}

void FilterManager::setSamplingFreqChI(double value)
{
    _samplingFreqChI = value;
}

void FilterManager::setSamplingFreqChII(double value)
{
    _samplingFreqChII = value;
}

void FilterManager::setCutoffFreqB(double value)
{
    _cutoffFreqB = value;
}

void FilterManager::setCutoffFreqChI(double value)
{
    _cutoffFreqChI = value;
}

void FilterManager::setCutoffFreqChII(double value)
{
    _cutoffFreqChII = value;
}

void FilterManager::setCenterFreqB(double value)
{
    _centerFreqB = value;
}

void FilterManager::setCenterFreqChI(double value)
{
    _centerFreqChI = value;
}

void FilterManager::setCenterFreqChII(double value)
{
    _centerFreqChII = value;
}

void FilterManager::setWidthFreqB(double value)
{
    _widthFreqB = value;
}

void FilterManager::setWidthFreqChI(double value)
{
    _widthFreqChI = value;
}

void FilterManager::setWidthFreqChII(double value)
{
    _widthFreqChII = value;
}

void FilterManager::setRippleChI(double value)
{
    _rippleChI = value;
}

void FilterManager::setStopBandChII(double value)
{
    _stopBandChII = value;
}
