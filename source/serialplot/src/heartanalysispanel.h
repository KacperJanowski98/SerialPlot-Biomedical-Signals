#ifndef HEARTANALYSISPANEL_H
#define HEARTANALYSISPANEL_H

#include <QWidget>
#include <QSettings>
#include "fftcontrol.h"
#include "pyhelper.h"

namespace Ui {
class HeartAnalysisPanel;
}

class HeartAnalysisPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HeartAnalysisPanel(FftControl *fftControl, QWidget *parent = nullptr);
    ~HeartAnalysisPanel();

    void clearBasic();
    void clearFiltered();

    void loadVisability(QSettings* settings);

signals:
    void buttonAnalyzePressed();

private:
    Ui::HeartAnalysisPanel *ui;
    FftControl* _fftControl;
    bool _visableBasic;
    bool _visableFiltered;
    CPyInstance *pyInstance;

public slots:
    void analysisVisableChange(int index, bool visable);

private slots:
    void onButtonAnalyze(bool state);
    void onButtonClose(bool state);
};

#endif // HEARTANALYSISPANEL_H
