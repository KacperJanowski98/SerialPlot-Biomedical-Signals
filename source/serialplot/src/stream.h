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

#ifndef STREAM_H
#define STREAM_H

#include <QObject>
#include <QModelIndex>
#include <QVector>
#include <QSettings>
#include <QTimer>

#include "sink.h"
#include "source.h"
#include "channelinfomodel.h"
#include "streamchannel.h"
#include "framebuffer.h"
#include "fft.h"
// IIR
#include "Iir.h"
#include "butterworthfilter.h"
#include "chebyshevI.h"

#include "filtermanager.h"

/**
 * Main waveform storage class. It consists of channels. Channels are
 * synchronized with each other.
 *
 * Implements `Sink` class for data entry. It's expected to be
 * connected to a `Device` source.
 */
class Stream : public QObject, public Sink
{
    Q_OBJECT

public:
    /**
     * @param nc number of channels
     * @param x has X data input
     * @param ns number of samples
     */
    Stream(unsigned nc = 1, bool x = false, unsigned ns = 2);
    ~Stream();

    bool hasX() const;
    unsigned numChannels() const;

    unsigned numSamples() const;
    const StreamChannel* channel(unsigned index) const;
    StreamChannel* channel(unsigned index);
    QVector<const StreamChannel*> allChannels() const;
    const ChannelInfoModel* infoModel() const;
    ChannelInfoModel* infoModel();
    // Fft
    double* getFftBuffer();
    unsigned getFftSize();
    void clearFft();
    // Filtering
    void setupFilter();
    void setFilterVersion(int version);
    void setFilterType(int type);
    void setFilterOrderButterworth(int order);
    void setFilterOrderChebyshevI(int order);
    void setFilterOrderChebyshevII(int order);
    void setSamplingFreqButterworth(double freq);
    void setSamplingFreqChebyshevI(double freq);
    void setSamplingFreqChebyshevII(double freq);
    void setCutoffFreqButterworth(double freq);
    void setCutoffFreqChebyshevI(double freq);
    void setCutoffFreqChebyshevII(double freq);
    void setCenterFreqButterworth(double freq);
    void setCenterFreqChebyshevI(double freq);
    void setCenterFreqChebyshevII(double freq);
    void setWidthFreqButterworth(double freq);
    void setWidthFreqChebyshevI(double freq);
    void setWidthFreqChebyshevII(double freq);
    void setRippleChebyshevI(double ripple);
    void setStopBandChebyshevII(double band);

    /// Saves channel information
    void saveSettings(QSettings* settings) const;
    /// Load channel information
    void loadSettings(QSettings* settings);

protected:
    // implementations for `Sink`
    virtual void setNumChannels(unsigned nc, bool x);
    virtual void feedIn(const SamplePack& pack);

signals:
    void numChannelsChanged(unsigned value);
    void numSamplesChanged(unsigned value);
    void channelAdded(const StreamChannel* chan);
    void channelNameChanged(unsigned channel, QString name); // TODO: does it stay?
    void dataAdded(); ///< emitted when data added to channel man.
    void fftBufferFull();

public slots:
    /// Change number of samples (buffer size)
    void setNumSamples(unsigned value);

    /// Change X axis style
    /// @note Ignored when X is provided by source (hasX == true)
    void setXAxis(bool asIndex, double min, double max);

    /// When paused data feed is ignored
    void pause(bool paused);

    /// Clears buffer data (fills with 0)
    void clear();

    void setFilterParameter();

private:
    unsigned _numSamples;
    bool _paused;

    bool _hasx;
    XFrameBuffer* xData;
    QList<StreamChannel*> channels;

    ChannelInfoModel _infoModel;

    bool xAsIndex;
    double xMin, xMax;
    double *temp_buf;

    // FFT
    Fft* mFft;
    unsigned mSize;

    // Filter
//    ChebyshevIFilter* mLowPass;
    FilterManager* mFilter;

    /**
     * Applies gain and offset to given pack.
     *
     * Caller is responsible for deleting returned `SamplePack`.
     *
     * @note Should be called only when gain or offset is enabled. Guard with
     * `ChannelInfoModel::gainOrOffsetEn()`.
     *
     * @param pack input data
     * @return modified data
     */
    const SamplePack* applyGainOffset(const SamplePack& pack) const;

    /// Returns a new virtual X buffer for settings
    XFrameBuffer* makeXBuffer() const;
};


#endif // STREAM_H
