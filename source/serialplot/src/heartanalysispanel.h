#ifndef HEARTANALYSISPANEL_H
#define HEARTANALYSISPANEL_H

#include <QWidget>

namespace Ui {
class HeartAnalysisPanel;
}

class HeartAnalysisPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HeartAnalysisPanel(QWidget *parent = nullptr);
    ~HeartAnalysisPanel();

private:
    Ui::HeartAnalysisPanel *ui;
};

#endif // HEARTANALYSISPANEL_H
