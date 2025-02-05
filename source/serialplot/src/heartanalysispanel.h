#ifndef HEARTANALYSISPANEL_H
#define HEARTANALYSISPANEL_H

#include <QWidget>
#include <QSettings>
#include <QButtonGroup>
#include "fftcontrol.h"
#include "pyhelper.h"

enum class SignalType
{
    PPG = 0,
    ECG = 1,
    EEG = 2
};

namespace Ui {
class HeartAnalysisPanel;
}

class HeartAnalysisPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HeartAnalysisPanel(
        FftControl *fftControl,
        QWidget *parent = nullptr);
    ~HeartAnalysisPanel();

    void clearBasic();
    void clearFiltered();

    void loadVisability(QSettings* settings);

    /// Saves channel information
    void saveSettings(QSettings* settings);
    /// Load channel information
    void loadSettings(QSettings* settings);

signals:
    void buttonAnalyzePressed();

private:
    Ui::HeartAnalysisPanel *ui;
    FftControl* _fftControl;
    bool _visableBasic;
    bool _visableFiltered;
    bool _analysisState;
    int signalType;
    CPyInstance *pyInstance;
    QButtonGroup signalTypeButtons;

    void makeAnalysis();
    int getSignalType();

public slots:
    void analysisVisableChange(int index, bool visable);
    void bufferSampleFull(double* buffer, unsigned size);

private slots:
    void signalTypeUpdate(bool state);
    void onButtonAnalyzeState(bool state);
    void onButtonAnalyze(bool state);
    void onButtonClose(bool state);
};

#endif // HEARTANALYSISPANEL_H
