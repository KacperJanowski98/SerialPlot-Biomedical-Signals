#include "filtercontrol.h"
#include "ui_filtercontrol.h"
#include "setting_defines.h"
#include <Qdebug.h>

FilterControl::FilterControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterControl)
{
    ui->setupUi(this);

    // setup filter version selection widget
    filterVersionButtons.addButton(ui->rbButterworth, static_cast<int>(FilterVersion::Butterworth));
    filterVersionButtons.addButton(ui->rbChebyshevI, static_cast<int>(FilterVersion::ChebyshevI));
    filterVersionButtons.addButton(ui->rbChebyshevII, static_cast<int>(FilterVersion::ChebyshevII));
}

FilterControl::~FilterControl()
{
    delete ui;
}

void FilterControl::saveSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_FilterControl);
    settings->setValue(SG_FilterControl_filterVersion, filterVersionButtons.checkedId());
    settings->setValue(SG_FilterControl_filterType, ui->cbFilterType->currentIndex());
    settings->setValue(SG_FilterControl_filterOrderB, ui->cbOrderButterworth->currentIndex());
    settings->setValue(SG_FilterControl_filterOrderChI, ui->cbOrderChebyshevI->currentIndex());
    settings->setValue(SG_FilterControl_filterOrderChII, ui->cbOrderChebyshevII->currentIndex());
    settings->setValue(SG_FilterControl_samplingFreqB, ui->sbSamplingFreqButterworth->value());
    settings->setValue(SG_FilterControl_samplingFreqChI, ui->sbSamplingFreqChebyshevI->value());
    settings->setValue(SG_FilterControl_samplingFreqChII, ui->sbSamplingFreqChebyshevII->value());
    settings->setValue(SG_FilterControl_cutoffFreqB, ui->sbCutoffFreqButterworth->value());
    settings->setValue(SG_FilterControl_cutoffFreqChI, ui->sbCutoffFreqChebyshevI->value());
    settings->setValue(SG_FilterControl_cutoffFreqChII, ui->sbCutoffFreqChebyshevII->value());
    settings->setValue(SG_FilterControl_centerFreqB, ui->sbCenterFreqButterworth->value());
    settings->setValue(SG_FilterControl_centerFreqChI, ui->sbCenterFreqChebyshevI->value());
    settings->setValue(SG_FilterControl_centerFreqChII, ui->sbCenterFreqChebyshevII->value());
    settings->setValue(SG_FilterControl_widthFreqB, ui->sbWidthFreqButterworth->value());
    settings->setValue(SG_FilterControl_widthFreqChI, ui->sbWidthFreqChebyshevI->value());
    settings->setValue(SG_FilterControl_widthFreqChII, ui->sbWidthFreqChebyshevII->value());
    settings->setValue(SG_FilterControl_rippleChI, ui->sbRippleChebyshevI->value());
    settings->setValue(SG_FilterControl_stopBandChII, ui->sbStopBandChebyshevII->value());
    settings->endGroup();
}

void FilterControl::loadSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_FilterControl);
    // load filter version
    int tempFilterVersion = settings->value(SG_FilterControl_filterVersion,
                                              filterVersionButtons.checkedId()).toInt();
    if (tempFilterVersion == static_cast<int>(FilterVersion::Butterworth))
    {
        ui->rbButterworth->setChecked(true);
    }
    else if (tempFilterVersion == static_cast<int>(FilterVersion::ChebyshevI))
    {
        ui->rbChebyshevI->setChecked(true);
    }
    else if (tempFilterVersion == static_cast<int>(FilterVersion::ChebyshevII))
    {
        ui-> rbChebyshevII->setChecked(true);
    }
    // load filter type
    ui->cbFilterType->setCurrentIndex(settings->value(SG_FilterControl_filterType,
                                        ui->cbFilterType->currentIndex()).toInt());
    // load Butterworth filter order
    ui->cbOrderButterworth->setCurrentIndex(settings->value(SG_FilterControl_filterOrderB,
                                                ui->cbOrderButterworth->currentIndex()).toInt());
    // load ChebyshevI filter order
    ui->cbOrderChebyshevI->setCurrentIndex(settings->value(SG_FilterControl_filterOrderChI,
                                                ui->cbOrderChebyshevI->currentIndex()).toInt());
    // load ChebyshevII filter order
    ui->cbOrderChebyshevII->setCurrentIndex(settings->value(SG_FilterControl_filterOrderChII,
                                                ui->cbOrderChebyshevII->currentIndex()).toInt());
    // load Butterworth sampling frequency
    ui->sbSamplingFreqButterworth->setValue(settings->value(SG_FilterControl_samplingFreqB,
                                                ui->sbSamplingFreqButterworth->value()).toInt());
    // load ChebyshevI sampling frequency
    ui->sbSamplingFreqChebyshevI->setValue(settings->value(SG_FilterControl_samplingFreqChI,
                                                ui->sbSamplingFreqChebyshevI->value()).toInt());
    // load ChebyshevII sampling frequency
    ui->sbSamplingFreqChebyshevII->setValue(settings->value(SG_FilterControl_samplingFreqChII,
                                                ui->sbSamplingFreqChebyshevII->value()).toInt());
    // load Butterworth cutoff frequency
    ui->sbCutoffFreqButterworth->setValue(settings->value(SG_FilterControl_cutoffFreqB,
                                                ui->sbCutoffFreqButterworth->value()).toInt());
    // load ChebyshevI cutoff frequency
    ui->sbCutoffFreqChebyshevI->setValue(settings->value(SG_FilterControl_cutoffFreqChI,
                                                ui->sbCutoffFreqChebyshevI->value()).toInt());
    // load ChebyshevII cutoff frequency
    ui->sbCutoffFreqChebyshevII->setValue(settings->value(SG_FilterControl_cutoffFreqChII,
                                                ui->sbCutoffFreqChebyshevII->value()).toInt());
    // load Butterworth center frequency
    ui->sbCenterFreqButterworth->setValue(settings->value(SG_FilterControl_centerFreqB,
                                                ui->sbCenterFreqButterworth->value()).toInt());
    // load ChebyshevI center frequency
    ui->sbCenterFreqChebyshevI->setValue(settings->value(SG_FilterControl_centerFreqChI,
                                                ui->sbCenterFreqChebyshevI->value()).toInt());
    // load ChebyshevII center frequency
    ui->sbCenterFreqChebyshevII->setValue(settings->value(SG_FilterControl_centerFreqChII,
                                                ui->sbCenterFreqChebyshevII->value()).toInt());
    // load Butterworth width frequency
    ui->sbWidthFreqButterworth->setValue(settings->value(SG_FilterControl_widthFreqB,
                                                ui->sbWidthFreqButterworth->value()).toInt());
    // load ChebyshevI width frequency
    ui->sbWidthFreqChebyshevI->setValue(settings->value(SG_FilterControl_widthFreqChI,
                                                ui->sbWidthFreqChebyshevI->value()).toInt());
    // load ChebyshevII width frequency
    ui->sbWidthFreqChebyshevII->setValue(settings->value(SG_FilterControl_widthFreqChII,
                                                ui->sbWidthFreqChebyshevII->value()).toInt());
    // load ChebyshevI ripple
    ui->sbRippleChebyshevI->setValue(settings->value(SG_FilterControl_rippleChI,
                                                ui->sbRippleChebyshevI->value()).toInt());
    // load ChebyshevII stop band
    ui->sbStopBandChebyshevII->setValue(settings->value(SG_FilterControl_stopBandChII,
                                                ui->sbStopBandChebyshevII->value()).toInt());
    settings->endGroup();
}
