#include <QtGlobal>

#include "fftringbuffer.h"

#include <QDebug>

FftRingBuffer::FftRingBuffer(unsigned n)
{
    _size = n;
    data = new double[_size]();
    headIndex = 0;

    limInvalid = false;
    limCache = {0, 0};
    // FFT
//    mFftIn  = fftw_alloc_real(_size);
//    mFftOut = fftw_alloc_real(_size);
//    mFftPlan = fftw_plan_r2r_1d(_size, mFftIn, mFftOut, FFTW_R2HC, FFTW_ESTIMATE);
}

FftRingBuffer::~FftRingBuffer()
{
    delete[] data;
    // FFT
//    fftw_free(mFftIn);
//    fftw_free(mFftOut);
//    fftw_destroy_plan(mFftPlan);
}

unsigned FftRingBuffer::size() const
{
    return _size;
}

double FftRingBuffer::sample(unsigned i) const
{
    unsigned index = headIndex + i;
    if (index >= _size) index -= _size;
    return data[index];
}

Range FftRingBuffer::limits() const
{
    if (limInvalid) updateLimits();
    return limCache;
}

void FftRingBuffer::resize(unsigned n)
{
    Q_ASSERT(n != _size);

    int offset = (int) n - (int) _size;
    if (offset == 0) return;

    double* newData = new double[n];

    // move data to new array
    int fill_start = offset > 0 ? offset : 0;

    for (int i = fill_start; i < int(n); i++)
    {
        newData[i] = sample(i - offset);
    }

    // fill the beginning of the new data
    if (fill_start > 0)
    {
        for (int i = 0; i < fill_start; i++)
        {
            newData[i] = 0;
        }
    }

    // data is ready, clean up and re-point
    delete data;
    data = newData;
    headIndex = 0;
    _size = n;

    // invalidate bounding rectangle
    limInvalid = true;
}

void FftRingBuffer::addSamples(double* samples, unsigned n)
{
    unsigned shift = n;

    // FFT
//    memcpy(mFftIn, samples, shift*sizeof(double));
//    fftw_execute(mFftPlan);
//    fftw_execute_r2r(mFftPlan, mFftIn, mFftOut);

    if (shift < _size)
    {
        unsigned x = _size - headIndex; // distance of `head` to end

        if (shift <= x) // there is enough room at the end of array
        {
            for (unsigned i = 0; i < shift; i++)
            {
//                data[i+headIndex] = abs(mFftOut[i]);
//                qDebug() << "RingBuffer::addSamples FFT (there is enough room at the end of array): " << i+headIndex << " value " << abs(mFftOut[i]);
                data[i+headIndex] = samples[i];
//                qDebug() << "RingBuffer::addSamples FFT (there is enough room at the end of array): " << i+headIndex << " value " << samples[i] * 2;
            }

            if (shift == x) // we used all the room at the end
            {
                headIndex = 0;
            }
            else
            {
                headIndex += shift;
            }
        }
        else // there isn't enough room
        {
            for (unsigned i = 0; i < x; i++) // fill the end part
            {
                data[i+headIndex] = samples[i];
//                data[i+headIndex] = abs(mFftOut[i]);
//                qDebug() << "RingBuffer::addSamples FFT (fill the end part): " << i+headIndex << " value " << abs(mFftOut[i]);
//                qDebug() << "RingBuffer::addSamples FFT (fill the end part): " << i+headIndex << " value " << samples[i] * 2;
            }
            for (unsigned i = 0; i < (shift-x); i++) // continue from the beginning
            {
                data[i] = samples[i+x];
//                data[i] = abs(mFftOut[i+x]);
//                qDebug() << "RingBuffer::addSamples FFT (continue from the beginning): " << i+headIndex << " value " << abs(mFftOut[i+x]);
//                qDebug() << "RingBuffer::addSamples FFT (continue from the beginning): " << i << " value " << samples[i+x] * 2;
            }
            headIndex = shift-x;
        }
    }
    else // number of new samples equal or bigger than current size (doesn't fit)
    {
        int x = shift - _size;

        for (unsigned i = 0; i < _size; i++)
        {
            data[i] = samples[i+x];
//            data[i] = abs(mFftOut[i+x]);
//            qDebug() << "RingBuffer::addSamples FFT (number of new samples equal or bigger than current size): " << i+headIndex << " value " << abs(mFftOut[i+x]);
//            qDebug() << "RingBuffer::addSamples FFT (number of new samples equal or bigger than current size): " << i+headIndex << " value " << samples[i+x];
        }
        headIndex = 0;
    }

    // invalidate cache
    limInvalid = true;
}

void FftRingBuffer::clear()
{
    for (unsigned i=0; i < _size; i++)
    {
        data[i] = 0.;
    }

    limCache = {0, 0};
    limInvalid = false;
}

void FftRingBuffer::updateLimits() const
{
    limCache.start = data[0];
    limCache.end = data[0];

    for (unsigned i = 0; i < _size; i++)
    {
        if (data[i] > limCache.end)
        {
            limCache.end = data[i];
        }
        else if (data[i] < limCache.start)
        {
            limCache.start = data[i];
        }
    }

    limInvalid = false;
}
