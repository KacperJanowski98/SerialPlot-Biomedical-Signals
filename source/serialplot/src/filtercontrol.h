#ifndef FILTERCONTROL_H
#define FILTERCONTROL_H

#include <QWidget>

namespace Ui {
class FilterControl;
}

class FilterControl : public QWidget
{
    Q_OBJECT

public:
    explicit FilterControl(QWidget *parent = nullptr);
    ~FilterControl();

private:
    Ui::FilterControl *ui;
};

#endif // FILTERCONTROL_H
