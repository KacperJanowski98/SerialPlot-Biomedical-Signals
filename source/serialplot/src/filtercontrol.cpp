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

    connect(ui->buttonApply, SIGNAL(clicked(bool)),
            this, SLOT(onButtonApply(bool)));
    connect(ui->rbButterworth, SIGNAL(toggled(bool)),
            this, SLOT(onFilterVersion(bool)));
    connect(ui->rbChebyshevI, SIGNAL(toggled(bool)),
            this, SLOT(onFilterVersion(bool)));
    connect(ui->rbChebyshevII, SIGNAL(toggled(bool)),
            this, SLOT(onFilterVersion(bool)));
    connect(ui->cbFilterType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onFilterType(int)));
}

FilterControl::~FilterControl()
{
    delete ui;
}

void FilterControl::filterControlPanelUpdate()
{
    auto version = getFilterVersion();
    auto type = getFilterType();
    // update active group box
    filterVersionPanelUpdate(version);
    // update active widgets in group box
    if (type == 0) // LowPass
        filterLowHightPassPanelUpdate(version);
    else if (type == 1) // HighPass
        filterLowHightPassPanelUpdate(version);
    else if (type == 2) // BandPass
        filterBandPassStopPanelUpdate(version);
    else if (type == 3) // BandStop
        filterBandPassStopPanelUpdate(version);
}

void FilterControl::filterVersionPanelUpdate(int version)
{
    switch (version) {
    case 0: // Butterworth
        ui->groupBoxButterworth->setEnabled(true);
        ui->groupBoxChebyshevI->setEnabled(false);
        ui->groupBoxChebyshevII->setEnabled(false);
        break;
    case 1: // ChebyshevI
        ui->groupBoxButterworth->setEnabled(false);
        ui->groupBoxChebyshevI->setEnabled(true);
        ui->groupBoxChebyshevII->setEnabled(false);
        break;
    case 2: // ChebyshevII
        ui->groupBoxButterworth->setEnabled(false);
        ui->groupBoxChebyshevI->setEnabled(false);
        ui->groupBoxChebyshevII->setEnabled(true);
        break;
    default:
        break;
    }
}

void FilterControl::filterLowHightPassPanelUpdate(int version)
{
    if (version == 0) // Butterworth
    {
        ui->sbSamplingFreqButterworth->setEnabled(true);
        ui->sbCutoffFreqButterworth->setEnabled(true);
        ui->sbCenterFreqButterworth->setEnabled(false);
        ui->sbWidthFreqButterworth->setEnabled(false);
    }
    else if (version == 1) // ChebyshevI
    {
        ui->sbSamplingFreqChebyshevI->setEnabled(true);
        ui->sbCutoffFreqChebyshevI->setEnabled(true);
        ui->sbCenterFreqChebyshevI->setEnabled(false);
        ui->sbWidthFreqChebyshevI->setEnabled(false);
        ui->sbRippleChebyshevI->setEnabled(true);
    }
    else if (version == 2) // ChebyshevII
    {
        ui->sbSamplingFreqChebyshevII->setEnabled(true);
        ui->sbCutoffFreqChebyshevII->setEnabled(true);
        ui->sbCenterFreqChebyshevII->setEnabled(false);
        ui->sbWidthFreqChebyshevII->setEnabled(false);
        ui->sbStopBandChebyshevII->setEnabled(true);
    }
}

void FilterControl::filterBandPassStopPanelUpdate(int version)
{
    if (version == 0) // Butterworth
    {
        ui->sbSamplingFreqButterworth->setEnabled(true);
        ui->sbCutoffFreqButterworth->setEnabled(false);
        ui->sbCenterFreqButterworth->setEnabled(true);
        ui->sbWidthFreqButterworth->setEnabled(true);
    }
    else if (version == 1) // ChebyshevI
    {
        ui->sbSamplingFreqChebyshevI->setEnabled(true);
        ui->sbCutoffFreqChebyshevI->setEnabled(false);
        ui->sbCenterFreqChebyshevI->setEnabled(true);
        ui->sbWidthFreqChebyshevI->setEnabled(true);
        ui->sbRippleChebyshevI->setEnabled(true);
    }
    else if (version == 2) // ChebyshevII
    {
        ui->sbSamplingFreqChebyshevII->setEnabled(true);
        ui->sbCutoffFreqChebyshevII->setEnabled(false);
        ui->sbCenterFreqChebyshevII->setEnabled(true);
        ui->sbWidthFreqChebyshevII->setEnabled(true);
        ui->sbStopBandChebyshevII->setEnabled(true);
    }
}

