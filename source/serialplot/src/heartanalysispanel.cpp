#include "heartanalysispanel.h"
#include "ui_heartanalysispanel.h"

#include <QDebug>

HeartAnalysisPanel::HeartAnalysisPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeartAnalysisPanel)
{
    ui->setupUi(this);

    connect(ui->pbAnalyze, SIGNAL(clicked(bool)),
            this, SLOT(onButtonAnalyze(bool)));
}

HeartAnalysisPanel::~HeartAnalysisPanel()
{
    delete ui;
}

void HeartAnalysisPanel::onButtonAnalyze(bool state)
{
    emit buttonAnalyzePressed();

    qDebug() << "Second";
}
