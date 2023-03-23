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
    _infoModel(nc*2)
{
    _numSamples = ns;
    _paused = false;

    xAsIndex = true;
    xMin = 0;
    xMax = 1;

    mFft = new Fft();
    mSize = 0;

    mFftFilter = new Fft();

    mFilter = new FilterManager();

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
    for (unsigned i = 0; i < nc*2; i++)
    {
        auto c = new StreamChannel(i, xData, new RingBuffer(ns), &_infoModel);
        // Create channel for filtering data
//        auto cF = new StreamChannel(i*10, xData, new RingBuffer(ns), &_infoModel);
        channels.append(c);
//        channels.append(cF);
    } 
}

Stream::~Stream()
{
    for (auto ch : channels)
    {
        delete ch;
    }
    delete xData;
    delete mFft;
    delete mFftFilter;
    delete mFilter;
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
    if (oldNum == nc*2 && x == _hasx) return;

    // adjust the number of channels
    if (nc*2 > oldNum) //nc
    {
        for (unsigned i = oldNum; i < nc*2; i++) // nc
        {
            auto c = new StreamChannel(i, xData, new RingBuffer(_numSamples), &_infoModel);
            channels.append(c);
        }
    }
    else if (nc*2 < oldNum) //nc
    {
        for (unsigned i = oldNum-1; i > (nc*2)-1; i--) // nc
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

    if (nc != oldNum) // nc
    {
        _infoModel.setNumOfChannels(nc*2); //nc
        // TODO: how about X change?
        emit numChannelsChanged(nc*2); //nc
    }

    Sink::setNumChannels(nc*2, x); // nc
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
    Q_ASSERT(pack.numChannels() == numChannels()/2 &&
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
        if (ci % 2 == 0) // Real data
        {
            auto buf = static_cast<RingBuffer*>(channels[ci]->yData());
            double* data = (mPack == nullptr) ? pack.data(ci) : mPack->data(ci);
            buf->addSamples(data, ns);
            mSize = buf->size();
            mFft->createFftBuffer(data, mSize, ns);
        }
        else // Filtered data
        {
            auto buf = static_cast<RingBuffer*>(channels[ci]->yData());
            double* data = (mPack == nullptr) ? pack.data(0) : mPack->data(0);
//            filterData(data, ns);
//            mLowPass->filterData(data, ns);
            mFilter->filterSignal(data, ns);
            buf->addSamples(data, ns);
            mFftFilter->createFftBuffer(data, buf->size(), ns);
//            mSize = buf->size();
//            mFft->createFftBuffer(data, mSize, ns);
        }
    }

    Sink::feedIn((mPack == nullptr) ? pack : *mPack);

    if (mPack != nullptr) delete mPack;
    if (mFft->getOffset() >= mFft->getSize())
    {
        qDebug() << "FFT ---------------------- przy rozmiarze: " << mFft->getSize();
        mFft->calculateFft();
        emit fftBufferFull();
    }
    if (mFftFilter->getOffset() >= mFftFilter->getSize())
    {
        mFftFilter->calculateFft();
        emit fftFilterBufferFull();
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

void Stream::setFilterParameter()
{
    qDebug() << "Udalo sie dostac w odpowiednie miejsce!";
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

double* Stream::getFftBuffer()
{
    return mFft->getFftBuffer();
}

unsigned Stream::getFftSize()
{
    return mFft->getFftSize();
}

void Stream::clearFft()
{
    mFft->clearFft();
    mFftFilter->clearFft();
}

double* Stream::getFftFilterBuffer()
{
    return mFftFilter->getFftBuffer();
}

unsigned Stream::getFftFilterSize()
{
    return mFftFilter->getFftSize();
}

void Stream::setupFilter()
{
    qDebug() << "Konfiguracja filtru";

    mFilter->setupFilter();
}

void Stream::setFilterVersion(int version)
{
    mFilter->setFilterVersion(version);

//    mFilter->setFilterVersion((int)FilterVersion::ChebyshevI);
//    mFilter->setFilterType((int)FilterType::LowPass);
//    mFilter->setFilterOrderChI((int)FilterOrder::Order5);
//    mFilter->setSamplingFreqChI(300);
//    mFilter->setCutoffFreqChI(10);
//    mFilter->setRippleChI(5);
}

void Stream::setFilterType(int type)
{
    mFilter->setFilterType(type);
}

void Stream::setFilterOrderButterworth(int order)
{
    mFilter->setFilterOrderB(order);
}

void Stream::setFilterOrderChebyshevI(int order)
{
    mFilter->setFilterOrderChI(order);
}

void Stream::setFilterOrderChebyshevII(int order)
{
    mFilter->setFilterOrderChII(order);
}

void Stream::setSamplingFreqButterworth(double freq)
{
    mFilter->setSamplingFreqB(freq);
}

void Stream::setSamplingFreqChebyshevI(double freq)
{
    mFilter->setSamplingFreqChI(freq);
}

void Stream::setSamplingFreqChebyshevII(double freq)
{
    mFilter->setSamplingFreqChII(freq);
}

void Stream::setCutoffFreqButterworth(double freq)
{
    mFilter->setCutoffFreqB(freq);
}

void Stream::setCutoffFreqChebyshevI(double freq)
{
    mFilter->setCutoffFreqChI(freq);
}

void Stream::setCutoffFreqChebyshevII(double freq)
{
    mFilter->setCutoffFreqChII(freq);
}

void Stream::setCenterFreqButterworth(double freq)
{
    mFilter->setCenterFreqB(freq);
}

void Stream::setCenterFreqChebyshevI(double freq)
{
    mFilter->setCenterFreqChI(freq);
}

void Stream::setCenterFreqChebyshevII(double freq)
{
    mFilter->setCenterFreqChII(freq);
}

void Stream::setWidthFreqButterworth(double freq)
{
    mFilter->setWidthFreqB(freq);
}

void Stream::setWidthFreqChebyshevI(double freq)
{
    mFilter->setWidthFreqChI(freq);
}

void Stream::setWidthFreqChebyshevII(double freq)
{
    mFilter->setWidthFreqChII(freq);
}

void Stream::setRippleChebyshevI(double ripple)
{
    mFilter->setRippleChI(ripple);
}

void Stream::setStopBandChebyshevII(double band)
{
    mFilter->setStopBandChII(band);
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
