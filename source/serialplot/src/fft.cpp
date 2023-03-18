#include "fft.h"
#include <QDebug>

Fft::Fft()
{
    flagReset = false;
    flagOverBuff = false;
    flagChangeSize = false;
    fftBufferiN = nullptr;
    fftBufferOUT = nullptr;

    offset = 0;
    size = 0;
    sizeControl = 0;
}

Fft::~Fft()
{
    if (fftBufferiN != nullptr)
        delete[] fftBufferiN;
    if (fftBufferOUT != nullptr)
        delete[] fftBufferOUT;
}

void Fft::createFftBuffer(double *data, unsigned size, unsigned ns)
{
    unsigned newSize = size * 4;

    if (fftBufferiN == nullptr)
    {
//        qDebug() << "Tworze nowy fftBufferIN o wielkosci: " << newSize;
        fftBufferiN = new double[newSize]();
        sizeControl = size;
    }

    if (fftBufferOUT == nullptr)
//        qDebug() << "Tworze nowy fftBufferOUT o wielkosci: " << newSize/2;
        fftBufferOUT =  new double[newSize/2]();

    if (flagReset)
    {
//        qDebug() << "wlaczona flaga flagReset";
        flagOverBuff = true;
        flagReset = false;
    }

    if (flagOverBuff)
    {
//        qDebug() << "wlaczona flaga flagOverBuff";
        offset = 0;
        sizeControl = size;
        flagOverBuff = false;

        if (fftBufferiN != nullptr)
        {
//            qDebug() << "usuwam i tworze nowy fftBufferIN o wielkosci: " << newSize;
            delete[] fftBufferiN;
            fftBufferiN = new double[newSize]();
        }
        if (fftBufferOUT != nullptr)
        {
//            qDebug() << "usuwam i tworze nowy fftBufferOUT o wielkosci: " << newSize/2;
            delete[] fftBufferOUT;
            fftBufferOUT = new double[newSize/2]();
        }
    }

    memcpy(fftBufferiN + offset, data, ns*sizeof(double));
//        for(unsigned i = 0; i < ns; i++)
//        {
//            qDebug() << "Calc, index: "<< i+offset << "val: " << fftBufferiN[i+offset];
//        }
    offset += ns;

//    qDebug() << "offset: " << offset;
//    qDebug() << "sizeControl: " << sizeControl << " size: " << size;

    if (offset >= sizeControl)
    {
//        qDebug() << "offset " << offset << " wiekszy rowny od sizeControl: " << sizeControl << ", ustawiona flagOverBuff";
        flagOverBuff = true;
    }
}

//void Fft::calculateFft(double* dataIn, unsigned n)
void Fft::calculateFft()
{
    mFftIn  = fftw_alloc_real(offset);
    mFftOut = fftw_alloc_real(offset);
    mFftPlan = fftw_plan_r2r_1d(offset, mFftIn, mFftOut, FFTW_R2HC, FFTW_ESTIMATE);

    memcpy(mFftIn, fftBufferiN, offset*sizeof(double));

    fftw_execute(mFftPlan);

    for (unsigned i = 0; i < offset/2; i++)
    {
//        dataOut[i] = abs(mFftOut[i]);
        fftBufferOUT[i] = abs(mFftOut[i]);
//        qDebug() << "Value: " << abs(mFftOut[i]);
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
    if (fftBufferiN != nullptr){
        return fftBufferiN;
    } else
    {
        return nullptr;
    }
}

double* Fft::getFftBuffer()
{
    if (fftBufferOUT != nullptr){
        return fftBufferOUT;
    } else
    {
        return nullptr;
    }
}

unsigned Fft::getFftSize()
{
    return offset/2;
}

unsigned Fft::getOffset()
{
    return offset;
}

unsigned Fft::getSize()
{
   return sizeControl;
}
