#ifndef FILTERCONTROL_H
#define FILTERCONTROL_H

#include <QWidget>
#include <QButtonGroup>
#include <QSettings>
#include "filter.h"

namespace Ui {
class FilterControl;
}

class FilterControl : public QWidget
{
    Q_OBJECT

public:
    explicit FilterControl(QWidget *parent = nullptr);
    ~FilterControl();

    FilterVersion getFilterVersion();
    FilterType getFilterType();
    FilterOrder getFilterOrderButterworth();
    FilterOrder getFilterOrderChebyshevI();
    FilterOrder getFilterOrderChebyshevII();
    double getSamplingFreqButterworth();
    double getSamplingFreqChebyshevI();
    double getSamplingFreqChebyshevII();
    double getCutoffFreqButterworth();
    double getCutoffFreqChebyshevI();
    double getCutoffFreqChebyshevII();
    double getCenterFreqButterworth();
    double getCenterFreqChebyshevI();
    double getCenterFreqChebyshevII();
    double getWidthFreqChebyshevI();
    double getWidthFreqChebyshevII();
    double getRippleChebyshevI();
    double getStopBandChebyshevII();

    /// Stores plot settings into a `QSettings`
    void saveSettings(QSettings* settings);
    /// Loads plot settings from a `QSettings`.
    void loadSettings(QSettings* settings);

private:
    Ui::FilterControl *ui;

    QButtonGroup filterVersionButtons;

    FilterVersion _filterVersion;
    FilterType _filterType;
    FilterOrder _filterOrderB;
    FilterOrder _filterOrderChI;
    FilterOrder _filterOrderChII;
    double _samplingFreqB;
    double _samplingFreqChI;
    double _samplingFreqChII;
    double _cutoffFreqB;
    double _cutoffFreqChI;
    double _cutoffFreqChII;
    double _centerFreqB;
    double _centerFreqChI;
    double _centerFreqChII;
    double _widthFreqB;
    double _widthFreqChI;
    double _widthFreqChII;
    double _rippleChI;
    double _stopBandChII;
};

#endif // FILTERCONTROL_H
