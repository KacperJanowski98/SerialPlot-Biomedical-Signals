#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#pragma once

#include "butterworthfilter.h"
#include "chebyshevI.h"
#include "chebyshevII.h"

class FilterManager
{
public:
    FilterManager();
    ~FilterManager();

    void setupFilter();
    void filterSignal(double *data, unsigned ns);

    void setFilterVersion(int value);
    void setFilterType(int value);
    void setFilterOrderB(int value);
    void setFilterOrderChI(int value);
    void setFilterOrderChII(int value);
    void setSamplingFreqB(double value);
    void setSamplingFreqChI(double value);
    void setSamplingFreqChII(double value);
    void setCutoffFreqB(double value);
    void setCutoffFreqChI(double value);
    void setCutoffFreqChII(double value);
    void setCenterFreqB(double value);
    void setCenterFreqChI(double value);
    void setCenterFreqChII(double value);
    void setWidthFreqB(double value);
    void setWidthFreqChI(double value);
    void setWidthFreqChII(double value);
    void setRippleChI(double value);
    void setStopBandChII(double value);

private:
    ButterworthFilter* mButterworthFilter;
    ChebyshevIFilter* mChebyshevIFilter;
    ChebyshevIIFilter* mChebyshevIIFilter;

    int _filterVersion;
    int _filterType;
    int _filterOrderB;
    int _filterOrderChI;
    int _filterOrderChII;
    double _samplingFreqB;
    double _samplingFreqChI;
    double _samplingFreqChII;
    double _cutoffFreqB;
    double _cutoffFreqChI;
    double _cutoffFreqChII;
    double _centerFreqB;
    double _centerFreqChI;
    double _centerFreqChII;
    double _widthFreqB;
    double _widthFreqChI;
    double _widthFreqChII;
    double _rippleChI;
    double _stopBandChII;
};

#endif // FILTERMANAGER_H
