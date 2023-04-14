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
//    explicit HeartAnalysisPanel(QWidget *parent = nullptr);
    explicit HeartAnalysisPanel(FftControl *fftControl, QWidget *parent = nullptr);
    ~HeartAnalysisPanel();

signals:
    void buttonAnalyzePressed();

private:
    Ui::HeartAnalysisPanel *ui;
//    CPyInstance* pyInstance;
//    std::unique_ptr<PyHelper> objBase;
//    std::unique_ptr<PyHelper> objFiltered;
//    PyHelper* objBase;
//    PyHelper* objFiltered;
    FftControl* _fftControl;

private slots:
    void onButtonAnalyze(bool state);
};

#endif // HEARTANALYSISPANEL_H