int FilterControl::getFilterVersion()
{
    return filterVersionButtons.checkedId();
}

int FilterControl::getFilterType()
{
    return ui->cbFilterType->currentIndex();
}

int FilterControl::getFilterOrderButterworth()
{
    return ui->cbOrderButterworth->currentIndex();
}

int FilterControl::getFilterOrderChebyshevI()
{
    return ui->cbOrderChebyshevI->currentIndex();
}

int FilterControl::getFilterOrderChebyshevII()
{
    return ui->cbOrderChebyshevII->currentIndex();
}

double FilterControl::getSamplingFreqButterworth()
{
    return ui->sbSamplingFreqButterworth->value();
}

double FilterControl::getSamplingFreqChebyshevI()
{
    return ui->sbSamplingFreqChebyshevI->value();
}

double FilterControl::getSamplingFreqChebyshevII()
{
    return ui->sbSamplingFreqChebyshevII->value();
}

double FilterControl::getCutoffFreqButterworth()
{
    return ui->sbCutoffFreqButterworth->value();
}

double FilterControl::getCutoffFreqChebyshevI()
{
    return ui->sbCutoffFreqChebyshevI->value();
}

double FilterControl::getCutoffFreqChebyshevII()
{
    return ui->sbCutoffFreqChebyshevII->value();
}

double FilterControl::getCenterFreqButterworth()
{
    return ui->sbCenterFreqButterworth->value();
}

double FilterControl::getCenterFreqChebyshevI()
{
    return ui->sbCenterFreqChebyshevI->value();
}

double FilterControl::getCenterFreqChebyshevII()
{
    return ui->sbCenterFreqChebyshevII->value();
}

double FilterControl::getWidthFreqButterworth()
{
    return ui->sbWidthFreqButterworth->value();
}

double FilterControl::getWidthFreqChebyshevI()
{
    return ui->sbWidthFreqChebyshevI->value();
}

double FilterControl::getWidthFreqChebyshevII()
{
    return ui->sbWidthFreqChebyshevII->value();
}

double FilterControl::getRippleChebyshevI()
{
    return ui->sbRippleChebyshevI->value();
}

double FilterControl::getStopBandChebyshevII()
{
    return ui->sbStopBandChebyshevII->value();
}

void FilterControl::onButtonApply(bool state)
{
    auto version = getFilterVersion();
    auto type = getFilterType();
    if (version == 0) // Butterworth
    {
        auto samplingFreq = getSamplingFreqButterworth();
        auto temp = (samplingFreq / 2) - 1;
        if (type == 0 or type == 1)
        {
            auto cutoffFreq = getCutoffFreqButterworth();
            if (samplingFreq < 2 * (cutoffFreq + 1))
                ui->sbCutoffFreqButterworth->setValue(temp);
        }
        else
        {
            auto centerFreq = getCenterFreqButterworth();
            if (samplingFreq < 2 * (centerFreq + 1))
                ui->sbCenterFreqButterworth->setValue(temp);
        }
    }
    else if (version == 1) // ChebyshevI
    {
        auto samplingFreq = getSamplingFreqChebyshevI();
        auto temp = (samplingFreq / 2) - 1;
        if (type == 0 or type == 1)
        {
            auto cutoffFreq = getCutoffFreqChebyshevI();
            if (samplingFreq < 2 * (cutoffFreq + 1))
                ui->sbCutoffFreqChebyshevI->setValue(temp);
        }
        else
        {
            auto centerFreq = getCenterFreqChebyshevI();
            if (samplingFreq < 2 * (centerFreq + 1))
                ui->sbCenterFreqChebyshevI->setValue(temp);
        }
    }
    else if (version == 2) // ChebyshevII
    {
        auto samplingFreq = getSamplingFreqChebyshevII();
        auto temp = (samplingFreq / 2) - 1;
        if (type == 0 or type == 1)
        {
            auto cutoffFreq = getCutoffFreqChebyshevII();
            if (samplingFreq < 2 * (cutoffFreq + 1))
                ui->sbCutoffFreqChebyshevII->setValue(temp);
        }
        else
        {
            auto centerFreq = getCenterFreqChebyshevII();
            if (samplingFreq < 2 * (centerFreq + 1))
                ui->sbCenterFreqChebyshevII->setValue(temp);
        }
    }

    emit buttonApplyPressd(state);
}

void FilterControl::onFilterType(int index)
{
    filterControlPanelUpdate();
}

void FilterControl::onFilterVersion(bool state)
{
    filterControlPanelUpdate();
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
