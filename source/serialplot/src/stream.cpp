/*
  Copyright © 2018 Hasan Yavuz Özderya

  This file is part of serialplot.

  serialplot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  serialplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with serialplot.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stream.h"
#include "ringbuffer.h"
#include "indexbuffer.h"
#include "linindexbuffer.h"

#include <QDebug>

Stream::Stream(unsigned nc, bool x, unsigned ns) :
    _infoModel(nc)
{
    _numSamples = ns;
    _paused = false;

    xAsIndex = true;
    xMin = 0;
    xMax = 1;

    flag = false;
    flagReset = false;
    flagOverBuff = false;
    flagChangeSize = false;
    flagSmallNs = false;
    fftBufferiN = nullptr;
    fftBufferOUT = nullptr;

    offset = 0;
    offsetData = 0;
    offsetSmall = 0;
    size = 0;
    sizeControl = 0;

    // create xdata buffer
    _hasx = x;
    if (x)
    {
        // TODO: implement XRingBuffer (binary search)
        Q_ASSERT(false);
    }
    else
    {
        xData = makeXBuffer();
    }

    // create channels
    for (unsigned i = 0; i < nc; i++)
    {
        auto c = new StreamChannel(i, xData, new RingBuffer(ns), &_infoModel);
        channels.append(c);
    }
}

Stream::~Stream()
{
    for (auto ch : channels)
    {
        delete ch;
    }
    delete xData;
    // FFT
    if (fftBufferiN != nullptr)
        delete[] fftBufferiN;
    if (fftBufferOUT != nullptr)
        delete[] fftBufferOUT;
}

bool Stream::hasX() const
{
    return _hasx;
}

unsigned Stream::numChannels() const
{
    return channels.length();
}

unsigned Stream::numSamples() const
{
    return _numSamples;
}

const StreamChannel* Stream::channel(unsigned index) const
{
    Q_ASSERT(index < numChannels());
    return channels[index];
}

StreamChannel* Stream::channel(unsigned index)
{
    return const_cast<StreamChannel*>(static_cast<const Stream&>(*this).channel(index));
}

QVector<const StreamChannel*> Stream::allChannels() const
{
    QVector<const StreamChannel*> result(numChannels());
    for (unsigned ci = 0; ci < numChannels(); ci++)
    {
        result[ci] = channel(ci);
    }
    return result;
}

const ChannelInfoModel* Stream::infoModel() const
{
    return &_infoModel;
}

ChannelInfoModel* Stream::infoModel()
{
    return const_cast<ChannelInfoModel*>(static_cast<const Stream&>(*this).infoModel());
}

void Stream::setNumChannels(unsigned nc, bool x)
{
    unsigned oldNum = numChannels();
    if (oldNum == nc && x == _hasx) return;

    // adjust the number of channels
    if (nc > oldNum)
    {
        for (unsigned i = oldNum; i < nc; i++)
        {
            auto c = new StreamChannel(i, xData, new RingBuffer(_numSamples), &_infoModel);
            channels.append(c);
        }
    }
    else if (nc < oldNum)
    {
        for (unsigned i = oldNum-1; i > nc-1; i--)
        {
            delete channels.takeLast();
        }
    }

    // change the xdata
    if (x != _hasx)
    {
        if (x)
        {
            // TODO: implement XRingBuffer (binary search)
            Q_ASSERT(false);
        }
        else
        {
            xData = makeXBuffer();
        }

        for (auto c : channels)
        {
            c->setX(xData);
        }
        _hasx = x;
    }

    if (nc != oldNum)
    {
        _infoModel.setNumOfChannels(nc);
        // TODO: how about X change?
        emit numChannelsChanged(nc);
    }

    Sink::setNumChannels(nc, x);
}

XFrameBuffer* Stream::makeXBuffer() const
{
    if (xAsIndex)
    {
        return new IndexBuffer(_numSamples);
    }
    else
    {
        return new LinIndexBuffer(_numSamples, xMin, xMax);
    }
}

const SamplePack* Stream::applyGainOffset(const SamplePack& pack) const
{
    qDebug() << "Test: Stream::applyGainOffset";
    Q_ASSERT(infoModel()->gainOrOffsetEn());

    SamplePack* mPack = new SamplePack(pack);
    unsigned ns = pack.numSamples();

    for (unsigned ci = 0; ci < numChannels(); ci++)
    {
        // TODO: we could use some kind of map (int32, int64 would suffice) to speed things up
        bool gainEn = infoModel()->gainEn(ci);
        bool offsetEn = infoModel()->offsetEn(ci);
        if (gainEn || offsetEn)
        {
            double* mdata = mPack->data(ci);
            double gain = infoModel()->gain(ci);
            double offset = infoModel()->offset(ci);

            if (gainEn)
            {
                for (unsigned i = 0; i < ns; i++)
                {
                    mdata[i] *= gain;
                }
            }
            if (offsetEn)
            {
                for (unsigned i = 0; i < ns; i++)
                {
                    mdata[i] += offset;
                }
            }
        }
    }

    return mPack;
}

void Stream::feedIn(const SamplePack& pack)
{
    Q_ASSERT(pack.numChannels() == numChannels() &&
             pack.hasX() == hasX());

    if (_paused) return;

    unsigned ns = pack.numSamples();
    if (_hasx)
    {
        // TODO: implement XRingBuffer (binary search)
        Q_ASSERT(false);
        // static_cast<RingBuffer*>(xData)->addSamples(pack.xData(), ns);
    }

    // modified pack that gain and offset is applied to
    const SamplePack* mPack = nullptr;
    if (infoModel()->gainOrOffsetEn())
        mPack = applyGainOffset(pack);

    for (unsigned ci = 0; ci < numChannels(); ci++)
    {
        auto buf = static_cast<RingBuffer*>(channels[ci]->yData());
        double* data = (mPack == nullptr) ? pack.data(ci) : mPack->data(ci);
//        qDebug() << "ns: " << ns;
        buf->addSamples(data, ns);
        size = buf->size();
        createFftBuffer(data, size, ns);
    }

    Sink::feedIn((mPack == nullptr) ? pack : *mPack);

    if (mPack != nullptr) delete mPack;
    if (offset >= size)
    {
        qDebug() << "FFT ---------------------- przy rozmiarze: " << offset;
        calculateFft(fftBufferiN, offset);
        emit fftBufferFull();
    }
    emit dataAdded();
}

void Stream::pause(bool paused)
{
    _paused = paused;
}

void Stream::clear()
{
    for (auto c : channels)
    {
        static_cast<RingBuffer*>(c->yData())->clear();
    }
}

void Stream::setNumSamples(unsigned value)
{
    if (value == _numSamples) return;
    _numSamples = value;

    xData->resize(value);
    for (auto c : channels)
    {
        static_cast<RingBuffer*>(c->yData())->resize(value);
    }
}

void Stream::createFftBuffer(double *data, unsigned size, unsigned ns)
{
    unsigned newSize = size * 2;
//    qDebug() << "data[0] " << data[0];
//    qDebug() << "data[1] " << data[1];
//    qDebug() << "data[2] " << data[2];
//    qDebug() << "data[3]] " << data[3];
//    qDebug() << "data[4] " << data[4];
//    qDebug() << "data[5] " << data[5];
//    qDebug() << "data[6] " << data[6];
//    qDebug() << "data[7] " << data[7];
//    qDebug() << "data[8] " << data[8];
//    qDebug() << "data[9] " << data[9];
//    qDebug() << "data[10] " << data[10];
//    qDebug() << "data[11] " << data[11];
//    qDebug() << "data[12] " << data[12];
//    qDebug() << "data[13] " << data[13];

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

    if (static_cast<unsigned>(data[0]) == 30) // && static_cast<unsigned>(data[4]) == size)
    {
        flag = true;
    }

    if (flag)
    {
        if (ns < 12)
        {
//            memcpy(fftBufferiN + offset, data + ns, ns*sizeof(double));
            offsetSmall += ns;
            flagSmallNs = true;
        } else
        {
            if (flagSmallNs)
            {
                unsigned tempOffset = 12 - offsetSmall;
                unsigned temp = ns - tempOffset;
                memcpy(fftBufferiN + offset, data + tempOffset, temp*sizeof(double));
                offset += temp;
                flagSmallNs = false;
                flag = false;
            } else
            {
                unsigned temp = ns - 12;
        //        qDebug() << "Dodaje pierwsze " << temp << " probki przesuniete o 12";
                memcpy(fftBufferiN + offset, data + 12, temp*sizeof(double));
        //        for(unsigned i = 0; i < temp; i++)
        //        {
        //            qDebug() << "Calc, index: "<< i << "val: " << fftBufferiN[i+offset];
        //        }
                offset += temp;
                flag = false;
            }
        }
    } else
    {
//        qDebug() << "Dodaje kolejne " << ns << " probek przesuniete o: " << offset;
        memcpy(fftBufferiN + offset, data, ns*sizeof(double));
//        for(unsigned i = 0; i < ns; i++)
//        {
//            qDebug() << "Calc, index: "<< i+offset << "val: " << fftBufferiN[i+offset];
//        }
        offset += ns;
    }

    if (offset >= sizeControl)
    {
//        qDebug() << "offset " << offset << " wiekszy rowny od size: " << size << ", ustawiona flagOverBuff";
        flagOverBuff = true;
    }
}

void Stream::calculateFft(double* dataIn, unsigned n)
{
    mFftIn  = fftw_alloc_real(n);
    mFftOut = fftw_alloc_real(n);
    mFftPlan = fftw_plan_r2r_1d(n, mFftIn, mFftOut, FFTW_R2HC, FFTW_ESTIMATE);

    memcpy(mFftIn, dataIn, n*sizeof(double));
    fftw_execute(mFftPlan);

    for (unsigned i = 0; i < n/2; i++)
    {
//        dataOut[i] = abs(mFftOut[i]);
        fftBufferOUT[i] = abs(mFftOut[i]);
//        qDebug() << "Value: " << abs(mFftOut[i]);
    }

    fftw_free(mFftIn);
    fftw_free(mFftOut);
    fftw_destroy_plan(mFftPlan);
}

double* Stream::getFftBuffer()
{
    if (fftBufferOUT != nullptr){
        return fftBufferOUT;
    } else
    {
        return nullptr;
    }
}

unsigned Stream::getFftSize()
{
//    return size/2;
    return offset/2;
}

void Stream::clearFft()
{
    flagReset = true;
}

void Stream::setXAxis(bool asIndex, double min, double max)
{
    xAsIndex = asIndex;
    xMin = min;
    xMax = max;

    // Note that x axis scaling is ignored when X is provided from source as data
    // TODO: assert (UI options for x axis should be disabled)
    if (!hasX())
    {
        xData = makeXBuffer();
        for (auto c : channels)
        {
            c->setX(xData);
        }
    }
}

void Stream::saveSettings(QSettings* settings) const
{
    _infoModel.saveSettings(settings);
}

void Stream::loadSettings(QSettings* settings)
{
    _infoModel.loadSettings(settings);
}
