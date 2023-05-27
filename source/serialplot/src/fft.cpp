#include "fft.h"
#include <QDebug>

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
//        qDebug() << "Tworze nowy fftBufferIN o wielkosci: " << newSize;
        mFftBufferiN = new double[newSize]();
        mSizeControl = size;
    }

    if (mFftBufferOUT == nullptr)
//        qDebug() << "Tworze nowy fftBufferOUT o wielkosci: " << newSize/2;
        mFftBufferOUT =  new double[newSize/2]();

    if (flagReset)
    {
//        qDebug() << "wlaczona flaga flagReset";
        flagOverBuff = true;
        flagReset = false;
    }

    if (flagOverBuff)
    {
//        qDebug() << "wlaczona flaga flagOverBuff";
        mOffset = 0;
        mSizeControl = size;
        flagOverBuff = false;

        if (mFftBufferiN != nullptr)
        {
//            qDebug() << "usuwam i tworze nowy fftBufferIN o wielkosci: " << newSize;
            delete[] mFftBufferiN;
            mFftBufferiN = new double[newSize]();
        }
        if (mFftBufferOUT != nullptr)
        {
//            qDebug() << "usuwam i tworze nowy fftBufferOUT o wielkosci: " << newSize/2;
            delete[] mFftBufferOUT;
            mFftBufferOUT = new double[newSize/2]();
        }
    }

    memcpy(mFftBufferiN + mOffset, data, ns*sizeof(double));
//        for(unsigned i = 0; i < ns; i++)
//        {
//            qDebug() << "Calc, index: "<< i+offset << "val: " << fftBufferiN[i+offset];
//        }
    mOffset += ns;

//    qDebug() << "offset: " << offset;
//    qDebug() << "sizeControl: " << sizeControl << " size: " << size;

    if (mOffset >= mSizeControl)
    {
//        qDebug() << "offset " << offset << " wiekszy rowny od sizeControl: " << sizeControl << ", ustawiona flagOverBuff";
        flagOverBuff = true;
    }
}

//void Fft::calculateFft(double* dataIn, unsigned n)
void Fft::calculateFft()
{
    mFftIn  = fftw_alloc_real(mOffset);
//    mFftIn = fftw_alloc_complex(mOffset);
//    mFftOut = fftw_alloc_complex(mOffset);
    mFftOut = fftw_alloc_real(mOffset);
    mFftPlan = fftw_plan_r2r_1d(mOffset, mFftIn, mFftOut, FFTW_R2HC, FFTW_ESTIMATE);
//    mFftPlan = fftw_plan_dft_r2c_1d(mOffset, mFftIn, mFftOut, FFTW_ESTIMATE);
//    mFftPlan = fftw_plan_dft_1d(mOffset, mFftIn, mFftOut, FFTW_BACKWARD, FFTW_ESTIMATE);

    memcpy(mFftIn, mFftBufferiN, mOffset*sizeof(double));
//    for(unsigned int i = 0; i < mOffset; i++)
//        mFftIn[i][0] = mFftBufferiN[i];

    fftw_execute(mFftPlan);

//    for (unsigned i = 0; i < mOffset / 2; i++)
    for (unsigned i = 0; i < mOffset; i++)
    {
//        dataOut[i] = abs(mFftOut[i]);
//        mFftBufferOUT[i] = abs(mFftOut[i]);
//        mFftBufferOUT[i] = abs(mFftOut[i])/1000;
        mFftBufferOUT[i] = 2.0/mOffset * abs(mFftOut[i]);
//        qDebug() << "Index: " << i << " value: " << abs(mFftOut[i])/1000 << " -- " << 2.0/mOffset * abs(mFftOut[i]);
//        qDebug() << "Value: " << abs(mFftOut[i])/1000;
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
