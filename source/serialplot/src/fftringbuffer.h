#ifndef FFTRINGBUFFER_H
#define FFTRINGBUFFER_H

#include "framebuffer.h"
// FFT
//#include <fftw3.h>

/// A fast buffer implementation for storing data.
class FftRingBuffer : public WFrameBuffer
{
public:
    FftRingBuffer(unsigned n);
    ~FftRingBuffer();

    virtual unsigned size() const;
    virtual double sample(unsigned i) const;
    virtual Range limits() const;
    virtual void resize(unsigned n);
    virtual void addSamples(double* samples, unsigned n);
    virtual void clear();

private:
    unsigned _size;            ///< size of `data`
    double* data;              ///< storage
    unsigned headIndex;        ///< indicates the actual `0` index of the ring buffer
    // FFT
//    fftw_plan mFftPlan;
//    double *mFftIn;
//    double *mFftOut;

    mutable bool limInvalid;   ///< Indicates that limits needs to be re-calculated
    mutable Range limCache;    ///< Cache for limits()
    void updateLimits() const; ///< Updates limits cache
};

#endif
