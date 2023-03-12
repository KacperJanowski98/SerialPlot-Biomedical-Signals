#ifndef FFTCONTROL_H
#define FFTCONTROL_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class FftControl;
}

class FftControl : public QWidget
{
    Q_OBJECT

public:
    explicit FftControl(QWidget *parent = nullptr);
    ~FftControl();

    unsigned getSamplingFreq();
    unsigned getFreqRangeStart();
    unsigned getFreqRangeEnd();

    /// Stores plot settings into a `QSettings`
    void saveSettings(QSettings* settings);
    /// Loads plot settings from a `QSettings`.
    void loadSettings(QSettings* settings);

signals:
//    void samplingFrequencyChanged();
//    void frequencyRangeStartChanged();
//    void frequencyRangeEndChanged();
//    void samplingFrequencyChanged(int value);
//    void frequencyRangeStartChanged(int value);
//    void frequencyRangeEndChanged(int value);
    void buttonApplyPressd(bool state);

private:
    Ui::FftControl *ui;

    unsigned _samplingFreq;
    unsigned _freqRangeStart;
    unsigned _freqRangeEnd;

private slots:
//    void onSamplingFrequency(int value);
//    void onFrequencyRangeStart(int value);
//    void onFrequencyRangeEnd(int value);

//    void onSamplingFrequency();
//    void onFrequencyRangeStart();
//    void onFrequencyRangeEnd();

    void onButtonApply(bool state);
};

#endif // FFTCONTROL_H
