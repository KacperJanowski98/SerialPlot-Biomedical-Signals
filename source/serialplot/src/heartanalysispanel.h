#ifndef HEARTANALYSISPANEL_H
#define HEARTANALYSISPANEL_H

#include <QWidget>
#include "pyhelper.h"

namespace Ui {
class HeartAnalysisPanel;
}

class HeartAnalysisPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HeartAnalysisPanel(QWidget *parent = nullptr);
    ~HeartAnalysisPanel();

signals:
    void buttonAnalyzePressed();

private:
    Ui::HeartAnalysisPanel *ui;

private slots:
    void onButtonAnalyze(bool state);
};

#endif // HEARTANALYSISPANEL_H
