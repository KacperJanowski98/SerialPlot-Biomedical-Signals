#ifndef FFT_H
#define FFT_H

#include <fftw3.h>

class Fft
{
public:
    Fft();
    ~Fft();

    void createFftBuffer(double *data, unsigned size, unsigned ns);
    //    void calculateFft(double* dataIn, unsigned n);
    void calculateFft();
    void clearFft();
    double* getDataBuffer();
    double* getFftBuffer();
    unsigned getFftSize();

    unsigned getOffset();
    unsigned getSize();
private:
    fftw_plan mFftPlan;
    double *mFftIn;
    double *mFftOut;
    double *mFftBufferiN;
    double *mFftBufferOUT;
    unsigned mOffset;
    bool flagReset;
    bool flagOverBuff;
    bool flagChangeSize;
    unsigned mSize;
    unsigned mSizeControl;
};

#endif  // FFT_H
