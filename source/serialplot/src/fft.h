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
    double *fftBufferiN;
    double *fftBufferOUT;
    unsigned offset;
    bool flagReset;
    bool flagOverBuff;
    bool flagChangeSize;
    unsigned size;
    unsigned sizeControl;
};

#endif  // FFT_H
