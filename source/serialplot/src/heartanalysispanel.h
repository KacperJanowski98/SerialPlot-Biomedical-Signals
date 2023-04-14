#ifndef HEARTANALYSISPANEL_H
#define HEARTANALYSISPANEL_H

#include <QWidget>
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

signals:
    void buttonAnalyzePressed();

private:
    Ui::HeartAnalysisPanel *ui;
    FftControl* _fftControl;

private slots:
    void onButtonAnalyze(bool state);
    void onButtonClose(bool state);
};

#endif // HEARTANALYSISPANEL_H
