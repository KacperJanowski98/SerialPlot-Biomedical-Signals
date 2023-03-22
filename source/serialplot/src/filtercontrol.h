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

    void filterControlPanelUpdate();
    void filterVersionPanelUpdate(int version);
    void filterLowHightPassPanelUpdate(int version);
    void filterBandPassStopPanelUpdate(int version);

    int getFilterVersion();
    int getFilterType();
    int getFilterOrderButterworth();
    int getFilterOrderChebyshevI();
    int getFilterOrderChebyshevII();
    double getSamplingFreqButterworth();
    double getSamplingFreqChebyshevI();
    double getSamplingFreqChebyshevII();
    double getCutoffFreqButterworth();
    double getCutoffFreqChebyshevI();
    double getCutoffFreqChebyshevII();
    double getCenterFreqButterworth();
    double getCenterFreqChebyshevI();
    double getCenterFreqChebyshevII();
    double getWidthFreqButterworth();
    double getWidthFreqChebyshevI();
    double getWidthFreqChebyshevII();
    double getRippleChebyshevI();
    double getStopBandChebyshevII();

    /// Stores plot settings into a `QSettings`
    void saveSettings(QSettings* settings);
    /// Loads plot settings from a `QSettings`.
    void loadSettings(QSettings* settings);

signals:
    void buttonApplyPressd(bool state);

private:
    Ui::FilterControl *ui;

    QButtonGroup filterVersionButtons;

//    int _filterVersion;
//    int _filterType;
//    int _filterOrderB;
//    int _filterOrderChI;
//    int _filterOrderChII;
//    double _samplingFreqB;
//    double _samplingFreqChI;
//    double _samplingFreqChII;
//    double _cutoffFreqB;
//    double _cutoffFreqChI;
//    double _cutoffFreqChII;
//    double _centerFreqB;
//    double _centerFreqChI;
//    double _centerFreqChII;
//    double _widthFreqB;
//    double _widthFreqChI;
//    double _widthFreqChII;
//    double _rippleChI;
//    double _stopBandChII;

private slots:
    void onButtonApply(bool state);
    void onFilterType(int index);
    void onFilterVersion(bool state);
public slots:
    void onPortToggled(bool open);
};

#endif // FILTERCONTROL_H
