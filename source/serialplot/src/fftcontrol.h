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
    unsigned getFreqRangeEnd();

    /// Stores plot settings into a `QSettings`
    void saveSettings(QSettings* settings);
    /// Loads plot settings from a `QSettings`.
    void loadSettings(QSettings* settings);

signals:
    void buttonApplyPressd(bool state);

private:
    Ui::FftControl *ui;

    unsigned _samplingFreq;
    unsigned _freqRangeEnd;

private slots:
    void onButtonApply(bool state);
};

#endif // FFTCONTROL_H
