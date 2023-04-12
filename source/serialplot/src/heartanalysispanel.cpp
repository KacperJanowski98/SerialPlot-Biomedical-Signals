#include "heartanalysispanel.h"
#include "ui_heartanalysispanel.h"

HeartAnalysisPanel::HeartAnalysisPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeartAnalysisPanel)
{
    ui->setupUi(this);
}

HeartAnalysisPanel::~HeartAnalysisPanel()
{
    delete ui;
}
