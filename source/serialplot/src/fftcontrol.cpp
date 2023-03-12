#include <QDebug>
#include <QMessageBox>

#include "fftcontrol.h"
#include "ui_fftcontrol.h"
#include "setting_defines.h"

FftControl::FftControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FftControl)
{
    ui->setupUi(this);

//    connect(ui->spinBoxSampling, SIGNAL(valueChanged(int)),
//            this, SLOT(onSamplingFrequency(int)));

//    connect(ui->spinBoxStart, SIGNAL(valueChanged(int)),
//            this, SLOT(onFrequencyRangeStart(int)));

//    connect(ui->spinBoxEnd, SIGNAL(valueChanged(int)),
//            this, SLOT(onFrequencyRangeStart(int)));

//    connect(ui->spinBoxSampling, SIGNAL(editingFinished()),
//            this, SLOT(onSamplingFrequency(int)));

//    connect(ui->spinBoxStart, SIGNAL(editingFinished()),
//            this, SLOT(onFrequencyRangeStart(int)));

//    connect(ui->spinBoxEnd, SIGNAL(editingFinished()),
//            this, SLOT(onFrequencyRangeStart(int)));

    connect(ui->buttonApply, SIGNAL(clicked(bool)),
            this, SLOT(onButtonApply(bool)));

    _samplingFreq = ui->spinBoxSampling->value();
    _freqRangeStart = ui->spinBoxStart->value();
    _freqRangeEnd = ui->spinBoxEnd->value();
}

FftControl::~FftControl()
{
    delete ui;
}

unsigned FftControl::getSamplingFreq()
{
    return ui->spinBoxSampling->value();
}

unsigned FftControl::getFreqRangeStart()
{
    return ui->spinBoxStart->value();
}

unsigned FftControl::getFreqRangeEnd()
{
    return ui->spinBoxEnd->value();
}

//void FftControl::onSamplingFrequency()
//{
//    _samplingFreq = getSamplingFreq();
////    _samplingFreq = value;
////    emit samplingFrequencyChanged(value);
//}

//void FftControl::onFrequencyRangeStart()
//{
//    _freqRangeStart = getFreqRangeStart();
////    _freqRangeStart = value;
////    emit frequencyRangeStartChanged(value);
//}

//void FftControl::onFrequencyRangeEnd()
//{
//    _freqRangeEnd = getFreqRangeEnd();
////    _freqRangeEnd = value;
////    emit frequencyRangeEndChanged(value);
//}

void FftControl::onButtonApply(bool state)
{
    auto sampleingFreq = getSamplingFreq();
    auto rangeEnd = getFreqRangeEnd();
    if (sampleingFreq < 2 * rangeEnd)
    {
        QMessageBox::critical(this, tr("Error"),
        tr("The sampling frequency fs must be greater than twice the highest frequency component in the measured signal. fmax <= fs/2, fmax = Frequency range end"));
        auto temp = rangeEnd * 2;
        ui->spinBoxSampling->setValue(temp);
        ui->spinBoxStart->setValue(getFreqRangeStart());
        ui->spinBoxEnd->setValue(getFreqRangeEnd());
    } else
    {
        ui->spinBoxSampling->setValue(getSamplingFreq());
        ui->spinBoxStart->setValue(getFreqRangeStart());
        ui->spinBoxEnd->setValue(getFreqRangeEnd());
    }
//    emit samplingFrequencyChanged(getSamplingFreq());
//    emit frequencyRangeStartChanged(getFreqRangeStart());
//    emit frequencyRangeEndChanged(getFreqRangeEnd());
//    emit samplingFrequencyChanged();
//    emit frequencyRangeStartChanged();
//    emit frequencyRangeEndChanged();
    _samplingFreq = getSamplingFreq();
    _freqRangeStart = getFreqRangeStart();
    _freqRangeEnd = getFreqRangeEnd();

    emit buttonApplyPressd(state);
}

void FftControl::saveSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_FftControl);
    settings->setValue(SG_FftControl_samplingFreq, _samplingFreq);
    settings->setValue(SG_FftControl_freqRangeStart, _freqRangeStart);
    settings->setValue(SG_FftControl_freqRangeEnd, _freqRangeEnd);
    settings->endGroup();
}

void FftControl::loadSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_FftControl);
    ui->spinBoxSampling->setValue(
                settings->value(SG_FftControl_samplingFreq, getSamplingFreq()).toInt());
    ui->spinBoxStart->setValue(
                settings->value(SG_FftControl_freqRangeStart, getFreqRangeStart()).toInt());
    ui->spinBoxEnd->setValue(
                settings->value(SG_FftControl_freqRangeEnd, getFreqRangeEnd()).toInt());
    settings->endGroup();
}
