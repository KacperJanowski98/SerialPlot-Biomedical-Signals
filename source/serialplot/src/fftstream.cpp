#include "fftstream.h"
#include "ringbuffer.h"
#include "indexbuffer.h"
#include "linindexbuffer.h"

#include <QDebug>

FftStream::FftStream(unsigned nc, bool x, unsigned ns) :
    _infoModel(nc)
{
    _numSamples = ns;
    _paused = false;

    xAsIndex = true;
    xMin = 0;
    xMax = 1;

    flag = true;

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

FftStream::~FftStream()
{
    for (auto ch : channels)
    {
        delete ch;
    }
    delete xData;
}

bool FftStream::hasX() const
{
    return _hasx;
}

unsigned FftStream::numChannels() const
{
    return channels.length();
}

unsigned FftStream::numSamples() const
{
    return _numSamples;
}

const StreamChannel* FftStream::channel(unsigned index) const
{
    Q_ASSERT(index < numChannels());
    return channels[index];
}

StreamChannel* FftStream::channel(unsigned index)
{
    return const_cast<StreamChannel*>(static_cast<const FftStream&>(*this).channel(index));
}

QVector<const StreamChannel*> FftStream::allChannels() const
{
    QVector<const StreamChannel*> result(numChannels());
    for (unsigned ci = 0; ci < numChannels(); ci++)
    {
        result[ci] = channel(ci);
    }
    return result;
}

const ChannelInfoModel* FftStream::infoModel() const
{
    return &_infoModel;
}

ChannelInfoModel* FftStream::infoModel()
{
    return const_cast<ChannelInfoModel*>(static_cast<const FftStream&>(*this).infoModel());
}

void FftStream::setNumChannels(unsigned nc, bool x)
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

XFrameBuffer* FftStream::makeXBuffer() const
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

const SamplePack* FftStream::applyGainOffset(const SamplePack& pack) const
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

void FftStream::feedIn(const SamplePack& pack)
{
    Q_ASSERT(pack.numChannels() == numChannels() &&
             pack.hasX() == hasX());

    if (_paused) return;

    unsigned ns = pack.numSamples();
    if (_hasx)
    {
        // TODO: implement XRingBuffer (binary search)
        Q_ASSERT(false);
        // static_cast<FftRingBuffer*>(xData)->addSamples(pack.xData(), ns);
    }

    // modified pack that gain and offset is applied to
    const SamplePack* mPack = nullptr;
    if (infoModel()->gainOrOffsetEn())
        mPack = applyGainOffset(pack);

    for (unsigned ci = 0; ci < numChannels(); ci++)
    {
        auto buf = static_cast<RingBuffer*>(channels[ci]->yData());
        double* data = (mPack == nullptr) ? pack.data(ci) : mPack->data(ci);
        // auto test = channels[ci]->xData()->size(); // wielkość buffora na dane wejściowe
        // Tu jest wejście danych (RingBuffer)
        double sampleFft[ns/2];
        calculateFft(data, sampleFft, ns);
//        buf->addSamples(data, ns);
        buf->addSamples(sampleFft, ns/2);
    }

    Sink::feedIn((mPack == nullptr) ? pack : *mPack);

    if (mPack != nullptr) delete mPack;
    emit dataAdded();
}

void FftStream::pause(bool paused)
{
    _paused = paused;
}

void FftStream::clear()
{
    for (auto c : channels)
    {
        static_cast<RingBuffer*>(c->yData())->clear();
    }
}

void FftStream::setNumSamples(unsigned value)
{
    if (value == _numSamples) return;
    _numSamples = value;

    xData->resize(value);
    for (auto c : channels)
    {
        static_cast<RingBuffer*>(c->yData())->resize(value);
    }
}

void FftStream::calculateFft(double* dataIn, double* dataOut, unsigned n)
{
    if (flag)
    {
        unsigned temp = n - 12;

        mFftIn  = fftw_alloc_real(temp);
        mFftOut = fftw_alloc_real(temp);
        mFftPlan = fftw_plan_r2r_1d(temp, mFftIn, mFftOut, FFTW_R2HC, FFTW_ESTIMATE);

        memcpy(mFftIn, dataIn + 12, temp*sizeof(double));
        fftw_execute(mFftPlan);

        flag = false;
    } else
    {
        mFftIn  = fftw_alloc_real(n);
        mFftOut = fftw_alloc_real(n);
        mFftPlan = fftw_plan_r2r_1d(n, mFftIn, mFftOut, FFTW_R2HC, FFTW_ESTIMATE);

        memcpy(mFftIn, dataIn, n*sizeof(double));
        fftw_execute(mFftPlan);
    }

    for (unsigned i = 0; i < n/2; i++)
    {
        dataOut[i] = abs(mFftOut[i]);
//        qDebug() << "Value: " << abs(mFftOut[i]);
    }
    fftw_free(mFftIn);
    fftw_free(mFftOut);
    fftw_destroy_plan(mFftPlan);
}

void FftStream::setXAxis(bool asIndex, double min, double max)
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

void FftStream::saveSettings(QSettings* settings) const
{
    _infoModel.saveSettings(settings);
}

void FftStream::loadSettings(QSettings* settings)
{
    _infoModel.loadSettings(settings);
}
