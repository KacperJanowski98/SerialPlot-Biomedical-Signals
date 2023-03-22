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

void FftControl::onButtonApply(bool state)
{
    auto sampleingFreq = getSamplingFreq();
    auto rangeEnd = getFreqRangeEnd();
    if (sampleingFreq < 2 * (rangeEnd + 1))
    {
        auto temp = (sampleingFreq / 2) - 1;
        ui->spinBoxSampling->setValue(getSamplingFreq());
        ui->spinBoxStart->setValue(getFreqRangeStart());
        ui->spinBoxEnd->setValue(temp);
    } else
    {
        ui->spinBoxSampling->setValue(getSamplingFreq());
        ui->spinBoxStart->setValue(getFreqRangeStart());
        ui->spinBoxEnd->setValue(getFreqRangeEnd());
    }

    _samplingFreq = getSamplingFreq();
    _freqRangeStart = getFreqRangeStart();
    _freqRangeEnd = getFreqRangeEnd();

    emit buttonApplyPressd(state);
}

void FftControl::saveSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_FftControl);
    settings->setValue(SG_FftControl_samplingFreq, ui->spinBoxSampling->value());
    settings->setValue(SG_FftControl_freqRangeStart,  ui->spinBoxStart->value());
    settings->setValue(SG_FftControl_freqRangeEnd, ui->spinBoxEnd->value());
    settings->endGroup();
}

void FftControl::loadSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_FftControl);
    ui->spinBoxSampling->setValue(
                settings->value(SG_FftControl_samplingFreq, ui->spinBoxSampling->value()).toInt());
    ui->spinBoxStart->setValue(
                settings->value(SG_FftControl_freqRangeStart, ui->spinBoxStart->value()).toInt());
    ui->spinBoxEnd->setValue(
                settings->value(SG_FftControl_freqRangeEnd, ui->spinBoxEnd->value()).toInt());
    settings->endGroup();
}
