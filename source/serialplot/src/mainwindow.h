/*
  Copyright © 2019 Hasan Yavuz Özderya

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QLabel>
#include <QString>
#include <QVector>
#include <QList>
#include <QSerialPort>
#include <QSignalMapper>
#include <QTimer>
#include <QColor>
#include <QtGlobal>
#include <QSettings>
#include <qwt_plot_curve.h>

#include "portcontrol.h"
#include "commandpanel.h"
#include "dataformatpanel.h"
#include "plotcontrolpanel.h"
#include "recordpanel.h"
#include "ui_about_dialog.h"
#include "stream.h"
#include "snapshotmanager.h"
#include "plotmanager.h"
#include "plotmenu.h"
#include "filtercontrol.h"
#include "fftcontrol.h"
#include "heartanalysispanel.h"
#include "updatecheckdialog.h"
#include "samplecounter.h"
#include "datatextview.h"
#include "bpslabel.h"

#include "qcustomplot.h"
#include "pyhelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    PlotViewSettings viewSettings() const;

    const QString extractMsg(const QString &logString);

    void messageHandler(QtMsgType type, const QString &logString, const QString &msg);

private:
    Ui::MainWindow *ui;

    QDialog aboutDialog;
    void setupAboutDialog();

    QSerialPort serialPort;
    PortControl portControl;

    unsigned int numOfSamples;

    QList<QwtPlotCurve*> curves;
    // ChannelManager channelMan;
    Stream stream;
    PlotManager* plotMan;
    QWidget* secondaryPlot;
    SnapshotManager snapshotMan;
    SampleCounter sampleCounter;

    QLabel spsLabel;
    CommandPanel commandPanel;
    DataFormatPanel dataFormatPanel;
    RecordPanel recordPanel;
    PlotControlPanel plotControlPanel;
    PlotMenu plotMenu;
    DataTextView textView;
    UpdateCheckDialog updateCheckDialog;
    BPSLabel bpsLabel;

    FilterControl filterControl;

    FftControl fftControl;
    unsigned endRange;
    unsigned sampleFreq;

    bool rescaleBasic;
    bool rescaleSampled;

    HeartAnalysisPanel heartAnalysisPanel;

    void handleCommandLineOptions(const QCoreApplication &app);

    /// Returns true if demo is running
    bool isDemoRunning();
    /// Display a secondary plot in the splitter, removing and
    /// deleting previous one if it exists
    void showSecondary(QWidget* wid);
    /// Hide secondary plot
    void hideSecondary();
    /// Stores settings for all modules
    void saveAllSettings(QSettings* settings);
    /// Load settings for all modules
    void loadAllSettings(QSettings* settings);
    /// Stores main window settings into a `QSettings`
    void saveMWSettings(QSettings* settings);
    /// Loads main window settings from a `QSettings`
    void loadMWSettings(QSettings* settings);
    // Load visability fft plot
    void loadVisabilityFft(QSettings* settings);

    /// `QWidget::closeEvent` handler
    void closeEvent(QCloseEvent * event);

    void updateFilterParameter();

    QVector<double> linspace(double start_in, double end_in, int num_in);

public slots:
    // FFT
//    void fftPlot();
    void fftPlot(double* buffer, unsigned size);
//    void fftFilterPlot();
    void fftFilterPlot(double* buffer, unsigned size);
    void mousePressOnFftPlot(QMouseEvent *event);
    void fftPlotVisableChange(int index, bool visable);

private slots:
    void onPortToggled(bool open);
    void onSourceChanged(Source* source);
    void onNumOfSamplesChanged(int value);

    // FFT control
//    void onSamplingFrequencyChanged();
//    void onFrequencyRangeStartChanged();
//    void onFrequencyRangeEndChanged();
//    void onSamplingFrequencyChanged(int value);
//    void onFrequencyRangeStartChanged(int value);
//    void onFrequencyRangeEndChanged(int value);
    void onButtonApplyPressed(bool state);
    void onButtonApplyPressedFiltering(bool state);

    void clearPlot();
    void onSpsChanged(float sps);
    void enableDemo(bool enabled);
    void showBarPlot(bool show);

    // log record panel
    bool selectLogFile();

    // Python data analysis
//    void onExportCsvPython();
    void onButtonAnalyzePressed();

    void onExportCsv();
    void onExportSvg();
    void onSaveSettings();
    void onLoadSettings();
};

#endif // MAINWINDOW_H
