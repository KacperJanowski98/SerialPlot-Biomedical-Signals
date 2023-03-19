#include "filtercontrol.h"
#include "ui_filtercontrol.h"

FilterControl::FilterControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterControl)
{
    ui->setupUi(this);
    if (ui->rbButterworth->isChecked())
    {
        ui->groupBoxButterworth->setEnabled(true);
        ui->groupBoxChebyshevI->setEnabled(false);
        ui->groupBoxChebyshevII->setEnabled(false);
    }
    else if (ui->rbChebyshevI->isChecked())
    {
        ui->groupBoxButterworth->setEnabled(false);
        ui->groupBoxChebyshevI->setEnabled(true);
        ui->groupBoxChebyshevII->setEnabled(false);
    }
    else if (ui->rbChebyshevII->isChecked())
    {
        ui->groupBoxButterworth->setEnabled(false);
        ui->groupBoxChebyshevI->setEnabled(false);
        ui->groupBoxChebyshevII->setEnabled(true);
    }
}

FilterControl::~FilterControl()
{
    delete ui;
}
