#ifndef FFTSTREAM_H
#define FFTSTREAM_H

#include <QObject>
#include <QModelIndex>
#include <QVector>
#include <QSettings>

#include "sink.h"
#include "source.h"
#include "channelinfomodel.h"
#include "streamchannel.h"
#include "framebuffer.h"

class FftStream : public QObject, public Sink
{
    Q_OBJECT
    
public:
    /**
     * @param nc number of channels
     * @param x has X data input
     * @param ns number of samples
     */
    FftStream(unsigned nc = 1, bool x = false, unsigned ns = 2);
    ~FftStream();

    bool hasX() const;
    unsigned numChannels() const;

    unsigned numSamples() const;
    const StreamChannel* channel(unsigned index) const;
    StreamChannel* channel(unsigned index);
    QVector<const StreamChannel*> allChannels() const;
    const ChannelInfoModel* infoModel() const;
    ChannelInfoModel* infoModel();

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

private:
    unsigned _numSamples;
    bool _paused;

    bool _hasx;
    XFrameBuffer* xData;
    QList<StreamChannel*> channels;

    ChannelInfoModel _infoModel;

    bool xAsIndex;
    double xMin, xMax;

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

#endif // FFTSTREAM_H