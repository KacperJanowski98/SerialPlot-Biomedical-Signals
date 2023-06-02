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

    void saveSettings(QSettings* settings);
    void loadSettings(QSettings* settings);

signals:
    void buttonApplyPressd(bool state);

private:
    Ui::FilterControl *ui;

    QButtonGroup filterVersionButtons;

private slots:
    void onButtonApply(bool state);
    void onFilterType(int index);
    void onFilterVersion(bool state);
public slots:
    void onPortToggled(bool open);
};

#endif // FILTERCONTROL_H
