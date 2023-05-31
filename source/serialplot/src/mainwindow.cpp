/*
  Copyright © 2021 Hasan Yavuz Özderya

  This file is part of serialplot.

  serialplot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  serialplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with serialplot.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QDesktopServices>
#include <QMap>
#include <QtDebug>
#include <QCommandLineParser>
#include <QFileInfo>
#include <qwt_plot.h>
#include <limits.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>

#include <plot.h>
#include <barplot.h>

#include "defines.h"
#include "setting_defines.h"

#if defined(Q_OS_WIN) && defined(QT_STATIC)
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

// TODO: depends on tab insertion order, a better solution would be to use object names
const QMap<int, QString> panelSettingMap({
        {0, "Port"},
        {1, "DataFormat"},
        {2, "Plot"},
        {3, "FilterControl"},
        {4, "FftControl"},
        {5, "HeartAnalysis"},
        {6, "Commands"},
        {7, "Record"},
        {8, "TextView"},
        {9, "Log"}
    });

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    aboutDialog(this),
    portControl(&serialPort),
    secondaryPlot(NULL),
    snapshotMan(this, &stream),
    commandPanel(&serialPort),
    dataFormatPanel(&serialPort),
    recordPanel(&stream),
    textView(&stream),
    updateCheckDialog(this),
    bpsLabel(&portControl, &dataFormatPanel, this),
    heartAnalysisPanel(&fftControl)
{
    ui->setupUi(this);

    plotMan = new PlotManager(ui->plotArea, &plotMenu, &stream);

    ui->tabWidget->insertTab(0, &portControl, "Port");
    ui->tabWidget->insertTab(1, &dataFormatPanel, "Data Format");
    ui->tabWidget->insertTab(2, &plotControlPanel, "Plot");
    ui->tabWidget->insertTab(3, &filterControl, "Filter Control");
    ui->tabWidget->insertTab(4, &fftControl, "FFT Control");
    ui->tabWidget->insertTab(5, &heartAnalysisPanel, "Heart Analysis");
    ui->tabWidget->insertTab(6, &commandPanel, "Commands");
    ui->tabWidget->insertTab(7, &recordPanel, "Record");
    ui->tabWidget->insertTab(8, &textView, "Text View");
    ui->tabWidget->setCurrentIndex(0);
    auto tbPortControl = portControl.toolBar();
    addToolBar(tbPortControl);
    addToolBar(recordPanel.toolbar());

    ui->plotToolBar->addAction(snapshotMan.takeSnapshotAction());
    menuBar()->insertMenu(ui->menuHelp->menuAction(), snapshotMan.menu());
    menuBar()->insertMenu(ui->menuHelp->menuAction(), commandPanel.menu());

    connect(&commandPanel, &CommandPanel::focusRequested, [this]()
            {
                this->ui->tabWidget->setCurrentWidget(&commandPanel);
                this->ui->tabWidget->showTabs();
            });

    tbPortControl->setObjectName("tbPortControl");
    ui->plotToolBar->setObjectName("tbPlot");

    setupAboutDialog();

    // init view menu
    ui->menuBar->insertMenu(ui->menuSecondary->menuAction(), &plotMenu);
    plotMenu.addSeparator();
    QMenu* tbMenu = plotMenu.addMenu("Toolbars");
    tbMenu->addAction(ui->plotToolBar->toggleViewAction());
    tbMenu->addAction(portControl.toolBar()->toggleViewAction());

    // init secondary plot menu
    auto group = new QActionGroup(this);
    group->addAction(ui->actionVertical);
    group->addAction(ui->actionHorizontal);

    // init UI signals

    // Secondary plot menu signals
    connect(ui->actionBarPlot, &QAction::triggered,
            this, &MainWindow::showBarPlot);

    connect(ui->actionVertical, &QAction::triggered,
            [this](bool checked)
            {
                if (checked) ui->splitter->setOrientation(Qt::Vertical);
            });

    connect(ui->actionHorizontal, &QAction::triggered,
            [this](bool checked)
            {
                if (checked) ui->splitter->setOrientation(Qt::Horizontal);
            });

    // Help menu signals
    QObject::connect(ui->actionHelpAbout, &QAction::triggered,
              &aboutDialog, &QWidget::show);

    QObject::connect(ui->actionCheckUpdate, &QAction::triggered,
              &updateCheckDialog, &QWidget::show);

    QObject::connect(ui->actionReportBug, &QAction::triggered,
                     [](){QDesktopServices::openUrl(QUrl(BUG_REPORT_URL));});

    // File menu signals
    QObject::connect(ui->actionExportCsv, &QAction::triggered,
                     this, &MainWindow::onExportCsv);

    QObject::connect(ui->actionExportSvg, &QAction::triggered,
                     this, &MainWindow::onExportSvg);

    QObject::connect(ui->actionSaveSettings, &QAction::triggered,
                     this, &MainWindow::onSaveSettings);

    QObject::connect(ui->actionLoadSettings, &QAction::triggered,
                     this, &MainWindow::onLoadSettings);

    ui->actionQuit->setShortcutContext(Qt::ApplicationShortcut);

    QObject::connect(ui->actionQuit, &QAction::triggered,
                     this, &MainWindow::close);

    // port control signals
    QObject::connect(&portControl, &PortControl::portToggled,
                     this, &MainWindow::onPortToggled);
    QObject::connect(&portControl, &PortControl::portToggled,
                     &filterControl, &FilterControl::onPortToggled);

    // plot control signals
    connect(&plotControlPanel, &PlotControlPanel::numOfSamplesChanged,
            this, &MainWindow::onNumOfSamplesChanged);

    connect(&plotControlPanel, &PlotControlPanel::numOfSamplesChanged,
            plotMan, &PlotManager::setNumOfSamples);

    connect(&plotControlPanel, &PlotControlPanel::yScaleChanged,
            plotMan, &PlotManager::setYAxis);

    connect(&plotControlPanel, &PlotControlPanel::xScaleChanged,
            &stream, &Stream::setXAxis);

    connect(&plotControlPanel, &PlotControlPanel::xScaleChanged,
            plotMan, &PlotManager::setXAxis);

    connect(&plotControlPanel, &PlotControlPanel::plotWidthChanged,
            plotMan, &PlotManager::setPlotWidth);

    // FFT
    connect(&stream, &Stream::fftBufferFull, this, &MainWindow::fftPlot);
    connect(ui->fftPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressOnFftPlot(QMouseEvent*)));

    // FFT Control
    connect(&fftControl, &FftControl::buttonApplyPressd,
            this, &MainWindow::onButtonApplyPressed);

    // Filter Control
    connect(&filterControl, &FilterControl::buttonApplyPressd,
            this, &MainWindow::onButtonApplyPressedFiltering);

    // FFT filtered data
    connect(&stream, &Stream::fftFilterBufferFull, this, &MainWindow::fftFilterPlot);

    // Analyze panel
    connect(&heartAnalysisPanel, &HeartAnalysisPanel::buttonAnalyzePressed,
            this, &MainWindow::onButtonAnalyzePressed);

    // plot toolbar signals
    QObject::connect(ui->actionClear, SIGNAL(triggered(bool)),
                     this, SLOT(clearPlot()));

    QObject::connect(snapshotMan.takeSnapshotAction(), &QAction::triggered,
                     plotMan, &PlotManager::flashSnapshotOverlay);

    QObject::connect(ui->actionPause, &QAction::triggered,
                     &stream, &Stream::pause);

    QObject::connect(ui->actionPause, &QAction::triggered,
                     [this](bool enabled)
                     {
                         if (enabled && !recordPanel.recordPaused())
                         {
                             dataFormatPanel.pause(true);
                         }
                         else
                         {
                             dataFormatPanel.pause(false);
                         }
                     });

    QObject::connect(&recordPanel, &RecordPanel::recordPausedChanged,
                     [this](bool enabled)
                     {
                         if (ui->actionPause->isChecked() && enabled)
                         {
                             dataFormatPanel.pause(false);
                         }
                     });

    connect(&serialPort, &QIODevice::aboutToClose,
            &recordPanel, &RecordPanel::onPortClose);

    // init plot
    numOfSamples = plotControlPanel.numOfSamples();
    stream.setNumSamples(numOfSamples);
    plotControlPanel.setChannelInfoModel(stream.infoModel());

    // init scales
    stream.setXAxis(plotControlPanel.xAxisAsIndex(),
                    plotControlPanel.xMin(), plotControlPanel.xMax());

    plotMan->setYAxis(plotControlPanel.autoScale(),
                      plotControlPanel.yMin(), plotControlPanel.yMax());
    plotMan->setXAxis(plotControlPanel.xAxisAsIndex(),
                      plotControlPanel.xMin(), plotControlPanel.xMax());
    plotMan->setNumOfSamples(numOfSamples);
    plotMan->setPlotWidth(plotControlPanel.plotWidth());

    // init bps (bits per second) counter
    ui->statusBar->addPermanentWidget(&bpsLabel);

    // Init sps (sample per second) counter
    spsLabel.setText("0sps");
    spsLabel.setToolTip(tr("samples per second (per channel)"));
    ui->statusBar->addPermanentWidget(&spsLabel);
    connect(&sampleCounter, &SampleCounter::spsChanged,
            this, &MainWindow::onSpsChanged);

    bpsLabel.setMinimumWidth(70);
    bpsLabel.setAlignment(Qt::AlignRight);
    spsLabel.setMinimumWidth(70);
    spsLabel.setAlignment(Qt::AlignRight);

    // init demo
    QObject::connect(ui->actionDemoMode, &QAction::toggled,
                     this, &MainWindow::enableDemo);

    QObject::connect(ui->actionDemoMode, &QAction::toggled,
                     plotMan, &PlotManager::showDemoIndicator);

    // init stream connections
    connect(&dataFormatPanel, &DataFormatPanel::sourceChanged,
            this, &MainWindow::onSourceChanged);
    onSourceChanged(dataFormatPanel.activeSource());

    // load default settings
    QSettings settings(PROGRAM_NAME, PROGRAM_NAME);
    loadAllSettings(&settings);

    // FFT Control
    sampleFreq = fftControl.getSamplingFreq();
    endRange = fftControl.getFreqRangeEnd();

    // FFT plot
    ui->fftPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->fftPlot->legend->setVisible(false);
    ui->fftPlot->yAxis->setLabel("");
    ui->fftPlot->xAxis->setLabel("Frequency [Hz]");
    ui->fftPlot->xAxis->setRange(0.0, endRange);
    ui->fftPlot->clearGraphs();
    ui->fftPlot->addGraph();
    ui->fftPlot->addGraph();

    ui->fftPlot->graph(0)->setPen(QPen(Qt::darkBlue));
    ui->fftPlot->graph(1)->setPen(QPen(Qt::darkGreen));
    ui->fftPlot->legend->setVisible(true);
    ui->fftPlot->graph(0)->setName("FFT basic signal");
    ui->fftPlot->graph(1)->setName("FFT filtered signal");

    // FFT visability
    connect(plotMan, &PlotManager::visabilityPlotChange, this, &MainWindow::fftPlotVisableChange);
    // Analysis visability
    connect(plotMan, &PlotManager::visabilityPlotChange,
            &heartAnalysisPanel, &HeartAnalysisPanel::analysisVisableChange);

    textItem = new QCPItemText(ui->fftPlot);
    connect(ui->fftPlot, &QCustomPlot::mouseMove, this, &MainWindow::onMouseMove);

    // Filter Control
    // Update control panel after load setting
    filterControl.filterControlPanelUpdate();

    // Connect full buffer for sample to analysis
    connect(&stream, &Stream::fftBufferFull,
            &heartAnalysisPanel, &HeartAnalysisPanel::bufferSampleFull);

    // Load fft visability
    loadVisabilityFft(&settings);

    updateFilterParameter();

    handleCommandLineOptions(*QApplication::instance());

    // panel log signal
    QList saveList = ui->tabLog->findChildren<QPushButton*>("pbBrowse");
    QPushButton *saveLog = saveList[0];
    connect(saveLog, &QPushButton::clicked,
            this, &MainWindow::selectLogFile);

    // ensure command panel has 1 command if none loaded
    if (!commandPanel.numOfCommands())
    {
        commandPanel.newCommandAction()->trigger();
    }

    // Important: This should be after newCommandAction is triggered
    // (above) we don't want user to be greeted with command panel on
    // the very first run.
    connect(commandPanel.newCommandAction(), &QAction::triggered, [this]()
            {
                this->ui->tabWidget->setCurrentWidget(&commandPanel);
                this->ui->tabWidget->showTabs();
            });
}

MainWindow::~MainWindow()
{
    if (serialPort.isOpen())
    {
        serialPort.close();
    }

    delete plotMan;

    delete ui;
    ui = NULL; // we check if ui is deleted in messageHandler
}

QVector<double> MainWindow::linspace(double start_in, double end_in, int num_in)
{

    QVector<double> linspaced;

    //        double start = static_cast<double>(start_in);
    //        double end = static_cast<double>(end_in);
    double num = static_cast<double>(num_in);

    if (num == 0) { return linspaced; }
    if (num == 1)
    {
        linspaced.push_back(start_in);
        return linspaced;
    }

    double delta = (end_in - start_in) / (num - 1);

    for(int i=0; i < num-1; ++i)
    {
        linspaced.push_back(start_in + delta * i);
    }
    linspaced.push_back(end_in); // I want to ensure that start and end
        // are exactly the same as the input
    return linspaced;
}

//void MainWindow::fftPlot()
void MainWindow::fftPlot(double* buffer, unsigned size)
{
    unsigned numSamples = size;
//    qDebug() << "Number of sample fftPlot: " << numSamples;

    QVector<double> vecY(buffer, buffer + numSamples);
    QVector<double> vecX = linspace(0.0,
                                    static_cast<double>((sampleFreq/2 - 1)),
                                    numSamples);
//    for (unsigned int i = 0; i < numSamples; i++)
//    {
//        qDebug() << "Czestotliwosc: " << std::ceil(vecX[i] * 100.0) / 100.0 << " wartosc: " << std::ceil(vecY[i] * 100.0) / 100.0;
//    }
    ui->fftPlot->graph(0)->setData(vecX, vecY);
//    QVector<double> vecY;
//    if ((sampleFreq/2 - 1) == endRange)
//    {
//        QVector<double> vecX = linspace(0.0,
//                                        static_cast<double>((sampleFreq/2 - 1)),
//                                        numSamples);
//        ui->fftPlot->graph(0)->setData(vecX, vecY);
//    } else
//    {
//        double newEnd = (numSamples * endRange)/(sampleFreq/2 - 1);
//        QVector<double> vecX = linspace(static_cast<int>(startRange),
//                                        static_cast<double>((sampleFreq/2 - 1)),
//                                        (numSamples - static_cast<int>(newEnd)));
//        QVector<double>::iterator first = vecY.begin() + startRange;
//        QVector<double>::iterator last = vecY.end() - (static_cast<int>(newEnd));
//        QVector<double> newVecY(first, last);
//        ui->fftPlot->graph(0)->setData(vecX, newVecY);
//    }

//    double freqStep = (double)sampleFreq / (double)numSamples;
//    double f = startRange;
//    while (f < endRange)
//    {
//        vecX.append(f);
//        f += freqStep;
//    }

//    for (unsigned i = (numSamples/sampleFreq)*startRange;
//         i < (numSamples/sampleFreq)*endRange;
//         i ++)

//    QVector<double> vecY(buffer, buffer + numSamples);
//    for (double d : vecY)
//    {
//        qDebug() << d;
//    }

//    for (unsigned i = 0; i < numSamples; i++)
//    {
//        qDebug() << "wartosc indexu: " << i;
//        vecY.append(buffer[i]);
//    }

//    qDebug() << "Dlugosc vecX" << vecX.length();
//    qDebug() << "Dlugosc vecY" << vecY.length();

//    ui->fftPlot->graph(0)->setData(vecX, vecY);
//    ui->fftPlot->graph(0)->setData(vecX.mid(0, vecY.length()), vecY);
    if (rescaleBasic == rescaleSampled)
    {
        ui->fftPlot->rescaleAxes();
    }
    else
    {
        if (rescaleBasic)
            ui->fftPlot->graph(0)->rescaleValueAxis();
        else if (rescaleSampled)
            ui->fftPlot->graph(1)->rescaleValueAxis();
    }
    ui->fftPlot->xAxis->setRange(0.0, endRange);
    ui->fftPlot->replot();
}

void MainWindow::fftFilterPlot(double* buffer, unsigned size)
{
    unsigned numSamples = size;
//    unsigned sampleFreq = 25; //ppg to 25
    QVector<double> vecY(buffer, buffer + numSamples);
    QVector<double> vecX = linspace(0.0,
                                    static_cast<double>((sampleFreq/2 - 1)),
                                    numSamples);
//    qDebug() << "First element before (filtered): " << vecX.front() << " " << vecY.front();
    vecX.erase(vecX.begin());
    vecY.erase(vecY.begin());
    ui->fftPlot->graph(1)->setData(vecX, vecY);
//    qDebug() << "First element after (filtered): " << vecX.front() << " " << vecY.front();

//    QVector<double> vecY;
//    QVector<double> vecX;

//    double freqStep = (double)sampleFreq / (double)numSamples;
//    double f = startRange;
//    while (f < endRange)
//    {
//        vecX.append(f);
//        f += freqStep;
//    }

//    for (unsigned i = (numSamples/sampleFreq)*startRange;
//         i < (numSamples/sampleFreq)*endRange;
//         i ++)
//    {
//        vecY.append(buffer[i]);
//    }
//    ui->fftPlot->graph(1)->setData(vecX.mid(0, vecY.length()), vecY);
    if (rescaleBasic == rescaleSampled)
    {
        ui->fftPlot->rescaleAxes();
    }
    else
    {
        if (rescaleBasic)
            ui->fftPlot->graph(0)->rescaleValueAxis();
        else if (rescaleSampled)
            ui->fftPlot->graph(1)->rescaleValueAxis();
    }
    ui->fftPlot->xAxis->setRange(0.0, endRange);
    ui->fftPlot->replot();
}

void MainWindow::mousePressOnFftPlot(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
//        ui->fftPlot->rescaleAxes();
        if (rescaleBasic == rescaleSampled)
        {
            ui->fftPlot->rescaleAxes();
        }
        else
        {
            if (rescaleBasic)
                ui->fftPlot->graph(0)->rescaleValueAxis();
            else if (rescaleSampled)
                ui->fftPlot->graph(1)->rescaleValueAxis();
        }

        ui->fftPlot->xAxis->setRange(0.0, endRange);
        ui->fftPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        ui->fftPlot->replot();
    }
}

void MainWindow::fftPlotVisableChange(int index, bool visable)
{
    if (index % 2 == 0)
    {
        rescaleBasic = visable;
        ui->fftPlot->graph(0)->setVisible(visable);
        if (visable)
        {
            ui->fftPlot->graph(0)->setName("FFT basic signal");
            if (rescaleBasic == rescaleSampled)
                ui->fftPlot->rescaleAxes();
            else if (rescaleBasic)
                ui->fftPlot->graph(0)->rescaleValueAxis();
            else if (rescaleSampled)
                ui->fftPlot->graph(1)->rescaleValueAxis();
        }
        else
        {
            ui->fftPlot->graph(0)->setName("");
            if (rescaleBasic == rescaleSampled)
                ui->fftPlot->rescaleAxes();
            else if (rescaleBasic)
                ui->fftPlot->graph(0)->rescaleValueAxis();
            else if (rescaleSampled)
                ui->fftPlot->graph(1)->rescaleValueAxis();
        }
    }
    else
    {
        rescaleSampled = visable;
        ui->fftPlot->graph(1)->setVisible(visable);
        if (visable)
        {
            ui->fftPlot->graph(1)->setName("FFT filtered signal");
            if (rescaleBasic == rescaleSampled)
                ui->fftPlot->rescaleAxes();
            else if (rescaleSampled)
                ui->fftPlot->graph(1)->rescaleValueAxis();
            else if (rescaleBasic)
                ui->fftPlot->graph(0)->rescaleValueAxis();
        }
        else
        {
            ui->fftPlot->graph(1)->setName("");
            if (rescaleBasic == rescaleSampled)
                ui->fftPlot->rescaleAxes();
            else if (rescaleSampled)
                ui->fftPlot->graph(1)->rescaleValueAxis();
            else if (rescaleBasic)
                ui->fftPlot->graph(0)->rescaleValueAxis();
        }
//        ui->fftPlot->replot();
    }

//    ui->fftPlot->rescaleAxes();
    ui->fftPlot->xAxis->setRange(0.0, endRange);
    ui->fftPlot->replot();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    // save snapshots
    if (!snapshotMan.isAllSaved())
    {
        auto clickedButton = QMessageBox::warning(
            this, "Closing SerialPlot",
            "There are un-saved snapshots. If you close you will loose the data.",
            QMessageBox::Discard, QMessageBox::Cancel);
        if (clickedButton == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    // save settings
    QSettings settings(PROGRAM_NAME, PROGRAM_NAME);
    saveAllSettings(&settings);
    settings.sync();

    if (settings.status() != QSettings::NoError)
    {
        QString errorText;

        if (settings.status() == QSettings::AccessError)
        {
            QString file = settings.fileName();
            errorText = QString("Serialplot cannot save settings due to access error. \
This happens if you have run serialplot as root (with sudo for ex.) previously. \
Try fixing the permissions of file: %1, or just delete it.").arg(file);
        }
        else
        {
            errorText = QString("Serialplot cannot save settings due to unknown error: %1").\
                arg(settings.status());
        }

        auto button = QMessageBox::critical(
            NULL,
            "Failed to save settings!", errorText,
            QMessageBox::Cancel | QMessageBox::Ok);

        if (button == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::updateFilterParameter()
{
    stream.setFilterVersion(filterControl.getFilterVersion());
    stream.setFilterType(filterControl.getFilterType());
    stream.setFilterOrderButterworth(filterControl.getFilterOrderButterworth());
    stream.setFilterOrderChebyshevI(filterControl.getFilterOrderChebyshevI());
    stream.setFilterOrderChebyshevII(filterControl.getFilterOrderChebyshevII());
    stream.setSamplingFreqButterworth(filterControl.getSamplingFreqButterworth());
    stream.setSamplingFreqChebyshevI(filterControl.getSamplingFreqChebyshevI());
    stream.setSamplingFreqChebyshevII(filterControl.getSamplingFreqChebyshevII());
    stream.setCutoffFreqButterworth(filterControl.getCutoffFreqButterworth());
    stream.setCutoffFreqChebyshevI(filterControl.getCutoffFreqChebyshevI());
    stream.setCutoffFreqChebyshevII(filterControl.getCutoffFreqChebyshevII());
    stream.setCenterFreqButterworth(filterControl.getCenterFreqButterworth());
    stream.setCenterFreqChebyshevI(filterControl.getCenterFreqChebyshevI());
    stream.setCenterFreqChebyshevII(filterControl.getCenterFreqChebyshevII());
    stream.setWidthFreqButterworth(filterControl.getWidthFreqButterworth());
    stream.setWidthFreqChebyshevI(filterControl.getWidthFreqChebyshevI());
    stream.setWidthFreqChebyshevII(filterControl.getWidthFreqChebyshevII());
    stream.setRippleChebyshevI(filterControl.getRippleChebyshevI());
    stream.setStopBandChebyshevII(filterControl.getStopBandChebyshevII());

    stream.setupFilter();
}

void MainWindow::setupAboutDialog()
{
    Ui_AboutDialog uiAboutDialog;
    uiAboutDialog.setupUi(&aboutDialog);

    QObject::connect(uiAboutDialog.pbAboutQt, &QPushButton::clicked,
                     [](){ QApplication::aboutQt();});

    QString aboutText = uiAboutDialog.lbAbout->text();
    aboutText.replace("$VERSION_STRING$", VERSION_STRING);
    aboutText.replace("$VERSION_REVISION$", VERSION_REVISION);
    uiAboutDialog.lbAbout->setText(aboutText);
}

void MainWindow::onButtonApplyPressed(bool state)
{
    sampleFreq = fftControl.getSamplingFreq();
    qDebug() << "onSamplingFrequencyChanged: " << sampleFreq;
    endRange = fftControl.getFreqRangeEnd();
    qDebug() << "onFrequencyRangeEndChanged: " << endRange;

    ui->fftPlot->xAxis->setRange(0.0, endRange);
    ui->fftPlot->replot();
}

void MainWindow::onButtonApplyPressedFiltering(bool state)
{
    stream.clear();
    plotMan->replot();
    // FFT
    stream.clearFft();
    ui->fftPlot->graph(0)->data()->clear();
    ui->fftPlot->graph(1)->data()->clear();
    ui->fftPlot->replot();
    updateFilterParameter();
}

void MainWindow::onPortToggled(bool open)
{
    // make sure demo mode is disabled
    if (open && isDemoRunning()) enableDemo(false);
    ui->actionDemoMode->setEnabled(!open);

    if (!open)
    {
        spsLabel.setText("0sps");
    }
}

void MainWindow::onSourceChanged(Source* source)
{
    source->connectSink(&stream);
    source->connectSink(&sampleCounter);
}

void MainWindow::clearPlot()
{
    stream.clear();
    plotMan->replot();
    // FFT
    stream.clearFft();
    ui->fftPlot->graph(0)->data()->clear();
    ui->fftPlot->graph(1)->data()->clear();
    ui->fftPlot->replot();
}

void MainWindow::onNumOfSamplesChanged(int value)
{
    numOfSamples = value;
    stream.setNumSamples(value);
    plotMan->replot();
}

void MainWindow::onSpsChanged(float sps)
{
    int precision = sps < 1. ? 3 : 0;
    spsLabel.setText(QString::number(sps, 'f', precision) + "sps");
}

bool MainWindow::isDemoRunning()
{
    return ui->actionDemoMode->isChecked();
}

void MainWindow::enableDemo(bool enabled)
{
    if (enabled)
    {
        if (!serialPort.isOpen())
        {
            dataFormatPanel.enableDemo(true);
        }
        else
        {
            ui->actionDemoMode->setChecked(false);
        }
    }
    else
    {
        dataFormatPanel.enableDemo(false);
        ui->actionDemoMode->setChecked(false);
    }
}

void MainWindow::showSecondary(QWidget* wid)
{
    if (secondaryPlot != NULL)
    {
        secondaryPlot->deleteLater();
    }

    secondaryPlot = wid;
    ui->splitter->addWidget(wid);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);
}

void MainWindow::hideSecondary()
{
    if (secondaryPlot == NULL)
    {
        qFatal("Secondary plot doesn't exist!");
    }

    secondaryPlot->deleteLater();
    secondaryPlot = NULL;
}

void MainWindow::showBarPlot(bool show)
{
    if (show)
    {
        auto plot = new BarPlot(&stream, &plotMenu);
        plot->setYAxis(plotControlPanel.autoScale(),
                       plotControlPanel.yMin(),
                       plotControlPanel.yMax());
        connect(&plotControlPanel, &PlotControlPanel::yScaleChanged,
                plot, &BarPlot::setYAxis);
        showSecondary(plot);
    }
    else
    {
        hideSecondary();
    }
}

void MainWindow::onMouseMove(QMouseEvent *event)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(sender());
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = customPlot->yAxis->pixelToCoord(event->pos().y());
    textItem->setText(QString("(%1, %2)").arg(x).arg(y));
    textItem->position->setCoords(QPointF(x, y));
    textItem->setFont(QFont(font().family(), 10));
    customPlot->replot();
}

bool MainWindow::selectLogFile()
{
    QString fileName = QFileDialog::getSaveFileName(
        parentWidget(), tr("Select file to save signal logs"));

    if (fileName.isEmpty())
    {
        return false;
    }
    else
    {
        QList fileList = ui->tabLog->findChildren<QLineEdit*>("lsFileName");
        QLineEdit *leFileName = fileList[0];
        leFileName->setText(fileName);
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return false;

        QTextStream out(&file);
        QList plainTextEdit = ui->tabLog->findChildren<QPlainTextEdit*>("ptLogStream");
        QPlainTextEdit *textLogs = plainTextEdit[0];
        out << textLogs->toPlainText();
        textLogs->clear();

        return true;
    }
}

void MainWindow::onButtonAnalyzePressed()
{
    bool wasPaused = ui->actionPause->isChecked();
    ui->actionPause->setChecked(true); // pause plotting

    const QString fileName = QDir::currentPath() + "/python_modules/data.csv";
    QDir dir(QDir::currentPath() + "/python_modules");

    if (QFileInfo::exists(fileName))
    {
        // Remove existed file with data.
        dir.remove("data.csv");
    }

    if (fileName.isNull())  // user canceled export
    {
        ui->actionPause->setChecked(wasPaused);
    }
    else
    {
        Snapshot* snapshot = snapshotMan.makeSnapshot();
        snapshot->save(fileName);
        delete snapshot;
    }
}

void MainWindow::onExportCsv()
{
    bool wasPaused = ui->actionPause->isChecked();
    ui->actionPause->setChecked(true); // pause plotting

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export CSV File"));

    if (fileName.isNull())  // user canceled export
    {
        ui->actionPause->setChecked(wasPaused);
    }
    else
    {
        Snapshot* snapshot = snapshotMan.makeSnapshot();
        snapshot->save(fileName);
        delete snapshot;
    }
}

void MainWindow::onExportSvg()
{
    bool wasPaused = ui->actionPause->isChecked();
    ui->actionPause->setChecked(true); // pause plotting

    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Export SVG File(s)"), QString(), "Images (*.svg)");

    if (fileName.isNull())  // user canceled export
    {
        ui->actionPause->setChecked(wasPaused);
    }
    else
    {
        plotMan->exportSvg(fileName);
    }
}

PlotViewSettings MainWindow::viewSettings() const
{
    return plotMenu.viewSettings();
}

const QString MainWindow::extractMsg(const QString &logString)
{
    std::string temp = logString.toStdString();
    std::string found_text;
    std::string::size_type start_position = 0;
    std::string::size_type end_position = 0;

    start_position = temp.find("\"");
    if (start_position != std::string::npos)
    {
        ++start_position; // start after the double quotes.
        // look for end position;
        end_position = temp.find("\"");
        if (end_position != std::string::npos)
        {
            found_text = temp.substr(start_position, end_position - start_position);
            found_text.pop_back();
        }
    }
    return QString::fromStdString(found_text);
}

void MainWindow::messageHandler(QtMsgType type,
                                const QString &logString,
                                const QString &msg)
{
    if (ui != NULL)
    {
        if (logString.toUpper().contains("LOG") || logString.toUpper().contains("ERR"))
            ui->ptLogStream->appendPlainText(extractMsg(logString));
        else
            ui->ptLog->appendPlainText(logString);
    }

    if (type != QtDebugMsg && ui != NULL)
    {
        ui->statusBar->showMessage(msg, 5000);
    }
}

void MainWindow::saveAllSettings(QSettings* settings)
{
    saveMWSettings(settings);
    portControl.saveSettings(settings);
    dataFormatPanel.saveSettings(settings);
    stream.saveSettings(settings);
    plotControlPanel.saveSettings(settings);
    plotMenu.saveSettings(settings);
    fftControl.saveSettings(settings);
    filterControl.saveSettings(settings);
    commandPanel.saveSettings(settings);
    recordPanel.saveSettings(settings);
    textView.saveSettings(settings);
    updateCheckDialog.saveSettings(settings);
    heartAnalysisPanel.saveSettings(settings);
}

void MainWindow::loadAllSettings(QSettings* settings)
{
    loadMWSettings(settings);
    portControl.loadSettings(settings);
    dataFormatPanel.loadSettings(settings);
    stream.loadSettings(settings);
    plotControlPanel.loadSettings(settings);
    plotMenu.loadSettings(settings);
    fftControl.loadSettings(settings);
    filterControl.loadSettings(settings);
    commandPanel.loadSettings(settings);
    recordPanel.loadSettings(settings);
    textView.loadSettings(settings);
    updateCheckDialog.loadSettings(settings);
    heartAnalysisPanel.loadSettings(settings);
    heartAnalysisPanel.loadVisability(settings);
}

void MainWindow::saveMWSettings(QSettings* settings)
{
    // save window geometry
    settings->beginGroup(SettingGroup_MainWindow);
    settings->setValue(SG_MainWindow_Size, size());
    settings->setValue(SG_MainWindow_Pos, pos());
    // save active panel
    settings->setValue(SG_MainWindow_ActivePanel,
                       panelSettingMap.value(ui->tabWidget->currentIndex()));
    // save panel minimization
    settings->setValue(SG_MainWindow_HidePanels,
                       ui->tabWidget->hideAction.isChecked());
    // save window maximized state
    settings->setValue(SG_MainWindow_Maximized,
                       bool(windowState() & Qt::WindowMaximized));
    // save toolbar/dockwidgets state
    settings->setValue(SG_MainWindow_State, saveState());
    settings->endGroup();
}

void MainWindow::loadMWSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_MainWindow);
    // load window geometry
    resize(settings->value(SG_MainWindow_Size, size()).toSize());
    move(settings->value(SG_MainWindow_Pos, pos()).toPoint());

    // set active panel
    QString tabSetting =
        settings->value(SG_MainWindow_ActivePanel, QString()).toString();
    ui->tabWidget->setCurrentIndex(
        panelSettingMap.key(tabSetting, ui->tabWidget->currentIndex()));

    // hide panels
    ui->tabWidget->hideAction.setChecked(
        settings->value(SG_MainWindow_HidePanels,
                        ui->tabWidget->hideAction.isChecked()).toBool());

    // maximize window
    if (settings->value(SG_MainWindow_Maximized).toBool())
    {
        showMaximized();
    }

    // load toolbar/dockwidgets state
    restoreState(settings->value(SG_MainWindow_State).toByteArray());
    settings->setValue(SG_MainWindow_State, saveState());

    settings->endGroup();
}

void MainWindow::loadVisabilityFft(QSettings* settings)
{
    settings->beginGroup(SettingGroup_Channels);
    unsigned size = settings->beginReadArray(SG_Channels_Channel);
    bool visable = false;

    for (unsigned ci = 0; ci < size; ci++)
    {
        settings->setArrayIndex(ci);
        if (ci % 2 == 0)
        {
            visable = settings->value(SG_Channels_Visible, visable).toBool();
            ui->fftPlot->graph(0)->setVisible(visable);
            rescaleBasic = visable;
            if (visable)
                ui->fftPlot->graph(0)->setName("FFT basic signal");
            else
                ui->fftPlot->graph(0)->setName("");
            ui->fftPlot->replot();
        }
        else
        {
            visable = settings->value(SG_Channels_Visible, visable).toBool();
            ui->fftPlot->graph(1)->setVisible(visable);
            rescaleSampled = visable;
            if (visable)
                ui->fftPlot->graph(1)->setName("FFT filtered signal");
            else
                ui->fftPlot->graph(1)->setName("");
            ui->fftPlot->replot();
        }
    }

    settings->endArray();
    settings->endGroup();
}

void MainWindow::onSaveSettings()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Settings"), QString(), "INI (*.ini)");

    if (!fileName.isNull()) // user canceled
    {
        QSettings settings(fileName, QSettings::IniFormat);
        saveAllSettings(&settings);
    }
}

void MainWindow::onLoadSettings()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Load Settings"), QString(), "INI (*.ini)");

    if (!fileName.isNull()) // user canceled
    {
        QSettings settings(fileName, QSettings::IniFormat);
        loadAllSettings(&settings);
    }
}

void MainWindow::handleCommandLineOptions(const QCoreApplication &app)
{
    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
    parser.setApplicationDescription("Small and simple software for plotting data from serial port in realtime.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configOpt({"c", "config"}, "Load configuration from file.", "filename");
    QCommandLineOption portOpt({"p", "port"}, "Set port name.", "port name");
    QCommandLineOption baudrateOpt({"b" ,"baudrate"}, "Set port baud rate.", "baud rate");
    QCommandLineOption openPortOpt({"o", "open"}, "Open serial port.");

    parser.addOption(configOpt);
    parser.addOption(portOpt);
    parser.addOption(baudrateOpt);
    parser.addOption(openPortOpt);

    parser.process(app);

    if (parser.isSet(configOpt))
    {
        QString fileName = parser.value(configOpt);
        QFileInfo fileInfo(fileName);

        if (fileInfo.exists() && fileInfo.isFile())
        {
            QSettings settings(fileName, QSettings::IniFormat);
            loadAllSettings(&settings);
        }
        else
        {
            qCritical() << "Configuration file not exist. Closing application.";
            std::exit(1);
        }
    }

    if (parser.isSet(portOpt))
    {
        portControl.selectPort(parser.value(portOpt));
    }

    if (parser.isSet(baudrateOpt))
    {
        portControl.selectBaudrate(parser.value(baudrateOpt));
    }

    if (parser.isSet(openPortOpt))
    {
        portControl.openPort();
    }
}
