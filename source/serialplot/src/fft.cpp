#include "fft.h"
#include <QDebug>
#include <complex>

Fft::Fft()
{
    flagReset = false;
    flagOverBuff = false;
    flagChangeSize = false;
    mFftBufferiN = nullptr;
    mFftBufferOUT = nullptr;

    mOffset = 0;
    mSize = 0;
    mSizeControl = 0;
}

Fft::~Fft()
{
    if (mFftBufferiN != nullptr)
        delete[] mFftBufferiN;
    if (mFftBufferOUT != nullptr)
        delete[] mFftBufferOUT;
}

void Fft::createFftBuffer(double *data, unsigned size, unsigned ns)
{
    unsigned newSize = size * 4;

    if (mFftBufferiN == nullptr)
    {
        mFftBufferiN = new double[newSize]();
        mSizeControl = size;
    }

    if (mFftBufferOUT == nullptr)
        mFftBufferOUT =  new double[newSize/2]();

    if (flagReset)
    {
        flagOverBuff = true;
        flagReset = false;
    }

    if (flagOverBuff)
    {
        mOffset = 0;
        mSizeControl = size;
        flagOverBuff = false;

        if (mFftBufferiN != nullptr)
        {
            delete[] mFftBufferiN;
            mFftBufferiN = new double[newSize]();
        }
        if (mFftBufferOUT != nullptr)
        {
            delete[] mFftBufferOUT;
            mFftBufferOUT = new double[newSize/2]();
        }
    }

    memcpy(mFftBufferiN + mOffset, data, ns*sizeof(double));
    mOffset += ns;

    if (mOffset >= mSizeControl)
    {
        flagOverBuff = true;
    }
}

void Fft::calculateFft()
{
    mFftIn  = fftw_alloc_real(mOffset);
    mFftOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (mOffset / 2 + 1));
    mFftPlan = fftw_plan_dft_r2c_1d(mOffset, mFftIn, mFftOut, FFTW_ESTIMATE);

    memcpy(mFftIn, mFftBufferiN, mOffset*sizeof(double));

    fftw_execute(mFftPlan);

    std::complex<double> *complexOut;
    complexOut = reinterpret_cast<std::complex<double> *>(mFftOut);

    for (unsigned i = 0; i < mOffset/2 + 1; i++)
    {
        mFftBufferOUT[i] = 2.0/mOffset * abs(complexOut[i]);
    }

    fftw_free(mFftIn);
    fftw_free(mFftOut);
    fftw_destroy_plan(mFftPlan);
}

void Fft::clearFft()
{
    flagReset = true;
}

double* Fft::getDataBuffer()
{
    if (mFftBufferiN != nullptr){
        return mFftBufferiN;
    } else
    {
        return nullptr;
    }
}

double* Fft::getFftBuffer()
{
    if (mFftBufferOUT != nullptr){
        return mFftBufferOUT;
    } else
    {
        return nullptr;
    }
}

unsigned Fft::getFftSize()
{
    return mOffset/2;
}

unsigned Fft::getOffset()
{
    return mOffset;
}

unsigned Fft::getSize()
{
    return mSizeControl;
}
